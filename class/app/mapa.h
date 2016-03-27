#ifndef MAPA_H
#define MAPA_H

#include <vector>

#include "decoracion.h"
#include "obstaculo.h"
#include "punto_ruta.h"
#include "generador_items.h"

namespace App
{

class Mapa
{
	public:

	std::vector<Obstaculo>			obstaculos;
	std::vector<Decoracion>			decoraciones;
	std::vector<Espaciable::tpunto>		puntos_inicio;
	std::vector<Espaciable::tpunto>		puntos_bot;
	std::vector<Punto_ruta>			puntos_ruta;
	std::vector<Generador_items>		generadores_items;

	std::vector<const Decoracion *>		decoraciones_frente;
	std::vector<const Decoracion *>		decoraciones_fondo;

	void					inicializar();
	bool					visibilidad_entre_puntos(Espaciable::tpunto, Espaciable::tpunto) const;
	void					limpiar();
	void					construir_nodos_ruta();
	const std::vector<Nodo_ruta>&		acc_nodos_ruta() const {return nodos_ruta;}
	const Nodo_ruta * 			localizar_nodo_cercano(Espaciable::tpunto) const;
	Nodo_ruta				crear_inicio_temporal(Punto_ruta& pr) const;

	private:

	bool					comprobacion_ancho(Espaciable::tpunto, Espaciable::tpunto, double) const;

	std::vector<Nodo_ruta>			nodos_ruta;

};
}

#endif
