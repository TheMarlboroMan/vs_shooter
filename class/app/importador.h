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

	void 		importar(const std::string&, std::vector<Obstaculo>&, std::vector<Decoracion>&, std::vector<DLibH::Punto_2d<double>>&, std::vector<DLibH::Punto_2d<double>>&, std::vector<Punto_ruta>&, std::vector<Generador_items>&);

	private:

	void 		deserializar_obstaculo(const Herramientas_proyecto::Dnot_token&, std::vector<Obstaculo>&);
	void 		deserializar_decoracion(const Herramientas_proyecto::Dnot_token&, std::vector<Decoracion>&);
	void 		deserializar_punto(const Herramientas_proyecto::Dnot_token&, std::vector<DLibH::Punto_2d<double>>&);
	void 		deserializar_punto_ruta(const Herramientas_proyecto::Dnot_token&, std::vector<Punto_ruta>&);
	void 		deserializar_generador(const Herramientas_proyecto::Dnot_token&, std::vector<Generador_items>&);
};
}

#endif
