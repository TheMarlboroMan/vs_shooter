#ifndef JUGADOR_ARMA_H
#define JUGADOR_ARMA_H

#include <memory>
#include "proyectil.h"
#include "disparador.h"

namespace App
{

class Jugador_arma
{
	public:

	virtual void				generar_proyectiles(Disparador::v_info&, int, DLibH::Punto_2d<double>, double, double)=0;
	virtual void				disparar()=0;
	virtual int				acc_municion_restante() const=0;

	bool					es_agotada() const {return acc_municion_restante()==0;}
	bool					es_preparada() const {return !cooldown_disparo && !es_agotada();}

	void					turno(float delta);

	protected:

	float					cooldown_disparo;
};

}

#endif
