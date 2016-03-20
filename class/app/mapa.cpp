#include "mapa.h"

using namespace App;

void Mapa::limpiar()
{
	obstaculos.clear();
	puntos_inicio.clear();
	generadores_items.clear();
}

bool Mapa::visibilidad_entre_puntos(Espaciable::tpunto pt1, Espaciable::tpunto pt2) const
{
	DLibH::Segmento_2d<double> seg={ {pt1.x, pt1.y}, {pt2.x, pt2.y}};

	for(const auto& o : obstaculos)
	{
		if(interseccion_segmento_poligono(seg, o.acc_poligono()))
		{
			return false;
		}
	}

	return true;
}

void Mapa::construir_nodos_ruta()
{
	nodos_ruta.clear();

	for(const auto& p : puntos_ruta)
	{
		Nodo_ruta nr{p};
		
		for(const auto& op : puntos_ruta)
		{
			if(p.id != op.id)
			{
				if(visibilidad_entre_puntos(p.pt, op.pt))
				{
					nr.conexiones.push_back(Nodo_ruta::conexion{op, p.pt.distancia_hasta(op.pt)});
				}
			}
		}

		//TODO: Quizás eliminar los duplicados...

		nodos_ruta.push_back(nr);
	}

/*
	for(const auto& n : nodos_ruta)
	{
std::cout<<"ORIGEN EN "<<n.origen.id<<":"<<std::endl;
		for(const auto& c : n.conexiones)
		{
std::cout<<"\t"<<c.destino.id<<" ["<<c.distancia<<"]"<<std::endl;
		}
	}
*/
}
