#ifndef IMPORTADOR_H
#define IMPORTADOR_H

#include <class/dnot_parser.h>
#include "obstaculo.h"
#include "decoracion.h"
#include "punto_ruta.h"
#include "generador_items.h"

namespace App
{
class Importador
{
	public:

	void 		importar(const std::string&, std::vector<Obstaculo>&, std::vector<Decoracion>&, std::vector<ldt::point_2d<double>>&, std::vector<ldt::point_2d<double>>&, std::vector<Punto_ruta>&, std::vector<Generador_items>&);

	private:

	void 		deserializar_obstaculo(const tools::dnot_token&, std::vector<Obstaculo>&);
	void 		deserializar_decoracion(const tools::dnot_token&, std::vector<Decoracion>&);
	void 		deserializar_punto(const tools::dnot_token&, std::vector<ldt::point_2d<double>>&);
	void 		deserializar_punto_ruta(const tools::dnot_token&, std::vector<Punto_ruta>&);
	void 		deserializar_generador(const tools::dnot_token&, std::vector<Generador_items>&);
};
}

#endif
