#ifndef TRAZADOR_RUTA_H
#define TRAZADOR_RUTA_H

#include "punto_ruta.h"

namespace App
{
class Trazador_ruta
{
	public:

	typedef DLibH::Punto_2d<double> 		tpunto_ruta;
	typedef std::vector<DLibH::Punto_2d<double>>	tvpunto_ruta;

	typedef Nodo_ruta				tnodo;
	typedef std::vector<Nodo_ruta>			tvnodo;

	typedef std::vector<Nodo_ruta::conexion>	tvconexiones;

	struct Ruta
	{
		tvpunto_ruta		 	ruta;
		double 				distancia;
		bool				resuelto;

		Ruta():distancia(0.0), resuelto(false) {}
	};

				Trazador_ruta();
	Ruta 			trazar_ruta(const tnodo& inicio, const tnodo& fin);

	private:

	void 			trazar(const tnodo& n, const tnodo& destino, tvpunto_ruta visitados, double distancia);
	tvconexiones	 	sustraer_visitados(const tvconexiones& conexiones, const tvpunto_ruta& visitados, double distancia);

	double 		distancia_minima;
	Ruta 		resultado;
};
}

#endif
