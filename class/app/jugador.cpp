#include "jugador.h"

using namespace App;

Jugador::Jugador(int indice, tcolor color)
	:indice(indice), angulo(90.0), velocidad(0.0f), salud(100), color(color)
{
	formar_poligono();
}

void Jugador::recibir_input(const Bloque_input& bi)
{
	input_actual=bi;
}

void Jugador::turno(float delta)
{
	if(input_actual.giro) girar(input_actual.giro, delta);

	if(input_actual.aceleracion) cambiar_velocidad(input_actual.aceleracion, delta);
	else if(velocidad) detener_velocidad(delta);

	if(velocidad) movimiento_tentativo(delta);
}

void Jugador::confirmar_movimiento()
{
	posicion_anterior=poligono;
}

void Jugador::cancelar_movimiento()
{
	poligono=posicion_anterior;
}

void Jugador::restar_salud(int v)
{
	salud-=v;
	if(salud < 0) salud=0;
}

void Jugador::girar(int dir, float delta)
{
	//TODO. A otro lado???
	const double factor_rotacion=90.0;

	double factor=((double)delta * factor_rotacion) * (double)dir;
	angulo+=factor;
	poligono.rotar(factor);
}

void Jugador::cambiar_velocidad(int dir, float delta)
{
	//TODO: A otro lado???
	const float max_vel=3.0f;
	const float min_vel=-1.0f;
	const float factor_aceleracion=1.0f;
	const float factor_freno=-5.0;

	velocidad+=dir > 0 ? delta * factor_aceleracion : delta * factor_freno;

	if(velocidad > max_vel) velocidad=max_vel;
	else if(velocidad < min_vel) velocidad=min_vel;
}

void Jugador::detener_velocidad(float delta)
{
	float vel=fabs(velocidad)-delta;
	if(vel < 0.0f) vel=0.0f;
	velocidad=vel* (velocidad > 0.f ? 1.0 : -1.0);
}

void Jugador::movimiento_tentativo(float delta)
{
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	DLibH::Punto_2d<double> pd{v.x * velocidad, v.y * velocidad};
	poligono.desplazar(pd);
}

void Jugador::formar_poligono()
{
	poligono.insertar_vertice({10.0, 30.0});
	poligono.insertar_vertice({20.0, 0.0});
	poligono.insertar_vertice({0.0, 0.0});
	poligono.cerrar();
	poligono.mut_centro({10.0, 10.0});
}
