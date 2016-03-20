#ifndef EXPORTADOR_H
#define EXPORTADOR_H

#include <class/dnot_token.h>
#include "obstaculo.h"
#include "punto_ruta.h"
#include "generador_items.h"

namespace App
{
class Exportador
{
	public:

	std::string serializar(const std::vector<Obstaculo>&, const std::vector<DLibH::Punto_2d<double>>&, const std::vector<Punto_ruta>&, const std::vector<Generador_items>&);

	private:

	Herramientas_proyecto::Dnot_token 		serializar_obstaculo(const Obstaculo&);
	Herramientas_proyecto::Dnot_token 		serializar_punto(DLibH::Punto_2d<double>);
	Herramientas_proyecto::Dnot_token 		serializar_generador(const Generador_items&);

	Herramientas_proyecto::Dnot_token::t_vector	generar_punto(double x, double y);
	Herramientas_proyecto::Dnot_token::t_vector	generar_color(int r, int g, int b, int a);
};
}
#endif
