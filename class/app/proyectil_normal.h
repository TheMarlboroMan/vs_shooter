#ifndef PROYECTIL_NORMAL_H
#define PROYECTIL_NORMAL_H

#include "proyectil.h"

namespace App
{
class Proyectil_normal:
	public Proyectil
{
	public:

				Proyectil_normal(int);

	virtual int		acc_potencia() const {return 20;}
	virtual tcolor		acc_color() const {return color;}
	virtual void		turno(float);
	virtual void		formar_poligono();
	virtual void		preparar(double, DLibH::Punto_2d<double>);

	private:

	tcolor			color;
};
}

#endif
