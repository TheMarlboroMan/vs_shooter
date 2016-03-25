#ifndef PUNTO_RUTA_H
#define PUNTO_RUTA_H

#include <vector>
#include "espaciable.h"

namespace App
{
struct Punto_ruta
{
	Espaciable::tpunto	pt;
	unsigned int            id;
	
	Punto_ruta(Espaciable::tpunto, bool=true);

	private:

	static unsigned int cid;
};

struct Nodo_ruta
{
	const Punto_ruta&	origen;
	
	struct conexion
	{	
		const Nodo_ruta&	destino;
		double			distancia;
	};
	
	std::vector<conexion>	conexiones;
};
}

#endif
