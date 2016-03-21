#ifndef STRUCT_CAMARA_H
#define STRUCT_CAMARA_H

namespace App
{
struct Struct_camara
{
	double  				zoom,
	 					xcam, ycam;

	Struct_camara(double z, double x, double y):
		zoom(z), xcam(x), ycam(y)
	{}
};
}

#endif
