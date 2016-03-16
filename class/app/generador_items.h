#ifndef GENERADOR_ITEMS_H
#define GENERADOR_ITEMS_H

#include "espaciable.h"
#include "color.h"

namespace App
{

/**
* Está fijo en el espacio, pasando el tiempo. Cuando el tiempo asignado pasa
* se colorea indicando que al tocarlo se obtiene un item.
*/

class Generador_items:
	public Espaciable
{
	public:

	enum class titems {triple, explosivo, trasero, divide};

					Generador_items(Espaciable::tpunto);

	titems				acc_tipo() const {return tipo;}
	bool				es_activo() const {return !tiempo_restante;}
	tcolor				acc_color() const;

	void				turno(float);
	void				reiniciar();

	private:

	void				formar_poligono();

	titems				tipo;
	float				tiempo_restante;
};

}

#endif
