#include "representador.h"

using namespace App;

void Representador::dibujar_poligono(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color, const DLibV::Camara& camara)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;

	for(const auto& v : poligono.acc_vertices())
		puntos.push_back(cartesiano_a_sdl(v));

	DLibV::Representacion_primitiva_poligono poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla, camara);
}

void Representador::dibujar_poligono_lineas(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color, const DLibV::Camara& camara)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;

	for(const auto& v : poligono.acc_vertices())
		puntos.push_back(cartesiano_a_sdl(v));

	DLibV::Representacion_primitiva_poligono_lineas poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla, camara);
}

void Representador::dibujar_segmento(DLibV::Pantalla& pantalla, const DLibH::Segmento_2d<double>& seg, tcolor color, const DLibV::Camara& camara)
{
	const auto v1=cartesiano_a_sdl(seg.v1);
	const auto v2=cartesiano_a_sdl(seg.v2);

	DLibV::Representacion_primitiva_linea lin(v1.x, v1.y, v2.x, v2.y, color.r, color.g, color.b);
	lin.establecer_alpha(color.a);
	lin.volcar(pantalla, camara);
}

void Representador::dibujar_poligono(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;
	for(const auto& v : poligono.acc_vertices()) puntos.push_back(cartesiano_a_sdl(v));

	DLibV::Representacion_primitiva_poligono poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla);
}

void Representador::dibujar_poligono_lineas(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;
	for(const auto& v : poligono.acc_vertices()) puntos.push_back(cartesiano_a_sdl(v));

	DLibV::Representacion_primitiva_poligono_lineas poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla);
}

void Representador::dibujar_segmento(DLibV::Pantalla& pantalla, const DLibH::Segmento_2d<double>& seg, tcolor color)
{
	const auto v1=cartesiano_a_sdl(seg.v1);
	const auto v2=cartesiano_a_sdl(seg.v2);

	DLibV::Representacion_primitiva_linea lin(v1.x, v1.y, v2.x, v2.y, color.r, color.g, color.b);
	lin.establecer_alpha(color.a);
	lin.volcar(pantalla);
}

void Representador::dibujar_rejilla(DLibV::Pantalla& pantalla, int grid, tcolor color, double nx, double ny, double zoom)
{
	DLibV::Representacion_primitiva_linea lin(0, 0, 0, 0, color.r, color.g, color.b);
	lin.establecer_alpha(color.a);
	//TODO: Not really 640 x 400.
	double 	inix=-(fmod(nx, grid)) / zoom, 
		finx=inix + 640;

	while(inix < finx)
	{
		lin.establecer_puntos(inix, 0, inix, 400);
		lin.volcar(pantalla);
		inix+=(grid / zoom);
	}

	double 	iniy=(fmod(ny, grid)) / zoom,
		finy=iniy + 400;

	while(iniy < finy)
	{
		lin.establecer_puntos(0, iniy, 640, iniy);
		lin.volcar(pantalla);
		iniy+=(grid / zoom);
	}
}

void Representador::dibujar_poligono_sin_transformar(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;
	for(const auto& v : poligono.acc_vertices()) puntos.push_back({(int)v.x, (int)v.y});

	DLibV::Representacion_primitiva_poligono poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla);
}


DLibV::Representacion_primitiva_poligono_base::punto Representador::cartesiano_a_sdl(const DLibH::Punto_2d<double>& pt)
{
	return DLibV::Representacion_primitiva_poligono_base::punto{(int)floor(pt.x), (int)floor(-pt.y)};
}

