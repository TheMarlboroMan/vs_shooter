#ifndef PROYECTIL_PEQ_H
#define PROYECTIL_PEQ_H

#include "proyectil.h"

namespace App
{
class Proyectil_peq:
	public Proyectil
{
	public:

				Proyectil_peq(int);

	virtual void		extinguir(std::vector<Disparador>&);
	virtual void		colisionar(std::vector<Disparador>&);
	virtual int		acc_potencia() const {return 10;}
	virtual tcolor		acc_color() const {return color;}
	virtual void		turno(float);
	virtual void		formar_poligono();
	virtual void		preparar(double, ldt::point_2d<double>);

	private:

	tcolor			color;
};
}

#endif
