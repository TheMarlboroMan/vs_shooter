#include "proyectil.h"

using namespace App;

Proyectil::Proyectil(int ij, double v, double t)
	:angulo(0.0), velocidad(v), tiempo(t),
	indice_jugador(ij)
{

}

void Proyectil::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	r.dibujar_poligono(pantalla, poligono, acc_color(), camara);
}
