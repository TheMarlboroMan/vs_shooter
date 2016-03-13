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
	poligono.centrar_en({x, y});
}

bool Espaciable::en_colision_con(const Espaciable& e) const
{
	return colision_poligono_SAT(poligono, e.poligono);
}
