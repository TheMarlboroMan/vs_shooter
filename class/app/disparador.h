#ifndef DISPARADOR_H
#define DISPARADOR_H

#include <memory>
#include <vector>
#include <functional>

#include <tools/point_2d/point_2d.h>

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
		ldt::point_2d<double>			pt;
		double					angulo;
	};

	typedef std::vector<info>			v_info;
	v_info						disparos;
};

}

#endif
