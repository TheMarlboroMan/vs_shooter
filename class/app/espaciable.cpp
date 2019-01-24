#include "espaciable.h"

using namespace App;

Espaciable::Espaciable()
{

}

Espaciable::Espaciable(const tpoligono& p)
	:poligono(p)
{

}

void Espaciable::establecer_posicion(double x, double y)
{
	poligono.center_in({x, y});
}

void Espaciable::mover(double x, double y)
{
	auto c=poligono.get_rotation_center();
	poligono.center_in({c.x+x, c.y+y});
}


bool Espaciable::en_colision_con(const Espaciable& e) const
{
	return SAT_collision_check(poligono, e.poligono);
}

void Espaciable::desplazar_angulo_velocidad(double angulo, double velocidad)
{
	ldt::vector_2d<double> v=vector_from_angle(angulo);
	ldt::point_2d<double> pd{v.x * velocidad, v.y * velocidad};
	poligono.move(pd);
}
