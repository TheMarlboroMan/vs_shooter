#include "importador.h"

using namespace App;

void Importador::importar(const std::string& fichero, 
	std::vector<Obstaculo>& obstaculos, 
	std::vector<DLibH::Punto_2d<double>>& inicios,
	std::vector<DLibH::Punto_2d<double>>& bots,  
	std::vector<Punto_ruta>& puntos_ruta, 
	std::vector<Generador_items>& generadores)
{
	using namespace Herramientas_proyecto;

	auto raiz=parsear_dnot(fichero);

	try
	{
		auto& array_geometria=raiz["geometria"].acc_lista();
		for(const auto& i : array_geometria) deserializar_obstaculo(i, obstaculos);

		auto& array_inicios=raiz["inicios"].acc_lista();
		for(const auto& i : array_inicios) deserializar_punto(i, inicios);

		auto& array_puntos_ruta=raiz["puntos_ruta"].acc_lista();
		for(const auto& i : array_puntos_ruta) deserializar_punto_ruta(i, puntos_ruta);

		auto& array_generadores=raiz["generadores"].acc_lista();
		for(const auto& i : array_generadores) deserializar_generador(i, generadores);

		auto& array_bots=raiz["bots"].acc_lista();
		for(const auto& i : array_bots) deserializar_punto(i, bots);
	}
	catch(std::exception &e)
	{
		//Simplemente por compatibilidad...
	}
}

void Importador::deserializar_obstaculo(const Herramientas_proyecto::Dnot_token& tok, std::vector<Obstaculo>& obstaculos)
{
	Espaciable::tpoligono poligono;

	const auto& lista_puntos=tok["p"].acc_lista();
	for(const auto& vp : lista_puntos)
	{
		const auto& pt=vp.acc_lista();
		poligono.insertar_vertice({pt[0], pt[1]});
	}
	
	const auto& centro=tok["cen"].acc_lista();
	poligono.mut_centro({centro[0], centro[1]});

	tcolor color={128, 128, 128, 255};
	tcolor clinea=color;

	try
	{
		const auto& colores=tok["col"].acc_lista();
		const auto& vcolor=colores[0].acc_lista();
		const auto& vcolorl=colores[1].acc_lista();

		color={vcolor[0], vcolor[1], vcolor[2], vcolor[3]};
		clinea={vcolorl[0], vcolorl[1], vcolorl[2], vcolorl[3]};

	}
	catch(std::exception& e)
	{
		//Do nothing... This is only for map compatibility sake.	
	}

	obstaculos.push_back({poligono, color, clinea});
}

void Importador::deserializar_punto(const Herramientas_proyecto::Dnot_token& tok, std::vector<DLibH::Punto_2d<double>>& puntos)
{
	const auto& pt=tok.acc_lista();
	puntos.push_back({pt[0], pt[1]});
}

void Importador::deserializar_punto_ruta(const Herramientas_proyecto::Dnot_token& tok, std::vector<Punto_ruta>& puntos)
{
	const auto& pt=tok.acc_lista();
	puntos.push_back(Punto_ruta({pt[0], pt[1]}));
}

void Importador::deserializar_generador(const Herramientas_proyecto::Dnot_token& tok, std::vector<Generador_items>& generadores)
{
	const auto& pt=tok.acc_lista();
	generadores.push_back(Generador_items({pt[0], pt[1]}) );
}
