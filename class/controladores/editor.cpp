#include "editor.h"

#include <fstream>
#include <string>

#include "../app/exportador.h"
#include "../app/importador.h"

#include "../app/framework_impl/input.h"

#include "../app/widget_editor_decoracion.h"
#include "../app/widget_editor_color.h"
#include "../app/widget_editor_obstaculo.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

const tcolor Objeto_editor::color_punto_ruta_editor={255, 255, 25, 128};
const tcolor Objeto_editor::color_generador_items_editor={0, 0, 255, 128};
const tcolor Objeto_editor::color_punto_inicio_editor={255, 0, 0, 128};
const tcolor Objeto_editor::color_punto_bot_editor={0, 0, 255, 255};
const tcolor Objeto_editor::color_seleccion={255, 255, 255, 255};
const tcolor Objeto_editor::color_obstaculo={128, 128, 128, 255};
const tcolor Objeto_editor::color_obstaculo_letal={255, 32, 32, 255};
const tcolor Objeto_editor::color_obstaculo_inocuo={32, 255, 32, 255};
const tcolor Objeto_editor::color_borde_obstaculo={210, 210, 128, 255};

Controlador_editor::Controlador_editor(DLibH::Log_base& l, const Fuentes& f)
	:log(l), 
	fuente_akashi(f.obtener_fuente("akashi", 16)),
	fuente_akashi_mensajes(f.obtener_fuente("akashi", 9)),
	camara(0, 0, 640, 400),
	mensajes(fuente_akashi_mensajes, 4, 16, 0),
	widget(nullptr),
	color_relleno({128, 128, 128, 255}),
	color_linea({128, 128, 128, 255}), grid(20), ver_flags(fvdeco_frente | fvdeco_fondo | fvobstaculos | fvconexiones),
	tobjeto(tobjetocreado::obstaculo),
	decoracion_frente(false)
{
	mensajes.insertar_mensaje("F1 para ayuda.", 2.f);
}

void Controlador_editor::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_editor::loop(DFramework::Input& input, float delta)
{
	mensajes.turno(delta);

	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
		return;
	}

	if(input.es_input_down(Input::ayuda))
	{
		solicitar_cambio_estado(ayuda_editor);
		return;
	}

	if(input.es_input_down(Input::cambio_logica))
	{
		solicitar_cambio_estado(principal);
		return;
	}

	//Una flag para saber si tenemos que llamar a inicializar. Cutre, pero nos vale.
	bool inicializar_widget=true;

	if(widget.get())
	{
		inicializar_widget=false;
		if(widget->es_cerrar())
		{
			widget->finalizar(input);
			//TODO: alguna forma de controlar esto???.
			reordenar_decoraciones();
			widget.reset(nullptr);
		}
		else
		{
			widget->input(input, delta);
			return;
		}
	}

	pos_raton=input.acc_posicion_raton();
	localizar_elementos_bajo_cursor();

	
	if(input.es_input_down(Input::escape)) poligono_construccion=Poligono_2d<double>{};

	if(input.es_input_down(Input::tab)) intercambiar_objeto_creado();

	if(input.es_input_down(Input::cargar_mapa)) cargar_mapa();	
	else if(input.es_input_down(Input::grabar_mapa)) grabar_mapa();

	if(input.es_input_pulsado(Input::control_izquierdo)) 
	{
		if(input.es_input_down(Input::tecla_1)) intercambiar_visibilidad(fvdeco_frente, "decoración frente");
		else if(input.es_input_down(Input::tecla_2)) intercambiar_visibilidad(fvdeco_fondo, "decoración fondo");
		else if(input.es_input_down(Input::tecla_3)) intercambiar_visibilidad(fvobstaculos, "obstáculos");
		else if(input.es_input_down(Input::tecla_4)) intercambiar_visibilidad(fvconexiones, "conexiones");
	}

	if(input.es_input_down(Input::click_i)) 
	{
		if(input.es_input_pulsado(Input::control_izquierdo)) seleccionar();
		else crear();
	}	
	else if(input.es_input_down(Input::click_d)) click_derecho();
	
	if(input.es_input_pulsado(Input::suprimir)) eliminar();

	if(input.es_input_pulsado(Input::control_izquierdo) && input.es_input_down(Input::copiar)) copiar();
	else if(input.es_input_pulsado(Input::control_izquierdo) && input.es_input_down(Input::pegar)) pegar();
	else if(input.es_input_down(Input::seleccion_color)) widget.reset(new Widget_editor_color(fuente_akashi, color_relleno, color_linea));

	if(input.es_input_down(Input::profundidad_mas)) cambiar_profundidad(1);
	else if(input.es_input_down(Input::profundidad_menos)) cambiar_profundidad(-1);

	if(input.es_input_pulsado(Input::control_izquierdo))
	{
		if(input.es_input_down(Input::cursor_arriba)) mover_seleccion(0.0, grid);
		else if(input.es_input_down(Input::cursor_abajo)) mover_seleccion(0.0, -grid);
		else if(input.es_input_down(Input::cursor_derecha)) mover_seleccion(grid, 0.0);
		else if(input.es_input_down(Input::cursor_izquierda)) mover_seleccion(-grid, 0.0);
	}
	else
	{
	//TODO: Constantes...
		if(input.es_input_pulsado(Input::cursor_arriba)) camara.movimiento_relativo(0, -500.0 * (double) delta);
		else if(input.es_input_pulsado(Input::cursor_abajo)) camara.movimiento_relativo(0, 500.0 * (double) delta);

		if(input.es_input_pulsado(Input::cursor_derecha)) camara.movimiento_relativo(500.0 * (double) delta, 0);
		else if(input.es_input_pulsado(Input::cursor_izquierda)) camara.movimiento_relativo(-500.0 * (double) delta, 0);
	}

	if(input.es_input_down(Input::TEST_VISIBILIDAD))
	{
		aplicar_a_mapa(mapa);
		mapa.construir_nodos_ruta();
	}

	if(input.es_input_pulsado(Input::zoom_mas)) 
	{
		double zoom=camara.acc_zoom()-(double)delta;
		if(zoom > 10.0) zoom=10.0;
		camara.mut_zoom(zoom);
	}
	else if(input.es_input_pulsado(Input::zoom_menos)) 
	{
		double zoom=camara.acc_zoom()+(double)delta;
		if(zoom < 0.10) zoom=0.10;
		camara.mut_zoom(zoom);
	}
	else if(input.es_input_down(Input::reset_zoom))
	{
		camara.mut_zoom(1.0);
	}

	if(input.es_input_down(Input::grid_menos)) cambiar_grid(-1);
	else if(input.es_input_down(Input::grid_mas)) cambiar_grid(1);

	if(inicializar_widget && widget.get()) widget->inicializar(input);
}

void Controlador_editor::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_editor::dibujar(ldv::screen& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);

	Representador r;
	r.dibujar_rejilla(pantalla, grid, {255, 255, 255, 64}, camara.acc_x(), -camara.acc_y(), camara.acc_zoom());

	auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y);

	if(ver_flags & fvobstaculos) for(const auto& eo : obstaculos) eo.dibujar(r, pantalla, camara);
	if(ver_flags & fvdeco_fondo) for(const auto& ed : decoraciones) if(!ed.elemento.es_frente()) ed.dibujar(r, pantalla, camara);
	if(ver_flags & fvdeco_frente) for(const auto& ed : decoraciones) if(ed.elemento.es_frente()) ed.dibujar(r, pantalla, camara);

	for(const auto& pi : puntos_inicio) pi.dibujar(r, pantalla, camara);
	for(const auto& pb : puntos_bot) pb.dibujar(r, pantalla, camara);
	for(const auto& eg : generadores_items) eg.dibujar(r, pantalla, camara);
	for(const auto& op : puntos_ruta) op.dibujar(r, pantalla, camara);
	for(const auto& oc : objetos_cursor) oc->dibujar(r, pantalla, camara, true);
	for(const auto& os : objetos_seleccionados) os->dibujar(r, pantalla, camara, true);

	if(ver_flags & fvconexiones)
	{
		for(const auto& nr : mapa.acc_nodos_ruta())
		{
			for(const auto& c : nr.conexiones)
			{
				Segmento_2d<double> s{ {nr.origen.pt.x, nr.origen.pt.y}, {c.destino.origen.pt.x, c.destino.origen.pt.y}};
				r.dibujar_segmento(pantalla, s, {255, 255, 25, 64}, camara);
			}
		}
	}

	for(const auto& s : poligono_construccion.acc_segmentos())
		r.dibujar_segmento(pantalla, s, {0, 255, 0, 128}, camara);
	
	//Segmento en construcción...
	if(poligono_construccion.acc_vertices().size())
	{
		const auto& v=poligono_construccion.acc_vertices().back();
		Segmento_2d<double> s{ {v.x, v.y}, {pt_raton.x, pt_raton.y}};
		r.dibujar_segmento(pantalla, s, {0, 255, 0, 128}, camara);
	}

	std::string texto="GRID: "+std::to_string((int)grid)
		+" CAM: "+std::to_string((int)camara.acc_x())+","+std::to_string((int)camara.acc_y())
		+" POS: "+std::to_string((int)pt_raton.x)+","+std::to_string((int)pt_raton.y);

	switch(tobjeto)
	{
		case tobjetocreado::obstaculo: texto+=" [geometry]"; break;
		case tobjetocreado::decoracion: texto+=" [decoration]"; break;
		case tobjetocreado::punto_ruta: texto+=" [waypoint]"; break;
		case tobjetocreado::inicio: texto+=" [spawn]"; break;
		case tobjetocreado::bot: texto+=" [bot]"; break;
		case tobjetocreado::arma: texto+=" [weapon]"; break;
	}

	ldv::ttf_font txt(fuente_akashi, {255, 255, 255, 255}, texto);
	txt.go_to(16, 380);
	txt.draw(pantalla);

	r.dibujar_poligono(pantalla, Objeto_editor::cuadrado(pt_raton.x, pt_raton.y, 3), {255, 255, 255, 128}, camara);

	//Color fondo y de línea...
	r.dibujar_poligono_sin_transformar(pantalla, Objeto_editor::cuadrado(570, 30, 10), color_relleno);
	r.dibujar_poligono_sin_transformar(pantalla, Objeto_editor::cuadrado(580, 40, 10), color_linea);

	mensajes.dibujar(pantalla);
	if(widget.get()) widget->dibujar(pantalla);
}

void Controlador_editor::despertar()
{

}

void Controlador_editor::dormir()
{
	tobjeto=tobjetocreado::obstaculo;
	poligono_construccion=Poligono_2d<double>{};
}

bool Controlador_editor::es_posible_abandonar_estado() const
{
	return true;
}

//////////////////////

void Controlador_editor::cargar_mapa()
{
	const std::string nombre_fichero="mapa.dat";

	mapa.limpiar();
	Importador importador;
	importador.importar(nombre_fichero.c_str(), mapa.obstaculos, mapa.decoraciones, mapa.puntos_inicio, mapa.puntos_bot, mapa.puntos_ruta, mapa.generadores_items);
	obtener_desde_mapa();
	mapa.limpiar();
	reordenar_decoraciones();

	mensajes.insertar_mensaje(nombre_fichero+" cargado de disco", 2.f);
}

void Controlador_editor::grabar_mapa()
{
	const std::string nombre_fichero="mapa.dat";

	std::ofstream fichero(nombre_fichero.c_str());
	Exportador exportador;
	aplicar_a_mapa(mapa);
	fichero<<exportador.serializar(mapa.obstaculos, mapa.decoraciones, mapa.puntos_inicio, mapa.puntos_bot, mapa.puntos_ruta, mapa.generadores_items);

	mensajes.insertar_mensaje(nombre_fichero+" guardado en disco", 2.f);
}

void Controlador_editor::intercambiar_objeto_creado()
{
	switch(tobjeto)
	{
		case tobjetocreado::obstaculo: 		tobjeto=tobjetocreado::decoracion; break;
		case tobjetocreado::decoracion: 	tobjeto=tobjetocreado::punto_ruta; break;
		case tobjetocreado::punto_ruta: 	tobjeto=tobjetocreado::inicio; break;
		case tobjetocreado::inicio: 		tobjeto=tobjetocreado::bot; break;
		case tobjetocreado::bot: 		tobjeto=tobjetocreado::arma; break;
		case tobjetocreado::arma: 		tobjeto=tobjetocreado::obstaculo; break;
	}
}

void Controlador_editor::crear()
{
	switch(tobjeto)
	{
		case tobjetocreado::obstaculo:
		case tobjetocreado::decoracion:
			nuevo_punto(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::punto_ruta: 
			crear_punto_ruta(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::inicio:
			crear_punto_inicio(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::bot:
			crear_punto_bot(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::arma:
			crear_generador_items(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
	}
}

void Controlador_editor::click_derecho()
{
	switch(tobjeto)
	{
		case tobjetocreado::obstaculo:
			if(ver_flags & fvobstaculos)
			{
				std::vector<Obstaculo_editor *> v;
				localizar_elementos_bajo_cursor_helper(obstaculos, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
				if(v.size()==1) widget.reset(new Widget_editor_obstaculo(fuente_akashi, v[0]->elemento));
			}
		break;
		case tobjetocreado::decoracion:
			if(ver_flags & fvdeco_fondo || ver_flags & fvdeco_frente)
			{
				std::vector<Decoracion_editor *> v;
				localizar_elementos_bajo_cursor_helper(decoraciones, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
				if(v.size()==1) widget.reset(new Widget_editor_decoracion(fuente_akashi, v[0]->elemento));
			}
		break;
		case tobjetocreado::punto_ruta: 

		break;
		case tobjetocreado::inicio:

		break;
		case tobjetocreado::bot:

		break;
		case tobjetocreado::arma:

		break;
	}
}

void Controlador_editor::seleccionar()
{	
	if(objetos_cursor.size())
	{
		for(auto& o : objetos_cursor)
		{
			auto it=std::find(std::begin(objetos_seleccionados), std::end(objetos_seleccionados), o);

			if(it!=std::end(objetos_seleccionados))
			{
				objetos_seleccionados.erase(it);
			}
			else
			{
				objetos_seleccionados.push_back(o);
			}
		}
	}
	else
	{
		objetos_seleccionados.clear();
	}
}

void Controlador_editor::crear_punto_inicio(ldt::point_2d<double> pt)
{
	puntos_inicio.push_back(Punto_inicio_editor{pt});
}

void Controlador_editor::crear_punto_bot(ldt::point_2d<double> pt)
{
	puntos_bot.push_back(Punto_bot_editor{pt});
}

void Controlador_editor::crear_punto_ruta(ldt::point_2d<double> pt)
{
	puntos_ruta.push_back(Punto_ruta_editor{pt});
}

void Controlador_editor::crear_generador_items(ldt::point_2d<double> pt)
{
	generadores_items.push_back(Generador_items_editor{pt});
}

ldt::point_2d<double>	Controlador_editor::punto_desde_pos_pantalla(int x, int y, bool a_rejilla)
{

	int px=camara.acc_x() + (x * camara.acc_zoom());
	int py=-camara.acc_y() - (y * camara.acc_zoom());

	if(a_rejilla)
	{
		px=round((double)px / (double)grid) * grid;
		py=round((double)py / (double)grid) * grid;
	}

	return ldt::point_2d<double>{(double)px, (double)py};
}

void Controlador_editor::nuevo_punto(ldt::point_2d<double> p)
{
	if(poligono_construccion.acc_vertices().size() > 2 && p==poligono_construccion.acc_vertices()[0])
	{
		cerrar_poligono();
	}
	else
	{
		const auto& v=poligono_construccion.acc_vertices();
		if(std::any_of(std::begin(v), std::end(v), [p](const ldt::point_2d<double>& b) {return p==b;}))
		{
			mensajes.insertar_mensaje("Vértice repetido, no se crea.", 2.f);
		}
		else
		{
			poligono_construccion.add_vertex(p);
		}
	}
}

void Controlador_editor::cerrar_poligono()
{
	if(poligono_construccion.size() < 3)
	{
		mensajes.insertar_mensaje("Polígono con menos de dos vértices", 2.f);
	}
	else
	{
		if(!poligono_construccion.es_sentido_horario())
		{
			mensajes.insertar_mensaje("Polígono no trazado en sentido horario", 2.f);
		}
		else
		{
			if(tobjeto==tobjetocreado::obstaculo)
			{
				if(poligono_construccion.es_concavo())
				{
					mensajes.insertar_mensaje("Polígono cóncavo o erróneo", 2.f);
				}
				else
				{
					poligono_construccion.cerrar();
					obstaculos.push_back(Obstaculo_editor(Obstaculo(poligono_construccion, Obstaculo::ttipos::normal)));
				}
			}
			else if(tobjeto==tobjetocreado::decoracion)
			{
				poligono_construccion.cerrar();
				decoraciones.push_back(Decoracion_editor(Decoracion(poligono_construccion, color_relleno, color_linea, decoracion_frente, 100)));
			}
		}
	}

	poligono_construccion=Poligono_2d<double>{};
}

void Controlador_editor::localizar_elementos_bajo_cursor()
{
	auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false);
	objetos_cursor.clear();

	if(ver_flags & fvobstaculos) localizar_elementos_bajo_cursor_helper(obstaculos, objetos_cursor, pt_raton);
	if(ver_flags & fvdeco_fondo || ver_flags & fvdeco_frente) localizar_elementos_bajo_cursor_helper(decoraciones, objetos_cursor, pt_raton);

	localizar_elementos_bajo_cursor_helper(puntos_inicio, objetos_cursor, pt_raton);
	localizar_elementos_bajo_cursor_helper(puntos_bot, objetos_cursor, pt_raton);
	localizar_elementos_bajo_cursor_helper(generadores_items, objetos_cursor, pt_raton);
	localizar_elementos_bajo_cursor_helper(puntos_ruta, objetos_cursor, pt_raton);
}

void Controlador_editor::mover_seleccion(double x, double y)
{
	for(auto& o : objetos_seleccionados) o->mover(x, y);
}

void Controlador_editor::aplicar_a_mapa(Mapa& m)
{
	m.limpiar();
	for(auto& o : obstaculos) m.obstaculos.push_back(o.elemento);
	for(auto& o : decoraciones) m.decoraciones.push_back(o.elemento);
	for(auto& o : puntos_ruta) m.puntos_ruta.push_back(o.elemento);
	for(auto& o : puntos_inicio) m.puntos_inicio.push_back(o.elemento);
	for(auto& o : puntos_bot) m.puntos_bot.push_back(o.elemento);
	for(auto& o : generadores_items) m.generadores_items.push_back(o.elemento);	
}

void Controlador_editor::obtener_desde_mapa()
{
	obstaculos.clear();
	decoraciones.clear();
	puntos_ruta.clear();
	puntos_inicio.clear();
	puntos_bot.clear();
	generadores_items.clear();

	for(auto& o : mapa.obstaculos) obstaculos.push_back(Obstaculo_editor(o));
	for(auto& o : mapa.decoraciones) decoraciones.push_back(Decoracion_editor(o));
	for(auto& o : mapa.puntos_ruta) puntos_ruta.push_back(Punto_ruta_editor(o));
	for(auto& o : mapa.puntos_inicio) puntos_inicio.push_back(Punto_inicio_editor(o));
	for(auto& o : mapa.puntos_bot) puntos_bot.push_back(Punto_bot_editor(o));
	for(auto& o : mapa.generadores_items) generadores_items.push_back(Generador_items_editor(o));
}

void Controlador_editor::eliminar()
{
	//Esto los marca para borrar...
	for(auto& o : objetos_seleccionados) o->borrar();
	objetos_seleccionados.clear();

	//Sólo borramos bajo el cursor los del tipo activo.
	auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false);	
	std::vector<Objeto_editor *> cursor;

	switch(tobjeto)
	{
		case tobjetocreado::obstaculo: localizar_elementos_bajo_cursor_helper(obstaculos, cursor, pt_raton); break;
		case tobjetocreado::decoracion: localizar_elementos_bajo_cursor_helper(decoraciones, cursor, pt_raton); break;
		case tobjetocreado::punto_ruta: localizar_elementos_bajo_cursor_helper(puntos_ruta, cursor, pt_raton); break;
		case tobjetocreado::inicio: localizar_elementos_bajo_cursor_helper(puntos_inicio, cursor, pt_raton); break;
		case tobjetocreado::bot: localizar_elementos_bajo_cursor_helper(puntos_bot, cursor, pt_raton); break;
		case tobjetocreado::arma: localizar_elementos_bajo_cursor_helper(generadores_items, cursor, pt_raton); break;
	}
	
	for(auto& o : cursor) o->borrar();

	//Esto los borra... Pasamos por todos los tipos ya que podríamos tener más cosas seleccionadas.
	eliminar_helper(obstaculos);
	eliminar_helper(decoraciones);
	eliminar_helper(puntos_ruta);
	eliminar_helper(puntos_inicio);
	eliminar_helper(puntos_bot);
	eliminar_helper(generadores_items);
}

void Controlador_editor::cambiar_grid(int dir)
{
	if(dir < 0) grid-=10.0;
	else grid+=10;

	if(grid < 10.0) grid=10.0;
	else if(grid > 50.0) grid=50.0;
}

//Mejor "pegar propiedades"?
void Controlador_editor::pegar()
{
	switch(tobjeto)
	{
		case tobjetocreado::obstaculo: 
		{
			std::vector<Obstaculo_editor *> v;
			localizar_elementos_bajo_cursor_helper(obstaculos, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size())
			{
				mensajes.insertar_mensaje("Propiedades pegadas", 2.0f);
				for(auto& e : v)
				{
					auto& item=e->elemento;
					item.mut_tipo(info_obstaculo.tipo);
				}
			}
		}
		break;
		case tobjetocreado::decoracion:
		{
			std::vector<Decoracion_editor *> v;
			localizar_elementos_bajo_cursor_helper(decoraciones, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size())
			{
				mensajes.insertar_mensaje("Propiedades copiadas", 2.0f);
				for(auto &e : v)
				{
					auto& item=e->elemento;
					item.mut_color(info_decoracion.relleno);
					item.mut_color_linea(info_decoracion.linea);
					item.mut_profundidad(info_decoracion.orden);
					item.mut_frente(info_decoracion.frente);
				}
			}
		}
		break;
		case tobjetocreado::punto_ruta:
		case tobjetocreado::inicio:
		case tobjetocreado::bot:
		case tobjetocreado::arma: break;
	}
}

void Controlador_editor::copiar()
{
	switch(tobjeto)
	{
		case tobjetocreado::obstaculo: 
		{
			std::vector<Obstaculo_editor *> v;
			localizar_elementos_bajo_cursor_helper(obstaculos, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size()==1)
			{
				mensajes.insertar_mensaje("Propiedades copiadas", 2.0f);
				const auto& item=v[0]->elemento;
				info_obstaculo={item.acc_tipo()};
			}
		}
		break;
		case tobjetocreado::decoracion:
		{
			std::vector<Decoracion_editor *> v;
			localizar_elementos_bajo_cursor_helper(decoraciones, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));
			if(v.size()==1)
			{
				mensajes.insertar_mensaje("Propiedades copiadas", 2.0f);
				const auto& item=v[0]->elemento;
				info_decoracion={item.acc_color(), item.acc_color_linea(), item.acc_profundidad(), item.es_frente()};
			}
		}
		break;
		case tobjetocreado::punto_ruta:
		case tobjetocreado::inicio:
		case tobjetocreado::bot:
		case tobjetocreado::arma: break;
	}
}

void Controlador_editor::intercambiar_visibilidad(int val, const std::string& tipo)
{
	if(ver_flags & val) 
	{
		ver_flags-=val;
		mensajes.insertar_mensaje("Ocultando "+tipo, 2.0f);
	}
	else 
	{
		ver_flags+=val;
		mensajes.insertar_mensaje("Mostrando "+tipo, 2.0f);
	}
}

void Controlador_editor::cambiar_profundidad(int dir)
{
	std::vector<Decoracion_editor *> v;
	localizar_elementos_bajo_cursor_helper(decoraciones, v, punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false));

	if(v.size()==1)
	{
		if(dir > 0) v[0]->elemento.subir_profundidad();
		else v[0]->elemento.bajar_profundidad();

		reordenar_decoraciones();
		mensajes.insertar_mensaje("Nueva profundidad de "+std::to_string(v[0]->elemento.acc_profundidad()), 2.0f);
	}
}

void Controlador_editor::reordenar_decoraciones()
{
	std::sort(
		std::begin(decoraciones), 
		std::end(decoraciones), 
		[](const Decoracion_editor& a, const Decoracion_editor& b) {return a.elemento < b.elemento;});
}
