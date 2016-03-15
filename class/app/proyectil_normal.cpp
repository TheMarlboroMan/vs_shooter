#include "proyectil_normal.h"

using namespace App;

Proyectil_normal::Proyectil_normal(int indice):
	//TODO: Constantes???
	Proyectil(indice, 4.0, 2.0),
	color({128, 128, 64, 192})
{

}

void Proyectil_normal::colisionar(std::vector<Disparador>& d)
{

}

void Proyectil_normal::extinguir(std::vector<Disparador>& d)
{

}

void Proyectil_normal::preparar(double ang, DLibH::Punto_2d<double> pt)
{
	angulo=ang;
	formar_poligono();
	poligono.rotar(angulo);
	establecer_posicion(pt.x, pt.y);
}

void Proyectil_normal::turno(float delta)
{
	tiempo-=delta;
	desplazar_angulo_velocidad(angulo, velocidad);
}

void Proyectil_normal::formar_poligono()
{
	poligono.insertar_vertice({4.0, 0.0});
	poligono.insertar_vertice({0.0, -4.0});
	poligono.insertar_vertice({-8.0, 0.0});
	poligono.insertar_vertice({0.0, 4.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}
