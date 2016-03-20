#ifndef STRUCT_CAMARA_H
#define STRUCT_CAMARA_H

namespace App
{
struct Struct_camara
{
	double  				zoom;
	int 					xcam, ycam;

	Struct_camara(double z, int x, int y):
		zoom(z), xcam(x), ycam(y)
	{}
};
}

#endif
