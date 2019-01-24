#ifndef REPRESENTADOR_H
#define REPRESENTADOR_H

#include <video/screen/screen.h>
#include <video/camera/camera.h>
#include <video/representation/primitive/polygon/polygon_representation.h>
#include <video/representation/primitive/line/line_representation.h>
#include <tools/polygon_2d/polygon_2d.h>
#include "color.h"


#include "espaciable.h" //Para el cuadrado.

namespace App
{
class Representador
{
	public:

	void 			dibujar_poligono(ldv::screen&, const ldt::polygon_2d<double>&, tcolor, const ldv::camera&);
	void 			dibujar_poligono_lineas(ldv::screen&, const ldt::polygon_2d<double>&, tcolor, const ldv::camera&);
	void 			dibujar_segmento(ldv::screen&, const ldt::segment_2d<double>&, tcolor, const ldv::camera&);



	void 			dibujar_poligono(ldv::screen&, const ldt::polygon_2d<double>&, tcolor);
	void 			dibujar_poligono_lineas(ldv::screen&, const ldt::polygon_2d<double>&, tcolor);
	void 			dibujar_segmento(ldv::screen&, const ldt::segment_2d<double>&, tcolor);

	void 			dibujar_poligono_sin_transformar(ldv::screen&, const ldt::polygon_2d<double>&, tcolor);

	void 			dibujar_rejilla(ldv::screen&, int grid, tcolor, double nx, double ny, double zoom);

	private:

	ldt::point_2d<int>	cartesiano_a_sdl(const ldt::point_2d<double>& pt);
};

}

#endif
