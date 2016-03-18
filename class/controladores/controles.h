#ifndef CONTROLADOR_CONTROLES_H
#define CONTROLADOR_CONTROLES_H

#include <map>
#include <string>

#include <def_video.h>
#include <herramientas/log_base/log_base.h>

#include "../framework/controlador_interface.h"
#include "../app/framework_impl/app_config.h"

namespace App
{

class Controlador_controles:
	public DFramework::Controlador_interface
{
	public:

					Controlador_controles(DLibH::Log_base&, App::App_config&);

	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	private:

	void				recomponer_str_controles();
	void				asignar_str_control_actual();
	void				finalizar_configuracion();

	struct tinput{
		enum tipos {registrar, disparo, arriba, abajo, izquierda, derecha};
					
		int 				jugador, tipo, clave;
		std::string 			nombre;
		DFramework::Input::Entrada 	entrada;
	};

	std::map<int, tinput>			controles;
	
	DLibH::Log_base&			log;
	App::App_config& 			config;

	DLibV::Fuente_TTF			fuente_akashi;
	std::string				str_controles, str_actual;
	int					kactual;

};

}

#endif