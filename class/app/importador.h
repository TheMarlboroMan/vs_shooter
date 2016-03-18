#ifndef IMPORTADOR_H
#define IMPORTADOR_H

#include <class/dnot_parser.h>
#include "obstaculo.h"
#include "generador_items.h"

namespace App
{
class Importador
{
	public:

	void 		importar(const std::string&, std::vector<Obstaculo>&, std::vector<DLibH::Punto_2d<double>>&, std::vector<Generador_items>&);

	private:

	void 		deserializar_obstaculo(const Herramientas_proyecto::Dnot_token&, std::vector<Obstaculo>&);
	void 		deserializar_inicio(const Herramientas_proyecto::Dnot_token&, std::vector<DLibH::Punto_2d<double>>&);
	void 		deserializar_generador(const Herramientas_proyecto::Dnot_token&, std::vector<Generador_items>&);
};
}

#endif
