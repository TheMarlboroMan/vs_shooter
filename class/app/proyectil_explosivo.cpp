#include "proyectil_explosivo.h"

using namespace App;

Proyectil_explosivo::Proyectil_explosivo(int indice):
	//TODO: Constantes???
	Proyectil(indice, 800.0, 1.0),
	color({255, 16, 16, 192})
{

}

void Proyectil_explosivo::colisionar(std::vector<Disparador>& d)
{
	extinguir(d);
}

//TODO: Mejorar... La interface es inconsistente con las armas...
void Proyectil_explosivo::extinguir(std::vector<Disparador>& d)
{
	Disparador res;

	double i=0;
	auto pt=poligono.acc_centro();
	while(i < 360.0)
	{
		res.disparos.push_back({Disparador::tproyectiles::peq, indice_jugador, pt, i});
		i+=10.0;
	}

	d.push_back(res);
}

void Proyectil_explosivo::preparar(double ang, DLibH::Punto_2d<double> pt)
{
	angulo=ang;
	rot=angulo;
	formar_poligono();
	poligono.rotar(angulo);
	establecer_posicion(pt.x, pt.y);
}

void Proyectil_explosivo::turno(float delta)
{
	velocidad-=delta * 1000.0;
	if(velocidad <= 100.0) velocidad=100.0;
	rot+=delta * 100.0;
	poligono.rotar(angulo);
	tiempo-=delta;
	desplazar_angulo_velocidad(angulo, velocidad*delta);
}

void Proyectil_explosivo::formar_poligono()
{
	poligono.insertar_vertice({-8.0, 0.0});
	poligono.insertar_vertice({-4.0, 4.0});
	poligono.insertar_vertice({4.0, 4.0});
	poligono.insertar_vertice({8.0, 0.0});
	poligono.insertar_vertice({4.0, -4.0});
	poligono.insertar_vertice({-4.0, -4.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}
