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
	generar(d, 110.0);
}

void Proyectil_divide::extinguir(std::vector<Disparador>& d)
{
	generar(d, 90.0);
}

void Proyectil_divide::preparar(double ang, ldt::point_2d<double> pt)
{
	angulo=ang;
	formar_poligono();
	poligono.rotate(angulo);
	establecer_posicion(pt.x, pt.y);
}

void Proyectil_divide::generar(std::vector<Disparador>& d, double ang)
{
	Disparador res;

	auto ang1=angulo-ang;
	auto ang2=angulo+ang;

	auto ptc=poligono.get_rotation_center();

	auto v1=vector_from_angle(ang1);
	auto v2=vector_from_angle(ang2);

	res.disparos.push_back({Disparador::tproyectiles::peq, indice_jugador, ptc+ldt::point_2d<double>{20.0 * v1.x, 20.0 * v1.y}, ang1});
	res.disparos.push_back({Disparador::tproyectiles::peq, indice_jugador, ptc+ldt::point_2d<double>{20.0 * v2.x, 20.0 * v2.y}, ang2});

	d.push_back(res);
}

void Proyectil_divide::turno(float delta)
{
	tiempo-=delta;
	desplazar_angulo_velocidad(angulo, velocidad*delta);
}

void Proyectil_divide::formar_poligono()
{
	poligono.add_vertex({4.0, 0.0});
	poligono.add_vertex({0.0, -4.0});
	poligono.add_vertex({-8.0, 0.0});
	poligono.add_vertex({0.0, 4.0});
	poligono.set_rotation_center({0.0, 0.0});
}
