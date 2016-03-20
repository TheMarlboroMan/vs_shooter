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
	if(input.es_input_down(Input::tab)) intercambiar_objeto_creado();

	if(input.es_input_down(Input::cargar_mapa)) cargar_mapa();	
	else if(input.es_input_down(Input::grabar_mapa)) grabar_mapa();	

	if(input.es_input_down(Input::click_i)) click_izq();
	else if(input.es_input_down(Input::click_d)) click_der();

	if(input.es_input_down(Input::TEST_VISIBILIDAD))
	{
		mapa.construir_nodos_ruta();
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
	{
		r.dibujar_poligono(pantalla, o.acc_poligono(), o.acc_color(), struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
	}

	for(const auto& s : poligono_construccion.acc_segmentos())
	{
		r.dibujar_segmento(pantalla, s, {0, 255, 0, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);		
	}
	
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
	{
		r.dibujar_poligono(pantalla, g.acc_poligono(), {0, 0, 255, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
	}	

	for(const auto& p : mapa.puntos_ruta)
	{
		Espaciable::tpoligono poli=cuadrado(p.pt.x, p.pt.y, 5);
		r.dibujar_poligono(pantalla, poli, {255, 0, 0, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
	}

	//Nodos ruta...
	for(const auto& nr : mapa.acc_nodos_ruta())
	{
		for(const auto& c : nr.conexiones)
		{
			Segmento_2d<double> s{ {nr.origen.pt.x, nr.origen.pt.y}, {c.destino.pt.x, c.destino.pt.y}};
			r.dibujar_segmento(pantalla, s, {255, 255, 25, 255}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
		}		
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
	Espaciable::tpoligono poli=cuadrado(pt_raton.x, pt_raton.y, 5);
	r.dibujar_poligono(pantalla, poli, {255, 255, 255, 128}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom);
}

void Logica_editor::cargar_mapa()
{
	mapa.limpiar();


	Importador importador;
	importador.importar("mapa.dat", mapa.obstaculos, mapa.puntos_inicio, mapa.generadores_items);
}

void Logica_editor::grabar_mapa()
{
	std::ofstream fichero("mapa.dat");
	Exportador exportador;
	fichero<<exportador.serializar(mapa.obstaculos, mapa.puntos_inicio, mapa.generadores_items);
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

DLibH::Punto_2d<double>	Logica_editor::punto_desde_pos_pantalla(int x, int y)
{
	int px=struct_camara.xcam+(x/struct_camara.zoom);
	int py=struct_camara.ycam-(y/struct_camara.zoom);

	px=round(px / grid) * grid;
	py=round(py / grid) * grid;

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

