#include "jugador_arma_alt.h"

using namespace App;

Jugador_arma_alt::Jugador_arma_alt()
	:municion(20)
{

}

void Jugador_arma_alt::generar_proyectiles(Disparador::v_info& info, int indice, DLibH::Punto_2d<double> pt, double angulo)
{
	info.push_back({std::unique_ptr<Proyectil>(new Proyectil_alt(indice)), pt, angulo-7.0});
	info.push_back({std::unique_ptr<Proyectil>(new Proyectil_alt(indice)), pt, angulo});
	info.push_back({std::unique_ptr<Proyectil>(new Proyectil_alt(indice)), pt, angulo+7.0});
}

void Jugador_arma_alt::disparar()
{
	cooldown_disparo=0.1;
	--municion;
}
