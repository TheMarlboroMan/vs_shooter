#ifndef DISPARADOR_H
#define DISPARADOR_H

#include <memory>
#include <vector>
#include <functional>

#include <herramientas/punto_2d/punto_2d.h>

namespace App
{

class Proyectil;

struct Disparador
{
	enum class tproyectiles {normal, peq, explosivo, divide};

	struct info
	{
		tproyectiles				tipo;
		int					indice;
		Punto_2d<double>			pt;
		double					angulo;
	};
	
	typedef std::vector<info>			v_info;
	v_info						disparos;
};

}

#endif
