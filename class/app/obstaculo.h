#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "color.h"
#include "espaciable.h"
#include "representable.h"

namespace App
{

class Obstaculo:
	public Espaciable,
	public Representable
{
	public:

					Obstaculo(const tpoligono&, tcolor, tcolor);
	tcolor				acc_color() const {return color;}
	tcolor				acc_color_linea() const {return color_linea;}

	void				mut_color(tcolor c) {color=c;}
	void				mut_color_linea(tcolor c) {color_linea=c;}

	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const Struct_camara&) const;

	private:

	tcolor				color,
					color_linea;
};

}

#endif
