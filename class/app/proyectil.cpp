#include "proyectil.h"

#include <video/representation/raster/bitmap/bitmap_representation.h>
#include <video/resource_manager/resource_manager.h>

using namespace App;

Proyectil::Proyectil(int ij, double v, double t)
	:angulo(0.0), velocidad(v), tiempo(t),
	indice_jugador(ij)
{

}

void Proyectil::dibujar(Representador& r, ldv::screen& pantalla, const ldv::camera& camara, const ldv::resource_manager& _rm) const
{
	ldv::bitmap_representation bmp(_rm.get_texture(1));

	auto c=poligono.get_rotation_center();
	bmp.set_blend(ldv::representation::blends::alpha);
	bmp.set_alpha(64);
	bmp.go_to({c.x - 25, -c.y - 25});
	bmp.draw(pantalla, camara);

	r.dibujar_poligono(pantalla, poligono, acc_color(), camara);
}
