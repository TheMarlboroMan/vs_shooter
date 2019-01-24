#include "jugador_arma_explosivo.h"

using namespace App;

Jugador_arma_explosivo::Jugador_arma_explosivo()
	:Jugador_arma(3)
{

}

void Jugador_arma_explosivo::generar_proyectiles(Disparador::v_info& info, int indice, ldt::point_2d<double> pt, double distancia, double angulo)
{
	ldt::vector_2d<double> v=vector_from_angle(angulo);
	pt.x+=v.x * distancia;
	pt.y+=v.y * distancia;

	info.push_back({Disparador::tproyectiles::explosivo, indice, pt, angulo});
}

void Jugador_arma_explosivo::disparar()
{
	cooldown_disparo=0.5;
	--municion;
}
