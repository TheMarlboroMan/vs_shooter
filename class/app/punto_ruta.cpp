#include "punto_ruta.h"

using namespace App;

unsigned int Punto_ruta::cid=1;

Punto_ruta::Punto_ruta(Espaciable::tpunto p, bool usar_id)
	:pt(p), id(0)
{
	if(usar_id)
 	{
		id=cid++;
	}
}
