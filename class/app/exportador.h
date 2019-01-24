#ifndef EXPORTADOR_H
#define EXPORTADOR_H

#include <class/dnot_token.h>
#include "decoracion.h"
#include "obstaculo.h"
#include "punto_ruta.h"
#include "generador_items.h"

namespace App
{
class Exportador
{
	public:

	std::string serializar(const std::vector<Obstaculo>&, const std::vector<Decoracion>&, const std::vector<ldt::point_2d<double>>&, const std::vector<ldt::point_2d<double>>&, const std::vector<Punto_ruta>&, const std::vector<Generador_items>&);

	private:

	tools::dnot_token 		serializar_obstaculo(const Obstaculo&);
	tools::dnot_token 		serializar_decoracion(const Decoracion&);
	tools::dnot_token 		serializar_punto(ldt::point_2d<double>);
	tools::dnot_token 		serializar_generador(const Generador_items&);

	tools::dnot_token::t_vector	generar_punto(double x, double y);
	tools::dnot_token::t_vector	generar_color(int r, int g, int b, int a);
};
}
#endif
