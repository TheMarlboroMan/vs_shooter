#ifndef PUNTO_RUTA_H
#define PUNTO_RUTA_H

#include "espaciable.h"

namespace App
{
struct Punto_ruta
{
	Espaciable::tpunto	pt;
	unsigned int            id;
	
	Punto_ruta(Espaciable::tpunto);

	private:

	static unsigned int cid;
};
}

#endif
