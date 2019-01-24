#ifndef REPRESENTADOR_H
#define REPRESENTADOR_H

#include <video/screen/screen.h>
#include <video/camera/camera.h>
#include <video/representation/primitive/polygon/polygon_representation.h>
#include <video/representation/primitive/line/line_representation.h>
#include <tools/2d_polygon/2d_polygon.h>
#include "color.h"


#include "espaciable.h" //Para el cuadrado.

namespace App
{
class Representador
{
	public:

	void 			dibujar_poligono(ldv::screen&, const ldt::2d_polygon<double>&, tcolor, const ldv::camera&);
	void 			dibujar_poligono_lineas(ldv::screen&, const ldt::2d_polygon<double>&, tcolor, const ldv::camera&);
	void 			dibujar_segmento(ldv::screen&, const DLibH::Segmento_2d<double>&, tcolor, const ldv::camera&);



	void 			dibujar_poligono(ldv::screen&, const ldt::2d_polygon<double>&, tcolor);
	void 			dibujar_poligono_lineas(ldv::screen&, const ldt::2d_polygon<double>&, tcolor);
	void 			dibujar_segmento(ldv::screen&, const DLibH::Segmento_2d<double>&, tcolor);

	void 			dibujar_poligono_sin_transformar(ldv::screen&, const ldt::2d_polygon<double>&, tcolor);

	void 			dibujar_rejilla(ldv::screen&, int grid, tcolor, double nx, double ny, double zoom);

	private:

	DLibV::Representacion_primitiva_poligono_base::punto	cartesiano_a_sdl(const DLibH::Punto_2d<double>& pt);
};

}

#endif
