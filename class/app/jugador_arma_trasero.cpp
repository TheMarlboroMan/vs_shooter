#include "jugador_arma_trasero.h"

using namespace App;

Jugador_arma_trasero::Jugador_arma_trasero()
	:Jugador_arma(30)
{

}

void Jugador_arma_trasero::generar_proyectiles(Disparador::v_info& info, int indice, ldt::point_2d<double> pt, double distancia, double angulo)
{
	double a1=angulo-180.0,
		a2=angulo+90.0,
		a3=angulo-90.0;

	ldt::vector_2d<double> v=vector_from_angle(a1);
	auto pt2=pt;
	pt2.x+=v.x * distancia;
	pt2.y+=v.y * distancia;
	info.push_back({Disparador::tproyectiles::peq, indice, pt2, a1});

	v=vector_from_angle(a2);
	pt2=pt;
	pt2.x+=v.x * distancia;
	pt2.y+=v.y * distancia;
	info.push_back({Disparador::tproyectiles::peq, indice, pt2, a2});

	v=vector_from_angle(a3);
	pt2=pt;
	pt2.x+=v.x * distancia;
	pt2.y+=v.y * distancia;
	info.push_back({Disparador::tproyectiles::peq, indice, pt2, a3});
}

void Jugador_arma_trasero::disparar()
{
	cooldown_disparo=0.08;
	--municion;
}
