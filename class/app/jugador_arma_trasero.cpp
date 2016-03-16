#include "jugador_arma_trasero.h"

using namespace App;

Jugador_arma_trasero::Jugador_arma_trasero()
	:municion(20)
{

}

void Jugador_arma_trasero::generar_proyectiles(Disparador::v_info& info, int indice, DLibH::Punto_2d<double> pt, double distancia, double angulo)
{
	double a1=angulo-180.0,
		a2=angulo+90.0,
		a3=angulo-90.0;

	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(a1);
	auto pt2=pt;
	pt2.x+=v.x * distancia;
	pt2.y+=v.y * distancia;
	info.push_back({Disparador::tproyectiles::peq, indice, pt2, a1});

	v=vector_unidad_para_angulo_cartesiano(a2);
	pt2=pt;
	pt2.x+=v.x * distancia;
	pt2.y+=v.y * distancia;
	info.push_back({Disparador::tproyectiles::peq, indice, pt2, a2});

	v=vector_unidad_para_angulo_cartesiano(a3);
	pt2=pt;
	pt2.x+=v.x * distancia;
	pt2.y+=v.y * distancia;
	info.push_back({Disparador::tproyectiles::peq, indice, pt2, a3});
}

void Jugador_arma_trasero::disparar()
{
	cooldown_disparo=0.1;
	--municion;
}
