#ifndef REPRESENTABLE_H
#define REPRESENTABLE_H

#include <video/pantalla/pantalla.h>

#include "representador.h"
#include "struct_camara.h"
#include "color.h"

namespace App
{

class Representable
{
	public:

	virtual void dibujar(Representador&, DLibV::Pantalla&, const Struct_camara&) const=0;
};

}

#endif
