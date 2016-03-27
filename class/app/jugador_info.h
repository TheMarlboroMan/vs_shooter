#ifndef JUGADOR_INFO_H
#define JUGADOR_INFO_H

#include "color.h"
#include "framework_impl/input.h"
#include "bloque_input.h"

namespace App
{

class Jugador_info
{
	public:
				Jugador_info(int, tcolor);
	int			acc_indice() const {return indice;}
	const tcolor&		acc_color() const {return color;}
	bool			es_registrado() const {return registrado;}
	const Traduccion_input&	acc_traduccion_input() const {return traduccion_input;}
	int			acc_x_hud() const {return x_hud;}
	int			acc_y_hud() const {return y_hud;}
	


	void			registrar() {registrado=true;}
	void			retirar() {registrado=false;}

	private:

	bool			registrado;
	int			indice,
				salud_max,
				puntuacion;
	tcolor			color;
	Traduccion_input	traduccion_input;
	int			x_hud, y_hud;

};

}

#endif
