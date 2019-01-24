#include "jugador_arma_triple.h"

using namespace App;

Jugador_arma_triple::Jugador_arma_triple()
	:Jugador_arma(30)
{

}

void Jugador_arma_triple::generar_proyectiles(Disparador::v_info& info, int indice, ldt::point_2d<double> pt, double distancia, double angulo)
{
	ldt::vector_2d<double> v=vector_from_angle(angulo);
	pt.x+=v.x * distancia;
	pt.y+=v.y * distancia;

	info.push_back({Disparador::tproyectiles::peq, indice, pt, angulo-7.0});
	info.push_back({Disparador::tproyectiles::peq, indice, pt, angulo});
	info.push_back({Disparador::tproyectiles::peq, indice, pt, angulo+7.0});
}

void Jugador_arma_triple::disparar()
{
	cooldown_disparo=0.08;
	--municion;
}
