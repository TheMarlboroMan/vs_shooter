#ifndef WIDGET_EDITOR_INTERFACE_H
#define WIDGET_EDITOR_INTERFACE_H

#include <video/screen/screen.h>
#include "../framework/input.h"

namespace App
{

class Widget_editor_interface
{
	public:

	virtual void	inicializar(DFramework::Input&)=0;
	virtual void 	dibujar(ldv::screen&)=0;
	virtual void	input(DFramework::Input&, float)=0;
	virtual void	finalizar(DFramework::Input&)=0;
	virtual bool	es_cerrar() const=0;
};

}

#endif
