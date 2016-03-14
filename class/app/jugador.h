#ifndef JUGADOR_H
#define JUGADOR_h

#include "color.h"
#include "espaciable.h"
#include "bloque_input.h"

namespace App
{

class Jugador:
	public Espaciable
{
	public:

					Jugador(int, tcolor);

	//TODO: Inelegante... Se usa sólo para el disparo.
	double				acc_angulo() const {return angulo;}
	int				acc_indice() const {return indice;}
	int				acc_salud() const {return salud;}
	tcolor				acc_color() const {return color;}
	bool				es_y_puede_disparar() const {return input_actual.disparo && !cooldown_disparo;}
	
	DLibH::Punto_2d<double>		disparar();
	void				recibir_input(const Bloque_input&);
	void				turno(float);
	void				confirmar_movimiento();
	void				cancelar_movimiento();
	void				restar_salud(int);

	private:

	void				formar_poligono();
	void				cambiar_velocidad(int, float);
	void				detener_velocidad(float);
	void				girar(int, float);
	void				movimiento_tentativo(float);

	Bloque_input			input_actual;
	tpoligono			posicion_anterior;

	int				indice;	//Indice del input que lo controla.
	double 				angulo, angulo_anterior;
	float				velocidad, cooldown_disparo;
	int				salud;
	tcolor				color;
};

}

#endif
