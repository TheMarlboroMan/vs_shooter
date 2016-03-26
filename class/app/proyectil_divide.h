#ifndef PROYECTIL_DIVIDE_H
#define PROYECTIL_DIVIDE_H

#include "proyectil.h"

namespace App
{
class Proyectil_divide:
	public Proyectil
{
	public:

				Proyectil_divide(int);

	virtual void		extinguir(std::vector<Disparador>&);
	virtual void		colisionar(std::vector<Disparador>&);
	virtual int		acc_potencia() const {return 20;}
	virtual tcolor		acc_color() const {return color;}
	virtual void		turno(float);
	virtual void		formar_poligono();
	virtual void		preparar(double, DLibH::Punto_2d<double>);

	private:

	void			generar(std::vector<Disparador>&, double);

	tcolor			color;
};
}

#endif
