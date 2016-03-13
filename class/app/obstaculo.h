#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "color.h"
#include "espaciable.h"

namespace App
{

class Obstaculo:
	public Espaciable
{
	public:

					Obstaculo(const tpoligono&, tcolor);
	tcolor				acc_color() const {return color;}

	private:

	tcolor				color;
};

}

#endif
