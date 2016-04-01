#ifndef GENERADOR_ITEMS_H
#define GENERADOR_ITEMS_H

#include "espaciable.h"
#include "representable.h"

namespace App
{

/**
* Está fijo en el espacio, pasando el tiempo. Cuando el tiempo asignado pasa
* se colorea indicando que al tocarlo se obtiene un item.
*/

class Generador_items:
	public Espaciable,
	public Representable
{
	public:

	enum class titems {triple, explosivo, trasero, divide};

					Generador_items(Espaciable::tpunto);

	titems				acc_tipo() const {return tipo;}
	bool				es_activo() const {return !tiempo_restante;}

	void				turno(float);
	void				reiniciar();

	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	void				formar_poligono();

	titems				tipo;
	float				tiempo_restante;
};

}

#endif
