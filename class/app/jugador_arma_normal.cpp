#include "jugador_arma_normal.h"

using namespace App;

void Jugador_arma_normal::generar_proyectiles(Disparador::v_info& info, int indice, DLibH::Punto_2d<double> pt, double angulo)
{
	info.push_back({Disparador::tproyectiles::normal, indice, pt, angulo});
}

void Jugador_arma_normal::disparar()
{
	cooldown_disparo=0.2;
}
