#ifndef LOGICA_EDITOR_H
#define LOGICA_EDITOR_H

#include <vector>

#include <def_video.h>

#include "logica_interface.h"

#include "struct_camara.h"
#include "mapa.h"
#include "trazador_ruta.h"

namespace App
{

//TODO: Move into another header...

class Objeto_editor
{
	public:

	virtual void		mover()=0;
	void			borrar() {para_borrar=true;}
	bool			es_borrar() const {return para_borrar;}
	
	private:

	bool			para_borrar=false;
};

class Obstaculo_editor:
	public Objeto_editor
{
	public:

	Obstaculo_editor(const Obstaculo& o):elemento(o) {}

	Obstaculo		elemento;
	void			mover() {};
	
};

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
	DLibH::Punto_2d<double>			punto_desde_pos_pantalla(int x, int y, bool=true);
	void					nuevo_punto(DLibH::Punto_2d<double>);
	void					cerrar_poligono();
	void					crear_punto_inicio(DLibH::Punto_2d<double>);
	void					crear_punto_ruta(DLibH::Punto_2d<double>);
	void					crear_punto_generador_items(DLibH::Punto_2d<double>);
	void					localizar_elementos_bajo_cursor();
	void					obtener_desde_mapa();
	void					aplicar_a_mapa();
	void					do_crazy_pathfind();

	Mapa&					mapa;

	//TODO: Añadir el resto si todo va bien :).
	std::vector<std::unique_ptr<Obstaculo_editor>>		obstaculos;

	//TODO: Usar un único gestor de fuentes...
	DLibV::Fuente_TTF			fuente_akashi;
	DFramework::Input::Posicion_raton	pos_raton;
	Struct_camara				struct_camara;
	Espaciable::tpoligono			poligono_construccion;
	int					grid;
	enum class tobjetocreado		{vertice, punto_ruta, inicio, arma} tobjeto;

	

	std::vector<DLibH::Punto_2d<double>>	ruta;
	std::vector<const Obstaculo *>		obstaculos_cursor;

};
}

#endif
