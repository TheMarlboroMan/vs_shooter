#include "generador_items.h"

#include <class/generador_numeros.h>

using namespace App;

Generador_items::Generador_items(Espaciable::tpunto pt)
	:tipo(titems::triple), tiempo_restante(0.0)
{
	formar_poligono();
	establecer_posicion(pt.x, pt.y);
}

tcolor Generador_items::acc_color() const
{
	switch(tipo)
	{
		case titems::triple:
			return {32, 255, 32, 128};
		break;
	
		case titems::explosivo:
			return {255, 32, 32, 128};
		break;

		default:
			return {255, 255, 255, 128};
		break;
	}
}

void Generador_items::turno(float delta)
{
	if(tiempo_restante)
	{
		tiempo_restante-=delta;
		if(tiempo_restante < 0.0f) tiempo_restante=0.0f;
	}
	
}

void Generador_items::reiniciar()
{
	tiempo_restante=6.0;

	Herramientas_proyecto::Generador_int gen(0, 1);
	switch(gen())
	{
		case 0: tipo=titems::triple; break;
		case 1: tipo=titems::explosivo; break;
	}
}

void Generador_items::formar_poligono()
{
	poligono.insertar_vertice({-5.0, 10.0});
	poligono.insertar_vertice({5.0, 10.0});
	poligono.insertar_vertice({10.0, 5.0});
	poligono.insertar_vertice({10.0, -5.0});
	poligono.insertar_vertice({5.0, -10.0});
	poligono.insertar_vertice({-5.0, -10.0});
	poligono.insertar_vertice({-10.0, -5.0});
	poligono.insertar_vertice({-10.0, 5.0});
	poligono.cerrar();
	poligono.mut_centro({-10.0, -10.0});
}
