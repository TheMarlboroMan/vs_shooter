#include "kernel_driver_interface.h"
#include <source/string_utils.h>
#include <class/text_reader.h>

using namespace DFramework;

std::vector<std::string> Kernel_driver_interface::obtener_entradas_desde_ruta(const std::string& ruta) const
{
	tools::text_reader L(ruta, '#');
	std::vector<std::string> resultado;

	if(!L)
	{
		throw Excepcion_carga_recursos("Imposible cargar fichero de recursos en "+ruta);
	}

	while(true)
	{
		std::string linea=L.read_line();
		if(!L) break;
		resultado.push_back(linea);
	}

	return resultado;
}
