#include "proyectil_normal.h"

using namespace App;

Proyectil_normal::Proyectil_normal(int indice):
	//TODO: Constantes???
	Proyectil(indice, 500.0, 1.0),
	color({128, 128, 64, 192})
{

}

void Proyectil_normal::colisionar(std::vector<Disparador>& d)
{

}

void Proyectil_normal::extinguir(std::vector<Disparador>& d)
{

}

void Proyectil_normal::preparar(double ang, ldt::point_2d<double> pt)
{
	angulo=ang;
	formar_poligono();
	poligono.rotate(angulo);
	establecer_posicion(pt.x, pt.y);
}

void Proyectil_normal::turno(float delta)
{
	tiempo-=delta;
	desplazar_angulo_velocidad(angulo, velocidad*delta);
}

void Proyectil_normal::formar_poligono()
{
	poligono.add_vertex({4.0, 0.0});
	poligono.add_vertex({0.0, -4.0});
	poligono.add_vertex({-8.0, 0.0});
	poligono.add_vertex({0.0, 4.0});
	poligono.set_rotation_center({0.0, 0.0});
}
