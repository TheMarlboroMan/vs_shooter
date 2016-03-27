#include "decoracion.h"

using namespace App;

Decoracion::Decoracion(const tpoligono& p, tcolor c, tcolor cl, bool fr)
	:Obstaculo(p), color(c), color_linea(cl), frente(fr)
{

}

void Decoracion::dibujar(Representador& r, DLibV::Pantalla& pantalla, const Struct_camara& struct_camara) const
{
	r.dibujar_poligono(pantalla, poligono, color, struct_camara);
	if(color_linea!=color) r.dibujar_poligono_lineas(pantalla, poligono, color_linea, struct_camara);
}
