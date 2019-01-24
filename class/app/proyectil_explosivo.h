#ifndef PROYECTIL_EXPLOSIVO_H
#define PROYECTIL_EXPLOSIVO_H

#include "proyectil.h"

namespace App
{
class Proyectil_explosivo:
	public Proyectil
{
	public:

				Proyectil_explosivo(int);

	virtual void		extinguir(std::vector<Disparador>&);
	virtual void		colisionar(std::vector<Disparador>&);
	virtual int		acc_potencia() const {return 50;}
	virtual tcolor		acc_color() const {return color;}
	virtual void		turno(float);
	virtual void		formar_poligono();
	virtual void		preparar(double, ldt::point_2d<double>);

	private:

	tcolor			color;
	float			rot;
};
}

#endif
