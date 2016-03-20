#ifndef LOGICA_EDITOR_H
#define LOGICA_EDITOR_H

#include <vector>

#include <def_video.h>

#include "logica_interface.h"

#include "struct_camara.h"
#include "mapa.h"

namespace App
{

class Logica_editor:
	public Logica_interface
{
	public:
	
						Logica_editor(Mapa&);
	virtual void				iniciar();
	virtual void				finalizar();
	virtual void 				loop(DFramework::Input& input, float delta);
	virtual void 				dibujar(DLibV::Pantalla& pantalla);

	private:

	void					cargar_mapa();
	void					grabar_mapa();
	void					click_izq();
	void					click_der();
	void					intercambiar_objeto_creado();
	DLibH::Punto_2d<double>			punto_desde_pos_pantalla(int x, int y);
	void					nuevo_punto(DLibH::Punto_2d<double>);
	void					cerrar_poligono();
	void					crear_punto_inicio(DLibH::Punto_2d<double>);
	void					crear_punto_ruta(DLibH::Punto_2d<double>);
	void					crear_punto_generador_items(DLibH::Punto_2d<double>);

	Mapa&					mapa;

	//TODO: Usar un único gestor de fuentes...
	DLibV::Fuente_TTF			fuente_akashi;
	DFramework::Input::Posicion_raton	pos_raton;
	Struct_camara				struct_camara;
	Espaciable::tpoligono			poligono_construccion;
	int					grid;
	enum class tobjetocreado		{vertice, punto_ruta, inicio, arma} tobjeto;

};
}

#endif
