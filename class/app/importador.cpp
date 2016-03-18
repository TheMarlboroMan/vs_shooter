#include "importador.h"

using namespace App;

void Importador::importar(const std::string& fichero, std::vector<Obstaculo>& obstaculos, std::vector<DLibH::Punto_2d<double>>& inicios, std::vector<Generador_items>& generadores)
{
	using namespace Herramientas_proyecto;

	auto raiz=parsear_dnot(fichero);

	auto& array_geometria=raiz["geometria"].acc_lista();
	for(const auto& i : array_geometria) deserializar_obstaculo(i, obstaculos);

	auto& array_inicios=raiz["inicios"].acc_lista();
	for(const auto& i : array_inicios) deserializar_inicio(i, inicios);

	auto& array_generadores=raiz["generadores"].acc_lista();
	for(const auto& i : array_generadores) deserializar_generador(i, generadores);
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

	const auto& vcolor=tok["col"].acc_lista();
	tcolor color{vcolor[0], vcolor[1], vcolor[2], vcolor[3]};
	obstaculos.push_back({poligono, color});
}

void Importador::deserializar_inicio(const Herramientas_proyecto::Dnot_token& tok, std::vector<DLibH::Punto_2d<double>>& puntos)
{
	const auto& pt=tok.acc_lista();
	puntos.push_back({pt[0], pt[1]});
}

void Importador::deserializar_generador(const Herramientas_proyecto::Dnot_token& tok, std::vector<Generador_items>& generadores)
{
	const auto& pt=tok.acc_lista();
	generadores.push_back(Generador_items({pt[0], pt[1]}) );
}
