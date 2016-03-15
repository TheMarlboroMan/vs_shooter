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
		Par_input{Par_input::tipos::teclado, Input::zoom_mas, SDL_SCANCODE_KP_PLUS, 0},
		Par_input{Par_input::tipos::teclado, Input::zoom_menos, SDL_SCANCODE_KP_MINUS, 0},
		Par_input{Par_input::tipos::teclado, Input::registrar_j1, SDL_SCANCODE_1, 0},
		Par_input{Par_input::tipos::teclado, Input::registrar_j2, SDL_SCANCODE_2, 0},
		Par_input{Par_input::tipos::raton, Input::click_i, SDL_BUTTON_LEFT, 0},
		Par_input{Par_input::tipos::raton, Input::click_d, SDL_BUTTON_RIGHT, 0},

		Par_input{tipo_desde_config(config.acc_tipo_disparo(1)), 	Input::j1_disparo, config.acc_disparo(1), 0},
		Par_input{tipo_desde_config(config.acc_tipo_izquierda(1)), 	Input::j1_izquierda, config.acc_izquierda(1), 0},
		Par_input{tipo_desde_config(config.acc_tipo_derecha(1)), 	Input::j1_derecha, config.acc_derecha(1), 0},
		Par_input{tipo_desde_config(config.acc_tipo_arriba(1)), 	Input::j1_arriba, config.acc_arriba(1), 0},
		Par_input{tipo_desde_config(config.acc_tipo_abajo(1)), 		Input::j1_abajo, config.acc_abajo(1), 0},

		Par_input{tipo_desde_config(config.acc_tipo_disparo(2)), 	Input::j2_disparo, config.acc_disparo(2), 0},
		Par_input{tipo_desde_config(config.acc_tipo_izquierda(2)), 	Input::j2_izquierda, config.acc_izquierda(2), 0},
		Par_input{tipo_desde_config(config.acc_tipo_derecha(2)), 	Input::j2_derecha, config.acc_derecha(2), 0},
		Par_input{tipo_desde_config(config.acc_tipo_arriba(2)), 	Input::j2_arriba, config.acc_arriba(2), 0},
		Par_input{tipo_desde_config(config.acc_tipo_abajo(2)), 		Input::j2_abajo, config.acc_abajo(2), 0},
};
	return res;
}
