#ifndef LOGICA_JUEGO_H
#define LOGICA_JUEGO_H

#include <vector>

#include <def_video.h>

#include "logica_interface.h"

#include "struct_camara.h"
#include "mapa.h"
#include "jugador.h"

//TODO: Experimental...
#include "bot.h"

#include "bloque_input.h"

//TODO: Quitar con factoría de disparos.
#include "proyectil.h"

#include "disparador.h"



namespace App
{
class Logica_juego:
	public Logica_interface
{
	public:
	
						Logica_juego(Mapa&);
	virtual void				iniciar();
	virtual void				finalizar();
	virtual void 				loop(DFramework::Input& input, float delta);
	virtual void 				dibujar(DLibV::Pantalla& pantalla);

	private:

	void					registrar_jugador(int);
	void					ajustar_camara();
	void					dibujar_info_jugador(DLibV::Pantalla&, const Jugador&);
	void					procesar_disparadores();
	Traduccion_input			obtener_traduccion_input(int) const;
	Bloque_input				obtener_bloque_input(DFramework::Input& input, const Traduccion_input&) const;

	Mapa&					mapa;

	DLibV::Fuente_TTF			fuente_akashi;
	Struct_camara				struct_camara;

	std::vector<Jugador>			jugadores;
	std::vector<std::unique_ptr<Proyectil>>	proyectiles;
	std::vector<Disparador>			disparadores;

	Bot					bot;
};
}

#endif
