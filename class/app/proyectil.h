#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "color.h"
#include "espaciable.h"

namespace App
{

class Proyectil:
	public Espaciable
{
	public:

				Proyectil(int, double, double);

	bool			es_activo() const {return tiempo > 0.0f;}
	void			desactivar() {tiempo=0.0f;}

	virtual int		acc_potencia() const=0;
	virtual tcolor		acc_color() const=0;
	virtual void		turno(float)=0;
	virtual void		formar_poligono()=0;
	virtual void		preparar(double, DLibH::Punto_2d<double>)=0;

	protected:

	double 			angulo;
	float			velocidad,
				tiempo;
	int 			indice_jugador;
};

}

#endif
