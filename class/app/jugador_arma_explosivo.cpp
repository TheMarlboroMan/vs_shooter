#include "jugador_arma_explosivo.h"

using namespace App;

Jugador_arma_explosivo::Jugador_arma_explosivo()
	:municion(3)
{

}

void Jugador_arma_explosivo::generar_proyectiles(Disparador::v_info& info, int indice, DLibH::Punto_2d<double> pt, double distancia, double angulo)
{
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	pt.x+=v.x * distancia;
	pt.y+=v.y * distancia;

	info.push_back({Disparador::tproyectiles::explosivo, indice, pt, angulo});
}

void Jugador_arma_explosivo::disparar()
{
	cooldown_disparo=1.0;
	--municion;
}
