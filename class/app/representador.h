#ifndef REPRESENTADOR_H
#define REPRESENTADOR_H

#include <video/pantalla/pantalla.h>
#include <video/representacion/representacion_primitiva/representacion_primitiva_poligono/representacion_primitiva_poligono.h>
#include <video/representacion/representacion_primitiva/representacion_primitiva_linea/representacion_primitiva_linea.h>
#include <herramientas/poligono_2d/poligono_2d.h>
#include "color.h"


#include "espaciable.h" //Para el cuadrado.

namespace App
{
class Representador
{
	public:

	void 			dibujar_poligono(DLibV::Pantalla&, const DLibH::Poligono_2d<double>&, tcolor, double nx, double ny, double zoom, bool relleno=true);
	void 			dibujar_segmento(DLibV::Pantalla&, const DLibH::Segmento_2d<double>&, tcolor, double nx, double ny, double zoom);
	void 			dibujar_rejilla(DLibV::Pantalla&, int grid, tcolor, double nx, double ny, double zoom);

	private:

	DLibH::Punto_2d<double> transformar(const DLibH::Punto_2d<double>& pt, double nx, double ny, double zoom);
};

}

#endif
