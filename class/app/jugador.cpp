#include "jugador.h"

using namespace App;

Jugador::Jugador(int indice, tcolor color)
	:arma(nullptr), indice(indice), angulo(0.0), angulo_anterior(angulo), velocidad(0.0f), 
	salud(100), color(color)
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
	if(arma) arma->turno(delta);
}

void Jugador::confirmar_movimiento()
{
	posicion_anterior=poligono;
	angulo_anterior=angulo;
}

void Jugador::colisionar()
{
	salud-=velocidad;
	poligono=posicion_anterior;
	angulo=angulo_anterior;
	velocidad=-velocidad / 2.0;
}

void Jugador::restar_salud(int v)
{
	salud-=v;
	if(salud < 0) salud=0;
}

void Jugador::girar(int dir, float delta)
{
	//TODO. A otro lado???
	const double factor_rotacion=180.0;

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
	desplazar_angulo_velocidad(angulo, velocidad);
}

void Jugador::formar_poligono()
{
	poligono.insertar_vertice({20.0, 0.0});
	poligono.insertar_vertice({0.0, -10.0});
	poligono.insertar_vertice({0.0, 10.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

Disparador Jugador::disparar()
{
	//TODO: Constantes por aquí?
	double distancia=30.0;

	//Obtener spawn point de disparo...
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	auto pt=poligono.acc_centro();
	pt.x+=v.x * distancia;
	pt.y+=v.y * distancia;

	Disparador res;
	arma->generar_proyectiles(res.disparos, indice, pt, angulo);
	arma->disparar();
	return res;
}

void Jugador::establecer_arma(Jugador_arma * a)
{
	arma.reset(nullptr);
	arma.reset(a);
}

int Jugador::acc_municion_restante() const
{
	if(arma!=nullptr)
	{
		return arma->acc_municion_restante();
	}

	return -1;
}

bool Jugador::es_arma_agotada() const
{
	if(arma!=nullptr) return arma->es_agotada();
	else return true;
}
