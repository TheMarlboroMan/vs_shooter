#include "logica_editor.h"

#include <fstream>
#include <string>

#include "exportador.h"
#include "importador.h"

#include "framework_impl/input.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Logica_editor::Logica_editor(Mapa& m)
	:mapa(m),
	fuente_akashi("data/fuentes/Akashi.ttf", 16),
	struct_camara({1.0, 0, 0}), grid(20), ver_conexiones(true),
	tobjeto(tobjetocreado::vertice)
{

}

void Logica_editor::iniciar()
{
	//No es necesario hacer nada...
}

void Logica_editor::finalizar()
{
	tobjeto=tobjetocreado::vertice;
	poligono_construccion=Poligono_2d<double>{};

	//Aplicar al mapa...
	aplicar_a_mapa();
}

void Logica_editor::loop(DFramework::Input& input, float delta)
{
	pos_raton=input.acc_posicion_raton();

	localizar_elementos_bajo_cursor();

	if(input.es_input_down(Input::tab)) intercambiar_objeto_creado();
	if(input.es_input_down(Input::cargar_mapa)) cargar_mapa();	
	else if(input.es_input_down(Input::grabar_mapa)) grabar_mapa();

	if(input.es_input_down(Input::intercambiar_ver_conexiones)) ver_conexiones=!ver_conexiones;	

	if(input.es_input_down(Input::click_i)) 
	{
		if(input.es_input_pulsado(Input::control_izquierdo)) seleccionar();
		else crear();
	}	
	else if(input.es_input_down(Input::click_d)) 
	{
		//Nothing yet...
	}
	
	if(input.es_input_pulsado(Input::suprimir)) eliminar();

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
		if(input.es_input_pulsado(Input::cursor_arriba)) struct_camara.ycam+=100.0 * (double) delta;
		else if(input.es_input_pulsado(Input::cursor_abajo)) struct_camara.ycam-=100.0 * (double) delta;

		if(input.es_input_pulsado(Input::cursor_derecha)) struct_camara.xcam+=100.0 * (double) delta;
		else if(input.es_input_pulsado(Input::cursor_izquierda)) struct_camara.xcam-=100.0 * (double) delta;
	}

	if(input.es_input_down(Input::TEST_VISIBILIDAD))
	{
		aplicar_a_mapa();
		mapa.construir_nodos_ruta();
	}

	if(input.es_input_down(Input::TEST_RUTA))
	{
		if(!ruta.size()) do_crazy_pathfind();
		else ruta.clear();
	}

	if(input.es_input_pulsado(Input::zoom_mas)) 
	{
		struct_camara.zoom+=(double)delta;
	}
	else if(input.es_input_pulsado(Input::zoom_menos)) 
	{
		struct_camara.zoom-=(double)delta;
		if(struct_camara.zoom < 0.10) struct_camara.zoom=0.10;
	}
}

void Logica_editor::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);

	Representador r;
	r.dibujar_rejilla(pantalla, grid, {255, 255, 255, 64}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);

	auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y);

	for(const auto& eo : obstaculos) eo.dibujar(r, pantalla, struct_camara);
	for(const auto& pi : puntos_inicio) pi.dibujar(r, pantalla, struct_camara);
	for(const auto& eg : generadores_items) eg.dibujar(r, pantalla, struct_camara);
	for(const auto& op : puntos_ruta) op.dibujar(r, pantalla, struct_camara);
	for(const auto& oc : objetos_cursor) oc->dibujar(r, pantalla, struct_camara, false);
	for(const auto& os : objetos_seleccionados) os->dibujar(r, pantalla, struct_camara, false);

	if(ver_conexiones)
	{
		for(const auto& nr : mapa.acc_nodos_ruta())
		{
			for(const auto& c : nr.conexiones)
			{
				Segmento_2d<double> s{ {nr.origen.pt.x, nr.origen.pt.y}, {c.destino.origen.pt.x, c.destino.origen.pt.y}};
				r.dibujar_segmento(pantalla, s, {255, 255, 25, 64}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
			}		
		}
	}

	//TODO: Temporal: mostrar ruta.
	if(ruta.size())
	{
		size_t i=1, max=ruta.size();

		while(i < max)
		{
			auto pt1=ruta[i-1], pt2=ruta[i];
			Segmento_2d<double> s{ {pt1.x, pt1.y}, {pt2.x, pt2.y}};
			r.dibujar_segmento(pantalla, s, {255, 25, 25, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
			++i;
		}
	}

	for(const auto& s : poligono_construccion.acc_segmentos())
		r.dibujar_segmento(pantalla, s, {0, 255, 0, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);		
	
	//Segmento en construcción...	
	if(poligono_construccion.acc_vertices().size())
	{
		const auto& v=poligono_construccion.acc_vertices().back();
		Segmento_2d<double> s{ {v.x, v.y}, {pt_raton.x, pt_raton.y}};
		r.dibujar_segmento(pantalla, s, {0, 255, 0, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);		
	}

	std::string texto=std::to_string((int)pt_raton.x)+","+std::to_string((int)pt_raton.y);
	switch(tobjeto)
	{
		case tobjetocreado::vertice: texto+=" [vertex]"; break;
		case tobjetocreado::punto_ruta: texto+=" [waypoint]"; break;
		case tobjetocreado::inicio: texto+=" [spawn]"; break;
		case tobjetocreado::arma: texto+=" [weapon]"; break;
	}

	DLibV::Representacion_TTF txt(fuente_akashi, {255, 255, 255, 255}, texto);
	txt.ir_a(16, 16);
	txt.volcar(pantalla);

	//Punto actual...
	Espaciable::tpoligono poli=Objeto_editor::cuadrado(pt_raton.x, pt_raton.y, 3);
	r.dibujar_poligono(pantalla, poli, {255, 255, 255, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
}

void Logica_editor::cargar_mapa()
{
	mapa.limpiar();
	Importador importador;
	importador.importar("mapa.dat", mapa.obstaculos, mapa.puntos_inicio, mapa.puntos_ruta, mapa.generadores_items);
	obtener_desde_mapa();
}

void Logica_editor::grabar_mapa()
{
	std::ofstream fichero("mapa.dat");
	Exportador exportador;
	aplicar_a_mapa();
	fichero<<exportador.serializar(mapa.obstaculos, mapa.puntos_inicio, mapa.puntos_ruta, mapa.generadores_items);
}

void Logica_editor::intercambiar_objeto_creado()
{
	switch(tobjeto)
	{
		case tobjetocreado::vertice: tobjeto=tobjetocreado::punto_ruta; break;
		case tobjetocreado::punto_ruta: tobjeto=tobjetocreado::inicio; break;
		case tobjetocreado::inicio: tobjeto=tobjetocreado::arma; break;
		case tobjetocreado::arma: tobjeto=tobjetocreado::vertice; break;
	}
}

void Logica_editor::crear()
{
	switch(tobjeto)
	{
		case tobjetocreado::vertice:
			nuevo_punto(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::punto_ruta: 
			crear_punto_ruta(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::inicio:
			crear_punto_inicio(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
		case tobjetocreado::arma:
			crear_generador_items(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
	}
}

void Logica_editor::seleccionar()
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

void Logica_editor::crear_punto_inicio(DLibH::Punto_2d<double> pt)
{
	puntos_inicio.push_back(Punto_inicio_editor{pt});
}

void Logica_editor::crear_punto_ruta(DLibH::Punto_2d<double> pt)
{
	puntos_ruta.push_back(Punto_ruta_editor{pt});
}

void Logica_editor::crear_generador_items(DLibH::Punto_2d<double> pt)
{
	generadores_items.push_back(Generador_items_editor{pt});
}

DLibH::Punto_2d<double>	Logica_editor::punto_desde_pos_pantalla(int x, int y, bool a_rejilla)
{
	int px=struct_camara.xcam+(x/struct_camara.zoom);
	int py=struct_camara.ycam-(y/struct_camara.zoom);

	if(a_rejilla)
	{
		px=round(px / grid) * grid;
		py=round(py / grid) * grid;
	}

	return DLibH::Punto_2d<double>{(double)px, (double)py};
}

void Logica_editor::nuevo_punto(DLibH::Punto_2d<double> p)
{
	if(poligono_construccion.acc_vertices().size() > 2 && p==poligono_construccion.acc_vertices()[0])
	{
		cerrar_poligono();
	}
	else
	{
		poligono_construccion.insertar_vertice(p);
	}
}

void Logica_editor::cerrar_poligono()
{
	if(poligono_construccion.size() >= 3 && !poligono_construccion.es_concavo())
	{
		poligono_construccion.cerrar();
		obstaculos.push_back(Obstaculo_editor(Obstaculo(poligono_construccion, {64, 64, 64, 255})));
	}

	poligono_construccion=Poligono_2d<double>{};
}

void Logica_editor::do_crazy_pathfind()
{
	ruta.clear();

	if(mapa.puntos_inicio.size() < 2) 
	{
		std::cout<<"ES NECESARIO SETEAR PUNTOS DE INICIO"<<std::endl;
		return;
	}

	//TODO: Este algoritmo se puede mejorar añadiendo los dos nuevos puntos como
	//puntos de ruta temporales: volvemos a computar y luego los borramos. De esta
	//forma evitaríamos desde el punto de inicio al siguiente más cercano aunque luego
	//tengamos que dar la vuelta. Resolver el primer punto es sencillo: lo tratamos como
	//un punto de ruta normal sin que el resto se vinculen a él.
	//Resolver el último ya es más complicado sin reconstruir el mapa cada vez, puesto
	//que los nodos reales tienen que vincularse y al terminar habría que desvincularlos.
	
	auto localizar=[this](Espaciable::tpunto pt, const std::vector<Nodo_ruta>& nr)
	{
		const Nodo_ruta * res=nullptr;
		double dist=-1.0;
		for(const auto& n : nr)
		{
			double d=pt.distancia_hasta(n.origen.pt);

			if((dist < 0.0 || d <= dist) && mapa.visibilidad_entre_puntos(pt, n.origen.pt))
			{
				dist=d;
				res=&n;
			}
		}

		return res;
	};

	const auto& nr=mapa.acc_nodos_ruta();

	const Nodo_ruta * ini=localizar(mapa.puntos_inicio.front(), nr);
	const Nodo_ruta * fin=localizar(mapa.puntos_inicio.back(), nr);

	if(!ini)
	{
		std::cout<<"NO SE HA LOCALIZADO NODO VISIBLE PARA INICIO"<<std::endl;
		return;
	}

	if(!fin)
	{
		return;
	}

	Trazador_ruta t;
	auto res=t.trazar_ruta(*ini, *fin);

	if(!res.resuelto) 
	{
		std::cout<<"NO SE HA PODIDO RESOLVER"<<std::endl;
	}
	else
	{
		ruta.push_back(mapa.puntos_inicio.front());

		for(const auto& pt: res.ruta)
			ruta.push_back({pt.x, pt.y});

		ruta.push_back(mapa.puntos_inicio.back());
	}
}

void Logica_editor::localizar_elementos_bajo_cursor()
{
	auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false);
	objetos_cursor.clear();

	localizar_elementos_bajo_cursor_helper(obstaculos, pt_raton);
	localizar_elementos_bajo_cursor_helper(puntos_inicio, pt_raton);
	localizar_elementos_bajo_cursor_helper(generadores_items, pt_raton);
	localizar_elementos_bajo_cursor_helper(puntos_ruta, pt_raton);
}

void Logica_editor::mover_seleccion(double x, double y)
{
	for(auto& o : objetos_seleccionados) o->mover(x, y);
}

void Logica_editor::aplicar_a_mapa()
{
	mapa.limpiar();
	for(auto& o : obstaculos) mapa.obstaculos.push_back(o.elemento);
	for(auto& o : puntos_ruta) mapa.puntos_ruta.push_back(o.elemento);
	for(auto& o : puntos_inicio) mapa.puntos_inicio.push_back(o.elemento);
	for(auto& o : generadores_items) mapa.generadores_items.push_back(o.elemento);	
}

void Logica_editor::obtener_desde_mapa()
{
	for(auto& o : mapa.obstaculos) obstaculos.push_back(Obstaculo_editor(o));
	for(auto& o : mapa.puntos_ruta) puntos_ruta.push_back(Punto_ruta_editor(o));
	for(auto& o : mapa.puntos_inicio) puntos_inicio.push_back(Punto_inicio_editor(o));
	for(auto& o : mapa.generadores_items) generadores_items.push_back(Generador_items_editor(o));
}

void Logica_editor::eliminar()
{
	//Esto los marca para borrar...
	for(auto& o : objetos_seleccionados) o->borrar();
	for(auto& o : objetos_cursor) o->borrar();

	objetos_seleccionados.clear();
	objetos_cursor.clear();

	//Esto los borra...
	eliminar_helper(obstaculos);
	eliminar_helper(puntos_ruta);
	eliminar_helper(puntos_inicio);
	eliminar_helper(generadores_items);
}
