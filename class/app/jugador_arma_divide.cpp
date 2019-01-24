#include "jugador_arma_divide.h"

using namespace App;

Jugador_arma_divide::Jugador_arma_divide()
	:Jugador_arma(30)
{

}

void Jugador_arma_divide::generar_proyectiles(Disparador::v_info& info, int indice, ldt::point_2d<double> pt, double distancia, double angulo)
{
	ldt::vector_2d<double> v=vector_from_angle(angulo);
	pt.x+=v.x * distancia;
	pt.y+=v.y * distancia;

	info.push_back({Disparador::tproyectiles::divide, indice, pt, angulo});
}

void Jugador_arma_divide::disparar()
{
	--municion;
	cooldown_disparo=0.12;
}
