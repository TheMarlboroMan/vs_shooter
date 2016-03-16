#include "jugador_arma_normal.h"

using namespace App;

void Jugador_arma_normal::generar_proyectiles(Disparador::v_info& info, int indice, DLibH::Punto_2d<double> pt, double distancia, double angulo)
{
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	pt.x+=v.x * distancia;
	pt.y+=v.y * distancia;

	info.push_back({Disparador::tproyectiles::normal, indice, pt, angulo});
}

void Jugador_arma_normal::disparar()
{
	cooldown_disparo=0.2;
}
