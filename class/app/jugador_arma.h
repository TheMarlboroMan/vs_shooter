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

						Jugador_arma(int m);
	virtual void				generar_proyectiles(Disparador::v_info&, int, ldt::point_2d<double>, double, double)=0;
	virtual void				disparar()=0;
	virtual bool				es_arma_defecto() const=0;
	virtual void				turno(float delta);

	int					acc_municion_restante() const {return municion;}
	bool					es_agotada() const {return municion==0;}
	bool					es_preparada() const {return !cooldown_disparo && !es_agotada();}


	protected:

	int					municion;
	float					cooldown_disparo;
};

}

#endif
