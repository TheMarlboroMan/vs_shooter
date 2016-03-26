#ifndef JUGADOR_ARMA_NORMAL_H
#define JUGADOR_ARMA_NORMAL_H

#include "jugador_arma.h"

namespace App
{
class Jugador_arma_normal:
	public Jugador_arma
{
	public:

						Jugador_arma_normal();
	virtual void				generar_proyectiles(Disparador::v_info&, int, DLibH::Punto_2d<double>, double, double);
	virtual bool				es_arma_defecto() const {return true;}
	virtual void				disparar();
	virtual void				turno(float delta);

	private:

	float					recarga;
};
}

#endif
