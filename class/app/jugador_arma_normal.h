#ifndef JUGADOR_ARMA_NORMAL_H
#define JUGADOR_ARMA_NORMAL_H

#include "jugador_arma.h"

namespace App
{
class Jugador_arma_normal:
	public Jugador_arma
{
	public:

	virtual void				generar_proyectiles(Disparador::v_info&, int, DLibH::Punto_2d<double>, double, double);
	virtual void				disparar();
	virtual int				acc_municion_restante() const {return -1;}
};
}

#endif
