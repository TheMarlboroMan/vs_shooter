#ifndef JUGADOR_ARMA_DIVIDE_H
#define JUGADOR_ARMA_DIVIDE_H

#include "jugador_arma.h"

namespace App
{
class Jugador_arma_divide:
	public Jugador_arma
{
	public:

						Jugador_arma_divide();
	virtual bool				es_arma_defecto() const {return false;}
	virtual void				generar_proyectiles(Disparador::v_info&, int, ldt::point_2d<double>, double, double);
	virtual void				disparar();
};
}

#endif
