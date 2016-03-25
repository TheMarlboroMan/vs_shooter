#include "obstaculo.h"

using namespace App;

Obstaculo::Obstaculo(const tpoligono& p, tcolor c, tcolor cl)
	:Espaciable(p), color(c), color_linea(cl)
{

}

void Obstaculo::dibujar(Representador& r, DLibV::Pantalla& pantalla, const Struct_camara& struct_camara) const
{
	r.dibujar_poligono(pantalla, poligono, color, struct_camara);
	if(color_linea!=color) r.dibujar_poligono_lineas(pantalla, poligono, color_linea, struct_camara);
}
