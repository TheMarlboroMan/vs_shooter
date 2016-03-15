#ifndef CONTROLADOR_PRINCIPAL_H
#define CONTROLADOR_PRINCIPAL_H

#include <vector>
#include <memory>

#include <def_video.h>
#include <herramientas/log_base/log_base.h>

#include <class/generador_numeros.h>

#include "../app/obstaculo.h"
#include "../app/jugador.h"
#include "../app/jugador_arma_alt.h"
#include "../app/jugador_arma_normal.h"
#include "../app/proyectil.h"
#include "../app/bloque_input.h"
#include "../app/representador.h"
#include "../app/disparador.h"
#include "../framework/controlador_interface.h"

namespace App
{

class Controlador_principal:
	public DFramework::Controlador_interface
{
	public:

					Controlador_principal(DLibH::Log_base&);

	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	void				registrar_jugador(int);

	private:

	//TODO: Esto es sólo para prototipo.
	DLibH::Punto_2d<double>			punto_desde_pos_pantalla(int x, int y);
	void					nuevo_punto(DLibH::Punto_2d<double>);
	void					cerrar_poligono();
	void					ajustar_camara();
	void					dibujar_info_jugador(DLibV::Pantalla&, const Jugador&);
	void					crear_punto_inicio(DLibH::Punto_2d<double>);
	void					procesar_disparadores();

	Traduccion_input			obtener_traduccion_input(int) const;
	Bloque_input				obtener_bloque_input(DFramework::Input& input, const Traduccion_input&) const;

	std::vector<Jugador>			jugadores;
	std::vector<Obstaculo>			obstaculos;
	std::vector<std::unique_ptr<Proyectil>>	proyectiles;
	std::vector<DLibH::Punto_2d<double>>	puntos_inicio;
	std::vector<Disparador>			disparadores;

	DLibH::Log_base&			log;

	DLibV::Fuente_TTF			fuente_akashi;
	DFramework::Input::Posicion_raton	pos_raton;

	double  				zoom;
	int 					xcam, ycam;
	//TODO: Esto es sólo para prototipo.
	Espaciable::tpoligono			poligono_construccion;
	int					grid;
};

}

#endif
