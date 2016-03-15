#ifndef DISPARADOR_H
#define DISPARADOR_H

#include <memory>
#include <vector>
#include <functional>

#include <herramientas/punto_2d/punto_2d.h>

namespace App
{

class Proyectil;

class Disparador
{
	public:

	//Estructura con el proyectil y su información de preparación. Nos
	//evitamos en todos los constructores pasar parámetros y llamar a formar polígono,
	//que se deriva en otro lado.
	struct info
	{
		std::unique_ptr<Proyectil>	proyectil;
		Punto_2d<double>		pt;
		double				angulo;
	};


	typedef std::vector<info>		v_info;

			Disparador(std::function<void(v_info&)>);
	void		generar_proyectiles(v_info&) const;

	private:

	std::function<void(v_info&)>	funcion;
};

}

#endif
