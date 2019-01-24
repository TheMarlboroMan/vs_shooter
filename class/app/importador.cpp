#include "importador.h"

using namespace App;

void Importador::importar(const std::string& fichero,
	std::vector<Obstaculo>& obstaculos,
	std::vector<Decoracion>& decoraciones,
	std::vector<ldt::point_2d<double>>& inicios,
	std::vector<ldt::point_2d<double>>& bots,
	std::vector<Punto_ruta>& puntos_ruta,
	std::vector<Generador_items>& generadores)
{
	using namespace tools;

	auto raiz=dnot_parse(fichero);

	try
	{
		auto& array_geometria=raiz["geometria"].get_vector();
		for(const auto& i : array_geometria) deserializar_obstaculo(i, obstaculos);

		auto& array_inicios=raiz["inicios"].get_vector();
		for(const auto& i : array_inicios) deserializar_punto(i, inicios);

		auto& array_puntos_ruta=raiz["puntos_ruta"].get_vector();
		for(const auto& i : array_puntos_ruta) deserializar_punto_ruta(i, puntos_ruta);

		auto& array_generadores=raiz["generadores"].get_vector();
		for(const auto& i : array_generadores) deserializar_generador(i, generadores);

		auto& array_bots=raiz["bots"].get_vector();
		for(const auto& i : array_bots) deserializar_punto(i, bots);

		auto& array_decoraciones=raiz["decoracion"].get_vector();
		for(const auto& i : array_decoraciones) deserializar_decoracion(i, decoraciones);
	}
	catch(std::exception &e)
	{
		//Simplemente por compatibilidad...
	}
}

void Importador::deserializar_obstaculo(const tools::dnot_token& tok, std::vector<Obstaculo>& obstaculos)
{
	Espaciable::tpoligono poligono;
	Obstaculo::ttipos tipo=Obstaculo::ttipos::normal;

	try
	{
		const auto& lista_puntos=tok["p"].get_vector();
		for(const auto& vp : lista_puntos)
		{
			const auto& pt=vp.get_vector();
			poligono.add_vertex({pt[0], pt[1]});
		}

		const auto& centro=tok["cen"].get_vector();
		poligono.set_rotation_center({centro[0], centro[1]});

		int val_tipo=tok["pr"]["t"];
		switch(val_tipo)
		{
			case 0:
			default:
				tipo=Obstaculo::ttipos::normal;
			break;
			case 1:
				tipo=Obstaculo::ttipos::letal;
			break;
			case 2:
				tipo=Obstaculo::ttipos::inocuo;
			break;
		}
	}
	catch(std::exception& e)
	{
		//Simplemente por compatibilidad...
	}

	obstaculos.push_back({poligono, tipo});
}

void Importador::deserializar_decoracion(const tools::dnot_token& tok, std::vector<Decoracion>& decoraciones)
{
	Espaciable::tpoligono poligono;

	const auto& lista_puntos=tok["p"].get_vector();
	for(const auto& vp : lista_puntos)
	{
		const auto& pt=vp.get_vector();
		poligono.add_vertex({pt[0], pt[1]});
	}

	const auto& centro=tok["cen"].get_vector();
	poligono.set_rotation_center({centro[0], centro[1]});

	tcolor color={128, 128, 128, 255};
	tcolor clinea=color;
	bool frente=false;
	int profundidad=100;

	try
	{
		const auto& colores=tok["col"].get_vector();
		const auto& vcolor=colores[0].get_vector();
		const auto& vcolorl=colores[1].get_vector();

		color={vcolor[0], vcolor[1], vcolor[2], vcolor[3]};
		clinea={vcolorl[0], vcolorl[1], vcolorl[2], vcolorl[3]};
		frente=tok["pr"]["fr"];
		profundidad=tok["pr"]["pr"];
	}
	catch(std::exception& e)
	{
		//No hacer nada... Tenemos esto aquí sólo para que no explote
		//seǵun vamos guardando más propiedades en el objeto.
	}

	decoraciones.push_back({poligono, color, clinea, frente, profundidad});
}

void Importador::deserializar_punto(const tools::dnot_token& tok, std::vector<ldt::point_2d<double>>& puntos)
{
	const auto& pt=tok.get_vector();
	puntos.push_back({pt[0], pt[1]});
}

void Importador::deserializar_punto_ruta(const tools::dnot_token& tok, std::vector<Punto_ruta>& puntos)
{
	const auto& pt=tok.get_vector();
	puntos.push_back(Punto_ruta({pt[0], pt[1]}));
}

void Importador::deserializar_generador(const tools::dnot_token& tok, std::vector<Generador_items>& generadores)
{
	const auto& pt=tok.get_vector();
	generadores.push_back(Generador_items({pt[0], pt[1]}) );
}
