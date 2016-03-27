#include "jugador_info.h"

using namespace App;

Jugador_info::Jugador_info(int indice, tcolor color)
	:registrado(false), indice(indice), salud_max(100), puntuacion(0),
	color(color)
{
	switch(indice)
	{
		case 0:
		default: 
			traduccion_input=Traduccion_input{Input::j1_arriba, Input::j1_abajo, Input::j1_izquierda, Input::j1_derecha, Input::j1_disparo, Input::j1_habilidad};
			x_hud=16; y_hud=16;
		break;
		case 1:
			traduccion_input=Traduccion_input{Input::j2_arriba, Input::j2_abajo, Input::j2_izquierda, Input::j2_derecha, Input::j2_disparo, Input::j2_habilidad};
			x_hud=250; y_hud=16;
		break;
		case 2:
			traduccion_input=Traduccion_input{Input::j3_arriba, Input::j3_abajo, Input::j3_izquierda, Input::j3_derecha, Input::j3_disparo, Input::j3_habilidad};
			x_hud=500; y_hud=16;
		break;
	}
}
