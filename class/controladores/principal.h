#ifndef CONTROLADOR_PRINCIPAL_H
#define CONTROLADOR_PRINCIPAL_H

#include <vector>
#include <memory>

#include <def_video.h>

#include <herramientas/log_base/log_base.h>

#include "../app/fuentes.h"
#include "../app/mapa.h"
#include "../app/jugador.h"
#include "../app/jugador_info.h"

//TODO: Experimental...
#include "../app/bot.h"

#include "../app/bloque_input.h"

//TODO: Quitar con factoría de disparos.
#include "../app/proyectil.h"

#include "../app/disparador.h"

#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

namespace App
{

class Controlador_principal:
	public DFramework::Controlador_interface
{
	public:

					Controlador_principal(DLibH::Log_base&, const Fuentes& f);

	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(ldv::screen& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	Mapa&				acc_mapa() {return mapa;}

	private:

	void					registrar_info_jugadores();
	void					registrar_jugador(int);
	void					ajustar_camara();
	void					dibujar_info_jugador(ldv::screen&, const Jugador&);
	void					procesar_proyectiles(float);
	void					procesar_bots(float);
	void					procesar_jugadores(DFramework::Input&, float);
	void					procesar_disparadores();
	void					registrar_bot();
	void					pre_eliminar_jugador(const Jugador& j);
	Bloque_input				obtener_bloque_input(DFramework::Input& input, const Traduccion_input&) const;

	DLibH::Log_base&			log;
	const DLibV::Fuente_TTF&		fuente_akashi;

	ldv::camera				camara;
	Mapa					mapa;

	std::vector<Jugador_info>		info_jugadores;
	std::vector<Jugador>			jugadores;
	std::vector<Bot>			bots;
	std::vector<std::unique_ptr<Proyectil>>	proyectiles;
	std::vector<Disparador>			disparadores;
};

}

#endif
