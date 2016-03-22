#include "mapa.h"

using namespace App;

void Mapa::limpiar()
{
	obstaculos.clear();
	puntos_inicio.clear();
	generadores_items.clear();
	puntos_ruta.clear();
	nodos_ruta.clear();
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

	//En primer lugar creamos todos los nodos de ruta, vacíos...
	for(const auto& p : puntos_ruta)
	{
		nodos_ruta.push_back(Nodo_ruta{p});
	}

	//Ahora los recorremos y los conectamos...
	for(auto& n : nodos_ruta)
	{
		for(const auto& on : nodos_ruta)
		{
			if(n.origen.id != on.origen.id)
			{
				if(visibilidad_entre_puntos(n.origen.pt, on.origen.pt))
				{
					n.conexiones.push_back(Nodo_ruta::conexion{on, n.origen.pt.distancia_hasta(on.origen.pt)});
				}
			}
		}

		//TODO: Quizás eliminar los duplicados...
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

void Mapa::inicializar()
{
	for(auto& g : generadores_items) g.reiniciar();
	construir_nodos_ruta();
}

	//TODO: Este algoritmo se puede mejorar añadiendo los dos nuevos puntos como
	//puntos de ruta temporales: volvemos a computar y luego los borramos. De esta
	//forma evitaríamos desde el punto de inicio al siguiente más cercano aunque luego
	//tengamos que dar la vuelta. Resolver el primer punto es sencillo: lo tratamos como
	//un punto de ruta normal sin que el resto se vinculen a él.
	//Resolver el último ya es más complicado sin reconstruir el mapa cada vez, puesto
	//que los nodos reales tienen que vincularse y al terminar habría que desvincularlos.
		
const Nodo_ruta * Mapa::localizar_nodo_cercano(Espaciable::tpunto pt) const
{
	const Nodo_ruta * res=nullptr;
	double dist=-1.0;
	for(const auto& n : nodos_ruta)
	{
		double d=pt.distancia_hasta(n.origen.pt);

		if((dist < 0.0 || d <= dist) && visibilidad_entre_puntos(pt, n.origen.pt))
		{
			dist=d;
			res=&n;
		}
	}

	return res;
}
