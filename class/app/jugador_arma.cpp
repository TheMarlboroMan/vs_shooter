#include "jugador_arma.h"

using namespace App;

void Jugador_arma::turno(float delta)
{
	if(cooldown_disparo)
	{
		cooldown_disparo-=delta;
		if(cooldown_disparo <= 0.0f) cooldown_disparo=0.0f;
	}
}

