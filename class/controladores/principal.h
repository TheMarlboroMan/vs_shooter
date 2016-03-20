#ifndef CONTROLADOR_PRINCIPAL_H
#define CONTROLADOR_PRINCIPAL_H

#include <vector>
#include <memory>

#include <herramientas/log_base/log_base.h>

#include "../app/logica_interface.h"

#include "../app/mapa.h"

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

	private:

	enum class tlogicas{editor, juego};


	void					cambiar_logica(tlogicas);

	Mapa					mapa;

	DLibH::Log_base&			log;

	tlogicas				tlogica;
	Logica_interface *			logica_actual;
	std::unique_ptr<Logica_interface>	logica_juego,
						logica_editor;
};

}

#endif
