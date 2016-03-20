#include "kernel_config.h"
#include <SDL2/SDL.h>
#include "input.h"

#include <iostream>

using namespace App;

Kernel_config::Kernel_config(const App_config& c)
	:config(c)
{

}

std::vector<std::string> Kernel_config::obtener_entradas_audio() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/audio.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_musica() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/musica.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_texturas() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/texturas.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_superficies() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/superficies.txt"));
}

DFramework::Info_ventana Kernel_config::obtener_info_ventana() const
{
	return DFramework::Info_ventana{600, 200, 600, 200, "2D TESTING...", true};
}

std::vector<DFramework::Par_input> Kernel_config::obtener_pares_input() const
{
	auto tipo_desde_config=[](int t)
	{
		using namespace DFramework;

		switch(t)
		{
			case 0: return Par_input::tipos::teclado; break;
			case 1: return Par_input::tipos::joystick; break;
		}

		return Par_input::tipos::teclado;
	};

	using namespace DFramework;
	std::vector<Par_input> res{
		Par_input{Par_input::tipos::teclado, Input::escape, SDL_SCANCODE_ESCAPE, 0},
		Par_input{Par_input::tipos::teclado, Input::tab, SDL_SCANCODE_TAB, 0},
		Par_input{Par_input::tipos::teclado, Input::TEST_VISIBILIDAD, SDL_SCANCODE_T, 0},
		Par_input{Par_input::tipos::teclado, Input::TEST_RUTA, SDL_SCANCODE_Y, 0},
		Par_input{Par_input::tipos::teclado, Input::cambio_logica, SDL_SCANCODE_F1, 0},
		Par_input{Par_input::tipos::teclado, Input::zoom_mas, SDL_SCANCODE_KP_PLUS, 0},
		Par_input{Par_input::tipos::teclado, Input::zoom_menos, SDL_SCANCODE_KP_MINUS, 0},
		Par_input{Par_input::tipos::teclado, Input::cargar_mapa, SDL_SCANCODE_C, 0},
		Par_input{Par_input::tipos::teclado, Input::grabar_mapa, SDL_SCANCODE_G, 0},
		Par_input{Par_input::tipos::raton, Input::click_i, SDL_BUTTON_LEFT, 0},
		Par_input{Par_input::tipos::raton, Input::click_d, SDL_BUTTON_RIGHT, 0}};

	auto obtener_info_jugador=[this](std::map<int, App_config::input_jugador>& res, int j, int idis, int iizq, int ider, int iarr, int iaba, int ireg)
	{
		res[idis]=config.acc_disparo(j);
		res[iizq]=config.acc_izquierda(j);
		res[ider]=config.acc_derecha(j);
		res[iarr]=config.acc_arriba(j);
		res[iaba]=config.acc_abajo(j);
		res[ireg]=config.acc_registrar(j);
	};

	std::map<int, App_config::input_jugador> mapa;
	obtener_info_jugador(mapa, 1, Input::j1_disparo, Input::j1_izquierda, Input::j1_derecha, Input::j1_arriba, Input::j1_abajo, Input::j1_registrar);
	obtener_info_jugador(mapa, 2, Input::j2_disparo, Input::j2_izquierda, Input::j2_derecha, Input::j2_arriba, Input::j2_abajo, Input::j2_registrar);
	obtener_info_jugador(mapa, 3, Input::j3_disparo, Input::j3_izquierda, Input::j3_derecha, Input::j3_arriba, Input::j3_abajo, Input::j3_registrar);

	for(const auto& p : mapa)
		res.push_back({tipo_desde_config(p.second.tipo), p.first, p.second.codigo, p.second.device});
	
	return res;
}
