#include "proyectil_peq.h"

using namespace App;

Proyectil_peq::Proyectil_peq(int indice):
	//TODO: Constantes???
	Proyectil(indice, 700.0, 1.0),
	color({255, 255, 255, 192})
{

}

void Proyectil_peq::colisionar(std::vector<Disparador>&)
{

}

void Proyectil_peq::extinguir(std::vector<Disparador>&)
{
	
}

void Proyectil_peq::preparar(double ang, DLibH::Punto_2d<double> pt)
{
	angulo=ang;
	formar_poligono();
	poligono.rotar(angulo);
	establecer_posicion(pt.x, pt.y);
}

void Proyectil_peq::turno(float delta)
{
	tiempo-=delta;
	velocidad-=10.0*delta;
	if(velocidad < 100.0) velocidad=100.0;

	desplazar_angulo_velocidad(angulo, velocidad*delta);
}

void Proyectil_peq::formar_poligono()
{
	poligono.insertar_vertice({2.0, 0.0});
	poligono.insertar_vertice({0.0, -2.0});
	poligono.insertar_vertice({-8.0, 0.0});
	poligono.insertar_vertice({0.0, 2.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}
