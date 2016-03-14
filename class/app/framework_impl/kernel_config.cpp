#include "kernel_config.h"
#include <SDL2/SDL.h>
#include "input.h"

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
	using namespace DFramework;
	std::vector<Par_input> res{
		Par_input{Par_input::tipos::teclado, Input::escape, SDL_SCANCODE_ESCAPE},
		Par_input{Par_input::tipos::teclado, Input::zoom_mas, SDL_SCANCODE_KP_PLUS},
		Par_input{Par_input::tipos::teclado, Input::zoom_menos, SDL_SCANCODE_KP_MINUS},
		Par_input{Par_input::tipos::teclado, Input::registrar_j1, SDL_SCANCODE_1},
		Par_input{Par_input::tipos::teclado, Input::registrar_j2, SDL_SCANCODE_2},
		Par_input{Par_input::tipos::raton, Input::click_i, SDL_BUTTON_LEFT},
		Par_input{Par_input::tipos::raton, Input::click_d, SDL_BUTTON_RIGHT},

		Par_input{Par_input::tipos::teclado, Input::j1_disparo, config.acc_disparo(1)},
		Par_input{Par_input::tipos::teclado, Input::j1_izquierda, config.acc_izquierda(1)},
		Par_input{Par_input::tipos::teclado, Input::j1_derecha, config.acc_derecha(1)},
		Par_input{Par_input::tipos::teclado, Input::j1_arriba, config.acc_arriba(1)},
		Par_input{Par_input::tipos::teclado, Input::j1_abajo, config.acc_abajo(1)},

		Par_input{Par_input::tipos::teclado, Input::j2_disparo, config.acc_disparo(2)},
		Par_input{Par_input::tipos::teclado, Input::j2_izquierda, config.acc_izquierda(2)},
		Par_input{Par_input::tipos::teclado, Input::j2_derecha, config.acc_derecha(2)},
		Par_input{Par_input::tipos::teclado, Input::j2_arriba, config.acc_arriba(2)},
		Par_input{Par_input::tipos::teclado, Input::j2_abajo, config.acc_abajo(2)}
};
	return res;
}
