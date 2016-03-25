#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "color.h"
#include "espaciable.h"
#include "disparador.h"
#include "representable.h"

namespace App
{

class Proyectil:
	public Espaciable,
	public Representable
{
	public:

				Proyectil(int, double, double);

	bool			es_activo() const {return tiempo > 0.0f;}
	int			acc_indice_jugador() const {return indice_jugador;}

	virtual void		extinguir(std::vector<Disparador>&)=0;
	virtual void		colisionar(std::vector<Disparador>&)=0;
	virtual int		acc_potencia() const=0;
	virtual tcolor		acc_color() const=0;
	virtual void		turno(float)=0;
	virtual void		formar_poligono()=0;
	virtual void		preparar(double, DLibH::Punto_2d<double>)=0;

	virtual void 		dibujar(Representador&, DLibV::Pantalla&, const Struct_camara&) const;

	protected:

	double 			angulo;
	float			velocidad,
				tiempo;
	int 			indice_jugador;
};

}

#endif
