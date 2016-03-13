#ifndef REPRESENTADOR_H
#define REPRESENTADOR_H

#include <video/pantalla/pantalla.h>
#include <video/representacion/representacion_primitiva/representacion_primitiva_poligono/representacion_primitiva_poligono.h>
#include <video/representacion/representacion_primitiva/representacion_primitiva_linea/representacion_primitiva_linea.h>
#include <herramientas/poligono_2d/poligono_2d.h>
#include "color.h"

namespace App
{
class Representador
{
	public:

	void dibujar_poligono(DLibV::Pantalla&, const DLibH::Poligono_2d<double>&, tcolor, int nx, int ny, double zoom);
	void dibujar_segmento(DLibV::Pantalla&, const DLibH::Segmento_2d<double>&, tcolor, int nx, int ny, double zoom);
	void dibujar_rejilla(DLibV::Pantalla&, int grid, tcolor, int nx, int ny, double zoom);

	private:

	DLibH::Punto_2d<double> transformar(const DLibH::Punto_2d<double>& pt, int nx, int ny, double zoom);
};

}

#endif
