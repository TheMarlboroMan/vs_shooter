#include "jugador.h"

using namespace App;

Jugador::Jugador(int indice, tcolor color)
	:arma(nullptr), indice(indice), angulo(0.0), angulo_anterior(angulo), velocidad(3.0f), 
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
	restar_salud(fabs(velocidad) / 4.0); //El fabs es importante, sobre todo si vamos marcha atrás...

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

	//La velocidad afecta a la capacidad de giro. No afecta parado o marcha atrás.	
	double factor=velocidad <= 0.0 ? factor_rotacion : factor_rotacion - (velocidad / 3.0);
	if(factor < 60.0) factor=60.0;

	double giro=((double)delta * factor) * (double)dir;
	angulo+=giro;
	poligono.rotar(giro);
}

void Jugador::cambiar_velocidad(int dir, float delta)
{
	//TODO: A otro lado???
	const float max_vel=300.0f;
	const float min_vel=-100.0f;
	const float factor_aceleracion=180.0f;
	const float factor_freno=-500.0;

	//TODO: ¿Cómo es eso de la velocidad máxima?
	velocidad+=dir > 0 ? delta * factor_aceleracion : delta * factor_freno;

	if(velocidad > max_vel) velocidad=max_vel;
	else if(velocidad < min_vel) velocidad=min_vel;
}

void Jugador::detener_velocidad(float delta)
{
	const float factor_perdida=100.0;
	float vel=fabs(velocidad)-(delta * factor_perdida);

	if(vel < 0.0f) vel=0.0f;
	velocidad=vel* (velocidad > 0.f ? 1.0 : -1.0);
}

void Jugador::movimiento_tentativo(float delta)
{
	desplazar_angulo_velocidad(angulo, velocidad*delta);

	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	DLibH::Punto_2d<double> pd{v.x * velocidad, v.y * velocidad};
}

void Jugador::formar_poligono()
{
	poligono.insertar_vertice({20.0, 0.0});
	poligono.insertar_vertice({0.0, -7.0});
	poligono.insertar_vertice({0.0, 7.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

Disparador Jugador::disparar()
{
	//TODO: Constantes por aquí?
	double distancia=30.0;

	//Obtener spawn point de disparo...
	auto pt=poligono.acc_centro();

	//TODO: Mejorar: quizás el jugador puede computar los puntos posibles para
	//iniciar un disparo en este punto y enviarlos todos. El arma podría saber
	//qué puntos tiene que elegir???.

	Disparador res;
	arma->generar_proyectiles(res.disparos, indice, pt, distancia, angulo);
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
