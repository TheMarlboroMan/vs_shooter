#include "representador.h"

using namespace App;

void Representador::dibujar_poligono(ldv::screen& pantalla, const ldt::polygon_2d<double>& poligono, tcolor color, const ldv::camera& camara)
{
	std::vector<ldv::point> puntos;

	for(const auto& v : poligono.get_vertices())
		puntos.push_back(cartesiano_a_sdl(v));

	ldv::polygon_representation poli(ldv::polygon_representation::type::fill, puntos, {color.r, color.g, color.b, color.a});
	poli.draw(pantalla, camara);
}

void Representador::dibujar_poligono_lineas(ldv::screen& pantalla, const ldt::polygon_2d<double>& poligono, tcolor color, const ldv::camera& camara)
{
	std::vector<ldv::point> puntos;

	for(const auto& v : poligono.get_vertices())
		puntos.push_back(cartesiano_a_sdl(v));

	ldv::polygon_representation poli(ldv::polygon_representation::type::line, puntos, {color.r, color.g, color.b, color.a});
	poli.draw(pantalla, camara);
}

void Representador::dibujar_segmento(ldv::screen& pantalla, const ldt::segment_2d<double>& seg, tcolor color, const ldv::camera& camara)
{
	const auto v1=cartesiano_a_sdl(seg.v1);
	const auto v2=cartesiano_a_sdl(seg.v2);

	ldv::line_representation lin({v1.x, v1.y}, {v2.x, v2.y}, {color.r, color.g, color.b, color.a});
	lin.draw(pantalla, camara);
}

void Representador::dibujar_poligono(ldv::screen& pantalla, const ldt::polygon_2d<double>& poligono, tcolor color)
{
	std::vector<ldv::point> puntos;
	for(const auto& v : poligono.get_vertices()) puntos.push_back(cartesiano_a_sdl(v));

	ldv::polygon_representation poli(ldv::polygon_representation::type::fill, puntos, {color.r, color.g, color.b, color.a});
	poli.draw(pantalla);
}

void Representador::dibujar_poligono_lineas(ldv::screen& pantalla, const ldt::polygon_2d<double>& poligono, tcolor color)
{
	std::vector<ldv::point> puntos;
	for(const auto& v : poligono.get_vertices()) puntos.push_back(cartesiano_a_sdl(v));

	ldv::polygon_representation poli(ldv::polygon_representation::type::line, puntos, {color.r, color.g, color.b, color.a});
	poli.draw(pantalla);
}

void Representador::dibujar_segmento(ldv::screen& pantalla, const ldt::segment_2d<double>& seg, tcolor color)
{
	const auto v1=cartesiano_a_sdl(seg.v1);
	const auto v2=cartesiano_a_sdl(seg.v2);

	ldv::line_representation lin({v1.x, v1.y}, {v2.x, v2.y}, {color.r, color.g, color.b, color.a});
	lin.draw(pantalla);
}

void Representador::dibujar_rejilla(ldv::screen& pantalla, int grid, tcolor color, double nx, double ny, double zoom)
{
	ldv::line_representation lin({0, 0}, {0, 0}, {color.r, color.g, color.b, color.a});
	//TODO: Not really 640 x 400.
	double 	inix=-(fmod(nx, grid)) / zoom,
		finx=inix + 640;

	while(inix < finx)
	{
		lin.set_points({inix, 0}, {inix, 400});
		lin.draw(pantalla);
		inix+=(grid / zoom);
	}

	double 	iniy=(fmod(ny, grid)) / zoom,
		finy=iniy + 400;

	while(iniy < finy)
	{
		lin.set_points({0, iniy}, {640, iniy});
		lin.draw(pantalla);
		iniy+=(grid / zoom);
	}
}

void Representador::dibujar_poligono_sin_transformar(ldv::screen& pantalla, const ldt::polygon_2d<double>& poligono, tcolor color)
{
	std::vector<ldv::point> puntos;
	for(const auto& v : poligono.get_vertices()) puntos.push_back({(int)v.x, (int)v.y});

	ldv::polygon_representation poli(ldv::polygon_representation::type::fill, puntos, {color.r, color.g, color.b, color.a});
	poli.draw(pantalla);
}


ldv::point Representador::cartesiano_a_sdl(const ldt::point_2d<double>& pt)
{
	return ldv::point{(int)floor(pt.x), (int)floor(-pt.y)};
}
