#ifndef FUENTES_H
#define FUENTES_H

#include <map>
#include <stdexcept>

#include <iostream>

#include <video/ttf_font/ttf_font.h>

namespace App
{

struct info_fuente
{
	std::string nombre;
	int tam;
	bool operator<(const info_fuente& o) const
	{
		if(nombre < o.nombre) return false;
		else if(nombre > o.nombre) return true;
		else return tam < o.tam;
	}
};

class Fuentes
{
	public:

	const ldv::ttf_font&					obtener_fuente(const std::string, int) const;
	void									registrar_fuente(const std::string, int);

	private:

	std::map<info_fuente, ldv::ttf_font>	fuentes;
};

}

#endif
