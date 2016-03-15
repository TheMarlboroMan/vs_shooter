#include "disparador.h"

using namespace App;

Disparador::Disparador(std::function<void(v_info&)> func)
	:funcion(func)
{

}

void Disparador::generar_proyectiles(v_info& v) const
{
	funcion(v);
}
