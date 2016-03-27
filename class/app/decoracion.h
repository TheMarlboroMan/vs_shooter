#ifndef DECORACION_H
#define DECORACION_H

#include "color.h"
#include "obstaculo.h"
#include "representable.h"

namespace App
{

class Decoracion:
	public Obstaculo,
	public Representable
{
	public:

					Decoracion(const tpoligono&, tcolor, tcolor, bool);
	tcolor				acc_color() const {return color;}
	tcolor				acc_color_linea() const {return color_linea;}
	bool				es_frente() const {return frente;}

	void				mut_color(tcolor c) {color=c;}
	void				mut_color_linea(tcolor c) {color_linea=c;}
	void				mut_frente(bool v) {frente=v;}

	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const Struct_camara&) const;

	private:

	tcolor				color,
					color_linea;
	bool				frente;
};

}

#endif
