#ifndef MAPA_H
#define MAPA_H

#include <vector>

#include "obstaculo.h"
#include "punto_ruta.h"
#include "generador_items.h"

namespace App
{

class Mapa
{
	public:

	std::vector<Obstaculo>			obstaculos;
	std::vector<Espaciable::tpunto>		puntos_inicio;
	std::vector<Punto_ruta>			puntos_ruta;
	std::vector<Generador_items>		generadores_items;

	bool					visibilidad_entre_puntos(Espaciable::tpunto, Espaciable::tpunto) const;
	void					limpiar();
	void					construir_nodos_ruta();
	const std::vector<Nodo_ruta>&		acc_nodos_ruta() const {return nodos_ruta;}

	private:

	std::vector<Nodo_ruta>			nodos_ruta;

};
}

#endif
