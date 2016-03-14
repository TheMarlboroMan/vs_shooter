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

				Proyectil(double, double, double, int, tcolor);

	bool			es_activo() const {return tiempo > 0.0f;}
	int			acc_potencia() const {return potencia;}
	tcolor			acc_color() const {return color;}

	void			desactivar() {tiempo=0.0f;}
	void			turno(float);	

	private:

	void			formar_poligono();

	double 			angulo;
	float			velocidad,
				tiempo;
	int 			potencia,
				indice_jugador;
	tcolor			color;

};

}

#endif
