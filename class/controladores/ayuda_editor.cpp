#include "ayuda_editor.h"

#include <fstream>

#include "../app/framework_impl/input.h"

using namespace App;

Controlador_ayuda_editor::Controlador_ayuda_editor(DLibH::Log_base& log, const Fuentes& fuentes)
	:log(log)
{
	layout.mapear_fuente("fuente", fuentes.obtener_fuente("akashi", 16));
}

void Controlador_ayuda_editor::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_ayuda_editor::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_ayuda_editor::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
	}
	else
	{
		if(input.es_input_down(Input::escape))
		{
			solicitar_cambio_estado(editor);
		}
/*
		else if(input.es_input_pulsado(Input::cursor_abajo))
		{
			--y;
			if(y < max_y) y=max_y;
		}
		else if(input.es_input_pulsado(Input::cursor_arriba))
		{
			++y;
			if(y > 0) y=0;
		}
		else if(input.es_input_pulsado(Input::cursor_derecha))
		{
			--x;
			if(x < max_x) x=max_x;
		}
		else if(input.es_input_pulsado(Input::cursor_izquierda))
		{
			++x;
			if(x > 0) x=0;
		}
*/
	}
}

void Controlador_ayuda_editor::dibujar(DLibV::Pantalla& pantalla)
{
	layout.volcar(pantalla);
}

void Controlador_ayuda_editor::despertar()
{
	layout.parsear("data/layout/ayuda_editor.dnot", "layout");

	std::ifstream fichero_ayuda("data/app/ayuda.txt");
	std::string l;

	if(!fichero_ayuda)
	{
		l="ERROR: No se localiza el fichero de ayuda...";
	}
	else
	{
		for(std::string cad; std::getline(fichero_ayuda, cad); )
			l+=cad+"\n";
	}

	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_ayuda"))->asignar(l);
}

void Controlador_ayuda_editor::dormir()
{
	layout.vaciar_vista();
}

bool Controlador_ayuda_editor::es_posible_abandonar_estado() const
{
	return true;
}
