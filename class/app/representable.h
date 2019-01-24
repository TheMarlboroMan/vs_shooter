#ifndef REPRESENTABLE_H
#define REPRESENTABLE_H

#include <video/screen/screen.h>
#include <video/camera/camera.h>
#include <video/resource_manager/resource_manager.h>

#include "representador.h"
#include "color.h"

namespace App
{

class Representable
{
	public:

	virtual void dibujar(Representador&, ldv::screen&, const ldv::camera&, const ldv::resource_manager&) const=0;
};

}

#endif
