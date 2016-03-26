#include "jugador_arma_divide.h"

using namespace App;

Jugador_arma_divide::Jugador_arma_divide()
	:Jugador_arma(30)
{

}

void Jugador_arma_divide::generar_proyectiles(Disparador::v_info& info, int indice, DLibH::Punto_2d<double> pt, double distancia, double angulo)
{
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	pt.x+=v.x * distancia;
	pt.y+=v.y * distancia;

	info.push_back({Disparador::tproyectiles::divide, indice, pt, angulo});
}

void Jugador_arma_divide::disparar()
{
	--municion;
	cooldown_disparo=0.12;
}

