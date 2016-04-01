#include "generador_items.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>

#include <class/generador_numeros.h>

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

	Herramientas_proyecto::Generador_int gen(0, 3);
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
	poligono.insertar_vertice({-5.0, 10.0});
	poligono.insertar_vertice({5.0, 10.0});
	poligono.insertar_vertice({10.0, 5.0});
	poligono.insertar_vertice({10.0, -5.0});
	poligono.insertar_vertice({5.0, -10.0});
	poligono.insertar_vertice({-5.0, -10.0});
	poligono.insertar_vertice({-10.0, -5.0});
	poligono.insertar_vertice({-10.0, 5.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Generador_items::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
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

		DLibV::Representacion_bitmap bmp(DLibV::Gestor_texturas::obtener(indice));

		auto c=poligono.acc_centro();
		bmp.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
		bmp.ir_a(c.x - 25, -c.y - 25);
		bmp.volcar(pantalla, camara);

	}

}
