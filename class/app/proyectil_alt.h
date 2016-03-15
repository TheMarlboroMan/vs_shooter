#ifndef PROYECTIL_ALT_H
#define PROYECTIL_ALT_H

#include "proyectil.h"

namespace App
{
class Proyectil_alt:
	public Proyectil
{
	public:

				Proyectil_alt(int);

	virtual int		acc_potencia() const {return 10;}
	virtual tcolor		acc_color() const {return color;}
	virtual void		turno(float);
	virtual void		formar_poligono();
	virtual void		preparar(double, DLibH::Punto_2d<double>);

	private:

	tcolor			color;
};
}

#endif
