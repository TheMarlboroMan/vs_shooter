#ifndef BOT_H
#define BOT_H

#include "espaciable.h"
#include "trazador_ruta.h"
#include "mapa.h"

namespace App
{

class Bot:
	public Espaciable
{
	public:

					Bot();
	void				turno(const Mapa&, float);
	void				establecer_destino(const Espaciable& e);

	private:

	enum class testados {nada, buscando, rotando, moviendo};

	void				formar_poligono();
	void				trazar_ruta(const Mapa&);
	void				orientar_giro(float);
	void				mover(float);

	std::vector<Espaciable::tpunto>	ruta;
	const Espaciable *		destino;

	testados			estado;
	double				angulo;
	float				velocidad;	
};

}

#endif
