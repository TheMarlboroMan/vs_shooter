#include "jugador_arma_explosivo.h"

using namespace App;

Jugador_arma_explosivo::Jugador_arma_explosivo()
	:municion(3)
{

}

void Jugador_arma_explosivo::generar_proyectiles(Disparador::v_info& info, int indice, DLibH::Punto_2d<double> pt, double angulo)
{
	info.push_back({Disparador::tproyectiles::explosivo, indice, pt, angulo});
}

void Jugador_arma_explosivo::disparar()
{
	cooldown_disparo=2.0;
	--municion;
}
