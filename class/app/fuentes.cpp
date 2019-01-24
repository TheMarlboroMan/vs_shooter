#include "fuentes.h"

using namespace App;

const ldv::ttf_font& Fuentes::obtener_fuente(const std::string f, int t) const
{
	if(!fuentes.count({f, t}))
	{
		throw std::runtime_error("La fuente "+f+" no está registrada en el tamaño solicitado");
	}

	return fuentes.at({f,t});
}

void Fuentes::registrar_fuente(const std::string f, int t)
{
	if(!fuentes.count({f, t}))
	{
		fuentes.insert( std::pair<info_fuente, ldv::ttf_font>({f, t}, ldv::ttf_font("data/fuentes/"+f+".ttf", t) ) );
	}
}
