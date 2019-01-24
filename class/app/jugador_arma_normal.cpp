#include "jugador_arma_normal.h"

using namespace App;

Jugador_arma_normal::Jugador_arma_normal()
	:Jugador_arma(10), recarga(0.0f)
{

}

void Jugador_arma_normal::turno(float delta)
{
	Jugador_arma::turno(delta);

	recarga+=delta;

	if(!cooldown_disparo && municion < 10)
	{
		if(recarga >= 0.8f)
		{
			recarga=0.0f;
			municion++;
		}
	}
}

void Jugador_arma_normal::generar_proyectiles(Disparador::v_info& info, int indice, ldt::point_2d<double> pt, double distancia, double angulo)
{
	ldt::vector_2d<double> v=vector_from_angle(angulo);
	pt.x+=v.x * distancia;
	pt.y+=v.y * distancia;

	info.push_back({Disparador::tproyectiles::normal, indice, pt, angulo});
}

void Jugador_arma_normal::disparar()
{
	recarga=0.0f;
	cooldown_disparo=0.12;
	--municion;
}
