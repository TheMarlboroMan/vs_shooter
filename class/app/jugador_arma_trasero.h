#ifndef JUGADOR_ARMA_TRASERO_H
#define JUGADOR_ARMA_TRASERO_H

#include "jugador_arma.h"

namespace App
{
class Jugador_arma_trasero:
	public Jugador_arma
{
	public:

						Jugador_arma_trasero();
	virtual bool				es_arma_defecto() const {return false;}
	virtual void				generar_proyectiles(Disparador::v_info&, int, DLibH::Punto_2d<double>, double, double);
	virtual void				disparar();
};
}

#endif
