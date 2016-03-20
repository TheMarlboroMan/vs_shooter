#include "principal.h"

#include "../app/logica_editor.h"
#include "../app/logica_juego.h"

#include "estados_controladores.h"

#include "../app/framework_impl/input.h"

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log)
	:log(log), 
	tlogica(tlogicas::editor),
	logica_actual(nullptr),
	logica_juego(nullptr),
	logica_editor(nullptr)
{
	logica_juego.reset(new Logica_juego(mapa));
	logica_editor.reset(new Logica_editor(mapa));
	logica_actual=logica_editor.get();
}

void Controlador_principal::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_principal::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
	}
	else
	{
		if(input.es_input_down(Input::escape))
		{
			switch(tlogica)
			{
				case tlogicas::juego:
					cambiar_logica(tlogicas::editor); 
				break;
				case tlogicas::editor:
					abandonar_aplicacion();
				break;
			}
		}
		else if(input.es_input_down(Input::cambio_logica))
		{
			switch(tlogica)
			{
				case tlogicas::juego:
					cambiar_logica(tlogicas::editor); 
				break;
				case tlogicas::editor:
					cambiar_logica(tlogicas::juego); 
				break;
			}
		}

		logica_actual->loop(input, delta);
	}
}

void  Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	logica_actual->dibujar(pantalla);
}

void  Controlador_principal::despertar()
{
	log<<"Despertando controlador principal"<<std::endl;
}

void  Controlador_principal::dormir()
{
	log<<"Durmiendo controlador principal"<<std::endl;
}

bool Controlador_principal::es_posible_abandonar_estado() const
{
	return true;
}

void Controlador_principal::cambiar_logica(tlogicas l)
{
	if(logica_actual) logica_actual->finalizar();

	switch(l)
	{
		case tlogicas::juego:
			logica_actual=logica_juego.get();
		break;
		case tlogicas::editor:
			logica_actual=logica_editor.get();
		break;
	}

	logica_actual->iniciar();
	tlogica=l;
}
