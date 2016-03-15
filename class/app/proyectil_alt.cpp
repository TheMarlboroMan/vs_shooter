#include "proyectil_alt.h"

using namespace App;

Proyectil_alt::Proyectil_alt(int indice):
	//TODO: Constantes???
	Proyectil(indice, 6.0, 1.0),
	color({255, 255, 255, 192})
{

}

void Proyectil_alt::preparar(double ang, DLibH::Punto_2d<double> pt)
{
	angulo=ang;
	formar_poligono();
	poligono.rotar(angulo);
	establecer_posicion(pt.x, pt.y);
}

void Proyectil_alt::turno(float delta)
{
	tiempo-=delta;
	velocidad-=5.0*delta;
	if(velocidad < 0.2) velocidad=0.2;

	//TODO: Esto ya es común a dos clases.
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	DLibH::Punto_2d<double> pd{v.x * velocidad, v.y * velocidad};
	poligono.desplazar(pd);
}

void Proyectil_alt::formar_poligono()
{
	poligono.insertar_vertice({2.0, 0.0});
	poligono.insertar_vertice({0.0, -2.0});
	poligono.insertar_vertice({-8.0, 0.0});
	poligono.insertar_vertice({0.0, 2.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}
