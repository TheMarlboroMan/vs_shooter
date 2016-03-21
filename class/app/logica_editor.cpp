#include "logica_editor.h"

#include <fstream>
#include <algorithm>
#include <string>

#include "exportador.h"
#include "importador.h"
#include "representador.h"

#include "framework_impl/input.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Logica_editor::Logica_editor(Mapa& m)
	:mapa(m),
	fuente_akashi("data/fuentes/Akashi.ttf", 16),
	struct_camara({1.0, 0, 0}), grid(20), 
	tobjeto(tobjetocreado::vertice)
{

}

void Logica_editor::iniciar()
{

}

void Logica_editor::finalizar()
{
	tobjeto=tobjetocreado::vertice;
	poligono_construccion=Poligono_2d<double>{};
}

void Logica_editor::loop(DFramework::Input& input, float delta)
{
	pos_raton=input.acc_posicion_raton();

	localizar_elementos_bajo_cursor();

	if(input.es_input_down(Input::tab)) intercambiar_objeto_creado();
	if(input.es_input_down(Input::cargar_mapa)) cargar_mapa();	
	else if(input.es_input_down(Input::grabar_mapa)) grabar_mapa();	

	if(input.es_input_down(Input::click_i)) click_izq();
	else if(input.es_input_down(Input::click_d)) click_der();

	//TODO: Constantes...
	if(input.es_input_pulsado(Input::cursor_arriba)) struct_camara.ycam+=100.0 * (double) delta;
	else if(input.es_input_pulsado(Input::cursor_abajo)) struct_camara.ycam-=100.0 * (double) delta;

	if(input.es_input_pulsado(Input::cursor_derecha)) struct_camara.xcam+=100.0 * (double) delta;
	else if(input.es_input_pulsado(Input::cursor_izquierda)) struct_camara.xcam-=100.0 * (double) delta;

	if(input.es_input_down(Input::TEST_VISIBILIDAD))
	{
		mapa.construir_nodos_ruta();
	}

	if(input.es_input_down(Input::TEST_RUTA))
	{
		do_crazy_pathfind();
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
	auto cuadrado=[](double x, double y, int rad)
	{
		return Espaciable::tpoligono { {{x-rad, y-rad}, {x-rad, y+rad}, {x+rad, y+rad}, {x+rad, y-rad}}, {x, y}};
	};

	pantalla.limpiar(0, 0, 0, 255);

	Representador r;
	r.dibujar_rejilla(pantalla, grid, {255, 255, 255, 64}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);

	auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y);

	for(const auto& o : mapa.obstaculos)
		r.dibujar_poligono(pantalla, o.acc_poligono(), o.acc_color(), struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);

	//Obstáculos bajo el cursor...
	for(const auto& o : obstaculos_cursor)
		r.dibujar_poligono(pantalla, o->acc_poligono(), {255, 255, 64, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom, false);

	for(const auto& s : poligono_construccion.acc_segmentos())
		r.dibujar_segmento(pantalla, s, {0, 255, 0, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);		
	
	//Segmento en construcción...	
	if(poligono_construccion.acc_vertices().size())
	{
		const auto& v=poligono_construccion.acc_vertices().back();
		Segmento_2d<double> s{ {v.x, v.y}, {pt_raton.x, pt_raton.y}};
		r.dibujar_segmento(pantalla, s, {0, 255, 0, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);		
	}

	for(const auto& p : mapa.puntos_inicio)
	{
		Espaciable::tpoligono poli={ {{p.x, p.y}, {p.x+10, p.y}, {p.x+10, p.y+10}, {p.x, p.y+10}}, {p.x, p.y}};
		r.dibujar_poligono(pantalla, poli, {255, 0, 0, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
	}

	for(const auto& g : mapa.generadores_items)
		r.dibujar_poligono(pantalla, g.acc_poligono(), {0, 0, 255, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);

	for(const auto& p : mapa.puntos_ruta)
	{
		Espaciable::tpoligono poli=cuadrado(p.pt.x, p.pt.y, 5);
		r.dibujar_poligono(pantalla, poli, {255, 255, 25, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
	}

	//Nodos ruta...
	//TODO: Poder desactivar esto...
	for(const auto& nr : mapa.acc_nodos_ruta())
	{
		for(const auto& c : nr.conexiones)
		{
			Segmento_2d<double> s{ {nr.origen.pt.x, nr.origen.pt.y}, {c.destino.origen.pt.x, c.destino.origen.pt.y}};
			r.dibujar_segmento(pantalla, s, {255, 255, 25, 64}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
		}		
	}

	//TODO: Esto es temporal...
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

	//TODO: Mostrar el objeto actual seleccionado...

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
	Espaciable::tpoligono poli=cuadrado(pt_raton.x, pt_raton.y, 5);
	r.dibujar_poligono(pantalla, poli, {255, 255, 255, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
}

void Logica_editor::cargar_mapa()
{
	mapa.limpiar();


	Importador importador;
	importador.importar("mapa.dat", mapa.obstaculos, mapa.puntos_inicio, mapa.puntos_ruta, mapa.generadores_items);
}

void Logica_editor::grabar_mapa()
{
	std::ofstream fichero("mapa.dat");
	Exportador exportador;
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

void Logica_editor::click_izq()
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
			crear_punto_generador_items(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		break;
	}
}

void Logica_editor::click_der()
{	
	switch(tobjeto)
	{
		case tobjetocreado::vertice:
			if(poligono_construccion.acc_vertices().size())
				cerrar_poligono();
		break;
		case tobjetocreado::inicio:
		case tobjetocreado::arma:
		case tobjetocreado::punto_ruta:
			
		break;
	}
}

void Logica_editor::crear_punto_inicio(DLibH::Punto_2d<double> pt)
{
	mapa.puntos_inicio.push_back(pt);
}

void Logica_editor::crear_punto_ruta(DLibH::Punto_2d<double> pt)
{
	mapa.puntos_ruta.push_back(Punto_ruta{pt});
}

void Logica_editor::crear_punto_generador_items(DLibH::Punto_2d<double> pt)
{
	Generador_items gi(pt);
	gi.reiniciar();
	mapa.generadores_items.push_back(gi);
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
	poligono_construccion.insertar_vertice(p);
}

void Logica_editor::cerrar_poligono()
{
	if(poligono_construccion.size() == 3)
	{
		poligono_construccion.cerrar();
		mapa.obstaculos.push_back({poligono_construccion, {64, 64, 64, 255}});
	}
	else if(poligono_construccion.size() > 3 && !poligono_construccion.es_concavo())
	{
		poligono_construccion.cerrar();
		mapa.obstaculos.push_back({poligono_construccion, {64, 64, 64, 255}});
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

	auto localizar=[this](Espaciable::tpunto pt, const std::vector<Nodo_ruta>& nr)
	{
		const Nodo_ruta * res=nullptr;
		double dist=pt.distancia_hasta(nr.front().origen.pt);
		for(const auto& n : nr)
		{
			double d=pt.distancia_hasta(n.origen.pt);

			if(d <= dist && mapa.visibilidad_entre_puntos(pt, n.origen.pt))
			{
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
		std::cout<<"NO SE HA LOCALIZADO NODO VISIBLE PARA FIN"<<std::endl;
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
		std::cout<<"DISTANCIA DE "<<res.distancia<<std::endl;

		ruta.push_back(mapa.puntos_inicio.front());
		for(const auto& pt: res.ruta)
		{
			ruta.push_back({pt.x, pt.y});
			std::cout<<pt.x<<","<<pt.y<<std::endl;
		}
		ruta.push_back(mapa.puntos_inicio.back());
	}
}

void Logica_editor::localizar_elementos_bajo_cursor()
{
	auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y, false);

	switch(tobjeto)
	{
		case tobjetocreado::vertice:
			obstaculos_cursor.clear();
			for(const auto& o : mapa.obstaculos)
			{
				if(punto_en_poligono(o.acc_poligono(), pt_raton))
				{
					obstaculos_cursor.push_back(&o);
				}
			}

		break;
		case tobjetocreado::inicio:
		case tobjetocreado::arma:
		case tobjetocreado::punto_ruta:
			
		break;
	}
	
}
