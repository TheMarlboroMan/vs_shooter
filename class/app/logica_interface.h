#ifndef LOGICA_INTERFACE_H
#define LOGICA_INTERFACE_H

#include "../framework/input.h"
#include <video/pantalla/pantalla.h>

namespace App
{

class Logica_interface
{
	public:

	virtual void			iniciar()=0;
	virtual void			finalizar()=0;
	virtual void 			loop(DFramework::Input& input, float delta)=0;
	virtual void 			dibujar(DLibV::Pantalla& pantalla)=0;
};

}

#endif
