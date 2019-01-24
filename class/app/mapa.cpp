#include "mapa.h"

#include <algorithm>

using namespace App;

void Mapa::limpiar()
{
	obstaculos.clear();
	decoraciones.clear();
	puntos_inicio.clear();
	puntos_bot.clear();
	generadores_items.clear();
	puntos_ruta.clear();
	nodos_ruta.clear();
	decoraciones_fondo.clear();
	decoraciones_frente.clear();
}

bool Mapa::visibilidad_entre_puntos(Espaciable::tpunto pt1, Espaciable::tpunto pt2) const
{
	ldt::segment_2d<double> seg={ {pt1.x, pt1.y}, {pt2.x, pt2.y}};

	for(const auto& o : obstaculos)
	{
		if(intersection_segment_polygon(seg, o.acc_poligono()))
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
				auto pta=n.origen.pt, ptb=on.origen.pt;

				if(visibilidad_entre_puntos(pta, ptb) && comprobacion_ancho(pta, ptb, 12.0) )
				{
					n.conexiones.push_back(Nodo_ruta::conexion{on, n.origen.pt.distance_to(on.origen.pt)});
				}
			}
		}

		//TODO: Quizás eliminar los duplicados...
	}
}

void Mapa::inicializar()
{
	for(auto& g : generadores_items) g.reiniciar();
	construir_nodos_ruta();

	for(const auto& d : decoraciones)
	{
		if(d.es_frente()) decoraciones_frente.push_back(&d);
		else decoraciones_fondo.push_back(&d);
	}

	std::sort(std::begin(decoraciones_frente), std::end(decoraciones_frente), [](const Decoracion * a, const Decoracion * b) {return *a < *b;});
	std::sort(std::begin(decoraciones_fondo), std::end(decoraciones_fondo), [](const Decoracion * a, const Decoracion * b) {return *a < *b;});
}

const Nodo_ruta * Mapa::localizar_nodo_cercano(Espaciable::tpunto pt) const
{
	const Nodo_ruta * res=nullptr;
	double dist=-1.0;
	for(const auto& n : nodos_ruta)
	{
		double d=pt.distance_to(n.origen.pt);

		if((dist < 0.0 || d <= dist) && visibilidad_entre_puntos(pt, n.origen.pt) && comprobacion_ancho(pt, n.origen.pt, 12.0))
		{
			dist=d;
			res=&n;
		}
	}

	return res;
}

//La razón para recibir directamente un punto de ruta es porque el nodo necesita
//una REFERENCIA a un punto de ruta. No podemos crearlo aquí y dejarlo
//apuntando a memoria inválida.
Nodo_ruta Mapa::crear_inicio_temporal(Punto_ruta& pr) const
{
	Nodo_ruta nr{pr};

	for(const auto& n : nodos_ruta)
	{
		auto pta=pr.pt, ptb=n.origen.pt;

		if(visibilidad_entre_puntos(pta, ptb) && comprobacion_ancho(pta, ptb, 12.0))
		{
			nr.conexiones.push_back(Nodo_ruta::conexion{n, n.origen.pt.distance_to(n.origen.pt)});
		}
	}

	return nr;
}

bool Mapa::comprobacion_ancho(Espaciable::tpunto pta, Espaciable::tpunto ptb, double ancho) const
{
	auto pt_mas_vector=[](ldt::point_2d<double> pt, ldt::vector_2d<double> v, double fac)
	{
		auto res=pt;
		res.x+=v.x * fac;
		res.y+=v.y * fac;
		return res;
	};

	//Coger el vector entre los puntos, desplazar perpendicularmente, probar más posiciones.
	auto v=vector_from_points(pta, ptb).right_normal();

	auto ptc=pt_mas_vector(pta, v, ancho), ptd=pt_mas_vector(ptb, v, ancho);
	if(!visibilidad_entre_puntos(ptc, ptd)) return false;

	auto pte=pt_mas_vector(pta, v, -ancho), ptf=pt_mas_vector(ptb, v, -ancho);
	if(!visibilidad_entre_puntos(pte, ptf)) return false;

	return true;
}
