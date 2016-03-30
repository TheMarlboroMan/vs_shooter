#include "proyectil.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>

using namespace App;

Proyectil::Proyectil(int ij, double v, double t)
	:angulo(0.0), velocidad(v), tiempo(t),
	indice_jugador(ij)
{

}

void Proyectil::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	DLibV::Representacion_bitmap bmp(DLibV::Gestor_texturas::obtener(1));

	auto c=poligono.acc_centro();
	bmp.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	bmp.establecer_alpha(64);
	bmp.ir_a(c.x - 25, -c.y - 25);
	bmp.volcar(pantalla, camara);

	r.dibujar_poligono(pantalla, poligono, acc_color(), camara);
}
