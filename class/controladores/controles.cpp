#include "controles.h"

#include <vector>

#include "estados_controladores.h"
#include "../app/framework_impl/input.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Controlador_controles::Controlador_controles(DLibH::Log_base& log, App_config& c, const Fuentes& f)
	:log(log), config(c), 
	fuente_akashi(f.obtener_fuente("akashi", 16)), kactual(Input::j1_disparo)
{
	
}

void  Controlador_controles::preloop(DFramework::Input& input, float delta)
{
	if(!controles.size())
	{	
		//TODO: Muy mejorable.
		controles[Input::j1_disparo]={1, tinput::disparo, Input::j1_disparo, "P1: Fire", input.localizar_entrada(Input::j1_disparo)};
		controles[Input::j1_habilidad]={1, tinput::habilidad, Input::j1_habilidad, "P1: Special", input.localizar_entrada(Input::j1_habilidad)};
		controles[Input::j1_izquierda]={1, tinput::izquierda, Input::j1_izquierda, "P1: Left", input.localizar_entrada(Input::j1_izquierda)};
		controles[Input::j1_derecha]={1, tinput::derecha, Input::j1_derecha, "P1: Right", input.localizar_entrada(Input::j1_derecha)};
		controles[Input::j1_arriba]={1, tinput::arriba, Input::j1_arriba, "P1: Forward", input.localizar_entrada(Input::j1_arriba)};
		controles[Input::j1_abajo]={1, tinput::abajo, Input::j1_abajo, "P1: Backward", input.localizar_entrada(Input::j1_abajo)};

		controles[Input::j2_disparo]={2, tinput::disparo, Input::j2_disparo, "P2: Fire", input.localizar_entrada(Input::j2_disparo)};
		controles[Input::j2_habilidad]={2, tinput::habilidad, Input::j2_habilidad, "P2: Special", input.localizar_entrada(Input::j2_habilidad)};
		controles[Input::j2_izquierda]={2, tinput::izquierda, Input::j2_izquierda, "P2: Left", input.localizar_entrada(Input::j2_izquierda)};
		controles[Input::j2_derecha]={2, tinput::derecha, Input::j2_derecha, "P2: Right", input.localizar_entrada(Input::j2_derecha)};
		controles[Input::j2_arriba]={2, tinput::arriba, Input::j2_arriba, "P2: Forward", input.localizar_entrada(Input::j2_arriba)};
		controles[Input::j2_abajo]={2, tinput::abajo, Input::j2_abajo, "P2: Backward", input.localizar_entrada(Input::j2_abajo)};

		controles[Input::j3_disparo]={3, tinput::disparo, Input::j3_disparo, "P3: Fire", input.localizar_entrada(Input::j3_disparo)};
		controles[Input::j3_habilidad]={2, tinput::habilidad, Input::j3_habilidad, "P3: Special", input.localizar_entrada(Input::j3_habilidad)};
		controles[Input::j3_izquierda]={3, tinput::izquierda, Input::j3_izquierda, "P3: Left", input.localizar_entrada(Input::j3_izquierda)};
		controles[Input::j3_derecha]={3, tinput::derecha, Input::j3_derecha, "P3: Right", input.localizar_entrada(Input::j3_derecha)};
		controles[Input::j3_arriba]={3, tinput::arriba, Input::j3_arriba, "P3: Forward", input.localizar_entrada(Input::j3_arriba)};
		controles[Input::j3_abajo]={3, tinput::abajo, Input::j3_abajo, "P3: Backward", input.localizar_entrada(Input::j3_abajo)};

		recomponer_str_controles();
		asignar_str_control_actual();
	}
}

void Controlador_controles::asignar_str_control_actual()
{
	str_actual="** "+controles[kactual].nombre+" **";
}

void Controlador_controles::recomponer_str_controles()
{
	str_controles="";

	auto traducir=[](const DFramework::Input::Entrada& e)
	{
		std::string res="???";

		switch(e.tipo)
		{
			case DFramework::Input::Entrada::ttipo::nada: break;

			case DFramework::Input::Entrada::ttipo::teclado: 
			{
				res="Keyboard (";
				res+=SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)e.codigo));
				res+=")";
			}
			break;

			case DFramework::Input::Entrada::ttipo::raton: 
				res="Mouse button "+std::to_string(e.codigo);
			break;

			case DFramework::Input::Entrada::ttipo::joystick: 
				res="Joystick ["+std::to_string(e.dispositivo)+"] button "+std::to_string(e.codigo);
			break;
		}

		return res;
	};

	for(const auto& v : controles)
	{
		str_controles+=v.second.nombre+" : "+traducir(v.second.entrada)+"\n";
	}

	str_controles.pop_back();
}


void Controlador_controles::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
		return; 
	}

	if(input.es_input_down(Input::escape))
	{
		solicitar_cambio_estado(principal);
		return;
	}

	auto c=input.obtener_entrada();

	using E=DFramework::Input::Entrada;

	if(c.tipo!=E::ttipo::nada)
	{
		input.limpiar(kactual);

		controles[kactual].entrada=c;
		input.configurar(input.desde_entrada(c, kactual));

		++kactual;

		if(!controles.count(kactual))
		{
			finalizar_configuracion();
		}
		else
		{
			recomponer_str_controles();
			asignar_str_control_actual();
		}
	}
}

void  Controlador_controles::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_controles::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);

	DLibV::Representacion_TTF txt(fuente_akashi, {255, 255, 255, 255}, str_actual);
	txt.ir_a(16, 16);
	txt.volcar(pantalla);

	DLibV::Representacion_TTF txt2(fuente_akashi, {255, 255, 255, 255}, str_controles);
	txt2.ir_a(16, 64);
	txt2.volcar(pantalla);
}

void  Controlador_controles::despertar()
{
	log<<"Despertando controlador controles"<<std::endl;
}

void  Controlador_controles::dormir()
{
	log<<"Durmiendo controlador controles"<<std::endl;
}

bool Controlador_controles::es_posible_abandonar_estado() const
{
	return true;
}

void Controlador_controles::finalizar_configuracion()
{
	auto traducir_tipo=[](const DFramework::Input::Entrada::ttipo t)
	{
		if(t!=DFramework::Input::Entrada::ttipo::teclado) return 1;
		else return 0;
	};

	for(const auto& par : controles)
	{
		const auto& c=par.second;

		App_config::input_jugador val={traducir_tipo(c.entrada.tipo), c.entrada.dispositivo, c.entrada.codigo};

		switch(c.tipo)
		{
			case tinput::habilidad: 	config.mut_habilidad(c.jugador, val); break;
			case tinput::disparo: 		config.mut_disparo(c.jugador, val); break;
			case tinput::arriba:		config.mut_arriba(c.jugador, val); break;
			case tinput::abajo:		config.mut_abajo(c.jugador, val); break;
			case tinput::izquierda:		config.mut_izquierda(c.jugador, val); break;
			case tinput::derecha:		config.mut_derecha(c.jugador, val); break;
		}
	}

	config.grabar();
	solicitar_cambio_estado(principal);
}

