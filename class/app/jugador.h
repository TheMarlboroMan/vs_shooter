#ifndef JUGADOR_H
#define JUGADOR_h

#include "color.h"
#include "espaciable.h"
#include "bloque_input.h"

#include "disparador.h"
#include "jugador_arma.h"

namespace App
{

class Jugador:
	public Espaciable
{
	public:

					Jugador(int, tcolor);

	int				acc_indice() const {return indice;}
	int				acc_salud() const {return salud;}
	tcolor				acc_color() const {return color;}

	bool				es_y_puede_disparar() const {return input_actual.disparo && arma!=nullptr && arma->es_preparada();
	}

	bool				es_arma_agotada() const;	
	int				acc_municion_restante() const;

	Disparador			disparar();
	void				recibir_input(const Bloque_input&);
	void				turno(float);
	void				confirmar_movimiento();
	void				colisionar();
	void				restar_salud(int);
	void				establecer_arma(Jugador_arma * a);

	private:

	void				formar_poligono();
	void				cambiar_velocidad(int, float);
	void				detener_velocidad(float);
	void				girar(int, float);
	void				movimiento_tentativo(float);

	std::unique_ptr<Jugador_arma>	arma;
	Bloque_input			input_actual;
	tpoligono			posicion_anterior;

	int				indice;	//Indice del input que lo controla.
	double 				angulo, angulo_anterior;
	float				velocidad;
	int				salud;
	tcolor				color;
};

}

#endif
