#include "proyectil.h"

using namespace App;

Proyectil::Proyectil(double angulo, double x, double y, int ij, tcolor c)
	:angulo(angulo), velocidad(4.0), tiempo(1.0f), potencia(20), 
	indice_jugador(ij), color(c)
{
	formar_poligono();
	poligono.rotar(angulo);
	establecer_posicion(x, y);
}


void Proyectil::turno(float delta)
{
	tiempo-=delta;

	//TODO: Esto ya es común a dos clases.
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	DLibH::Punto_2d<double> pd{v.x * velocidad, v.y * velocidad};
	poligono.desplazar(pd);
}

void Proyectil::formar_poligono()
{
	poligono.insertar_vertice({4.0, 0.0});
	poligono.insertar_vertice({0.0, -4.0});
	poligono.insertar_vertice({-8.0, 0.0});
	poligono.insertar_vertice({0.0, 4.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}
