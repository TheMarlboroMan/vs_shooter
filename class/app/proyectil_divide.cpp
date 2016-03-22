#include "proyectil_divide.h"

using namespace App;

Proyectil_divide::Proyectil_divide(int indice):
	//TODO: Constantes???
	Proyectil(indice, 600.0, 0.5),
	color({255, 32, 32, 192})
{

}

void Proyectil_divide::colisionar(std::vector<Disparador>& d)
{
	Disparador res;

	auto pt=poligono.acc_centro();

	res.disparos.push_back({Disparador::tproyectiles::peq, indice_jugador, pt, angulo-110.0});
	res.disparos.push_back({Disparador::tproyectiles::peq, indice_jugador, pt, angulo+110.0});

	d.push_back(res);
}

void Proyectil_divide::extinguir(std::vector<Disparador>& d)
{
	Disparador res;

	auto pt=poligono.acc_centro();
	res.disparos.push_back({Disparador::tproyectiles::peq, indice_jugador, pt, angulo-90.0});
	res.disparos.push_back({Disparador::tproyectiles::peq, indice_jugador, pt, angulo+90.0});

	d.push_back(res);
}

void Proyectil_divide::preparar(double ang, DLibH::Punto_2d<double> pt)
{
	angulo=ang;
	formar_poligono();
	poligono.rotar(angulo);
	establecer_posicion(pt.x, pt.y);
}

void Proyectil_divide::turno(float delta)
{
	tiempo-=delta;
	desplazar_angulo_velocidad(angulo, velocidad*delta);
}

void Proyectil_divide::formar_poligono()
{
	poligono.insertar_vertice({4.0, 0.0});
	poligono.insertar_vertice({0.0, -4.0});
	poligono.insertar_vertice({-8.0, 0.0});
	poligono.insertar_vertice({0.0, 4.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}
