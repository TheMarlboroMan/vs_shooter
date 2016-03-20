#include "trazador_ruta.h"

using namespace App;

Trazador_ruta::Trazador_ruta()
	:distancia_minima(0.0)
{

}

Trazador_ruta::Ruta Trazador_ruta::trazar_ruta(const tnodo& inicio, const tnodo& fin)
{
	tvpunto_ruta visitados;
	trazar(inicio, fin, visitados, 0.0);
	return resultado;
}

void Trazador_ruta::trazar(const tnodo& n, const tnodo& destino, tvpunto_ruta visitados, double distancia)
{
	visitados.push_back(n.origen.pt);

	if(n.origen.id == destino.origen.id)
	{
		distancia_minima=distancia;
		resultado.ruta=visitados;
		resultado.distancia=distancia;
		resultado.resuelto=true;
	}
	else
	{
		auto conexiones=sustraer_visitados(n.conexiones, visitados, distancia);

		if(!conexiones.size())
		{
			return;
		}
		else
		{
			for(const auto& p : conexiones)
			{
				trazar(p.destino, destino, visitados, distancia+p.distancia);
			}
		}
	}
}

Trazador_ruta::tvconexiones Trazador_ruta::sustraer_visitados(const tvconexiones& conexiones, const tvpunto_ruta& visitados, double distancia)
{
	tvconexiones res;

	for(const auto& n : conexiones)
	{
		double nueva_distancia=distancia+n.distancia;

		if(resultado.resuelto && nueva_distancia > distancia_minima)
		{
			continue;
		}

		const auto& pt=n.destino.origen.pt;

		bool insertar=true;

		for(const auto& pv : visitados)
		{
			if(pt==pv)
			{
				insertar=false;
				continue;
			}
		}

		if(insertar)
		{
			res.push_back(n);
		}
	}

	return res;
}
