#ifndef ESPACIABLE_H
#define ESPACIABLE_H

#include <tools/polygon_2d/polygon_2d.h>

/*
Define una interface común para todas las cosas colisionables que consten de
un único polígono (como va a ser el caso en toda la aplicación).
*/

namespace App
{

class Espaciable
{
	public:

	//TODO: Cambiar instancias de punto_2d por eso.
	typedef ldt::point_2d<double>		tpunto;
	typedef ldt::polygon_2d<double>	tpoligono;

						Espaciable();
						Espaciable(const tpoligono&);

	const tpoligono&			acc_poligono() const {return poligono;}
	void					establecer_posicion(double, double);
	void					mover(double, double);
	bool					en_colision_con(const Espaciable& e) const;
	void					desplazar_angulo_velocidad(double, double);

	protected:

	tpoligono				poligono;


};

}

#endif
