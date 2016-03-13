#include "representador.h"

using namespace App;

void Representador::dibujar_poligono(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color, int nx, int ny, double zoom)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;

	for(const auto& v : poligono.acc_vertices())
	{
		const auto pt=transformar(v, nx, ny, zoom);
		puntos.push_back({(int)pt.x, (int)pt.y});
	}

	DLibV::Representacion_primitiva_poligono poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla);
}

void Representador::dibujar_segmento(DLibV::Pantalla& pantalla, const DLibH::Segmento_2d<double>& seg, tcolor color, int nx, int ny, double zoom)
{
	const auto p1=transformar(seg.v1, nx, ny, zoom);
	const auto p2=transformar(seg.v2, nx, ny, zoom);
	
	DLibV::Representacion_primitiva_linea lin(p1.x, p1.y, p2.x, p2.y, color.r, color.g, color.b);
	lin.establecer_alpha(color.a);
	lin.volcar(pantalla);
}

void Representador::dibujar_rejilla(DLibV::Pantalla& pantalla, int grid, tcolor color, int nx, int ny, double zoom)
{
	DLibV::Representacion_primitiva_linea lin(0, 0, 0, 0, color.r, color.g, color.b);
	lin.establecer_alpha(color.a);


	//TODO: Not really 640 x 400.
	double 	longx=640 / zoom,
		longy=400 / zoom;

	double 	inix=-(nx % grid) * zoom, 
		finx=inix + longx;

	while(inix < finx)
	{
		lin.establecer_puntos(inix, 0, inix, 400);
		lin.volcar(pantalla);		
		inix+=(grid * zoom);
	}

	double 	iniy=(ny % grid) * zoom,
		finy=iniy + longy;

	while(iniy < finy)
	{
		lin.establecer_puntos(0, iniy, 640, iniy);
		lin.volcar(pantalla);		
		iniy+=(grid * zoom);
	}
}

DLibH::Punto_2d<double> Representador::transformar(const DLibH::Punto_2d<double>& pt, int nx, int ny, double zoom)
{
	double x=(pt.x - nx) * zoom;
	double y=(-pt.y + ny) * zoom;
	return DLibH::Punto_2d<double>(x, y);
}
