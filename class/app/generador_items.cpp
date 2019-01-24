#include "generador_items.h"

#include <video/representation/raster/bitmap/bitmap_representation.h>

#include <class/number_generator.h>

using namespace App;

Generador_items::Generador_items(Espaciable::tpunto pt)
	:tipo(titems::triple), tiempo_restante(0.0)
{
	formar_poligono();
	establecer_posicion(pt.x, pt.y);
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

	tools::int_generator gen(0, 3);
	switch(gen())
	{
		case 0: tipo=titems::triple; break;
		case 1: tipo=titems::explosivo; break;
		case 2: tipo=titems::trasero; break;
		case 3: tipo=titems::divide; break;
	}
}

void Generador_items::formar_poligono()
{
	poligono.add_vertex({-5.0, 10.0});
	poligono.add_vertex({5.0, 10.0});
	poligono.add_vertex({10.0, 5.0});
	poligono.add_vertex({10.0, -5.0});
	poligono.add_vertex({5.0, -10.0});
	poligono.add_vertex({-5.0, -10.0});
	poligono.add_vertex({-10.0, -5.0});
	poligono.add_vertex({-10.0, 5.0});
	poligono.set_rotation_center({0.0, 0.0});
}

void Generador_items::dibujar(Representador& r, ldv::screen& pantalla, const ldv::camera& camara, const ldv::resource_manager& _rm) const
{
	if(!tiempo_restante)
	{
		int indice=0;

		switch(tipo)
		{
			case titems::triple: 	indice=2; break;
			case titems::explosivo: indice=3; break;
			case titems::trasero:	indice=4; break;
			case titems::divide:	indice=5; break;
		}

		ldv::bitmap_representation bmp(_rm.get_texture(indice));

		auto c=poligono.get_rotation_center();
		bmp.set_blend(ldv::representation::blends::alpha);
		bmp.go_to({c.x - 25, -c.y - 25});
		bmp.draw(pantalla, camara);

	}

}
