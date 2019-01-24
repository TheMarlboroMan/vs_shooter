#ifndef CONTROLADOR_EDITOR_H
#define CONTROLADOR_EDITOR_H

#include <vector>
#include <memory>
#include <algorithm>

#include <def_video.h>

#include "../app/mapa.h"
#include "../app/mensajes.h"
#include "../app/trazador_ruta.h"
#include "../app/representador.h"
#include "../app/fuentes.h"
#include "../app/logica_editor_objetos.h"

#include "../app/widget_editor_interface.h"

#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

namespace App
{

class Controlador_editor:
	public DFramework::Controlador_interface
{
	public:

					Controlador_editor(DLibH::Log_base&, const Fuentes&);

	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(ldv::screen& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	void					aplicar_a_mapa(Mapa&);

	private:

	struct {
		Obstaculo::ttipos	tipo;
	}info_obstaculo;

	struct{
		tcolor			relleno,
					linea;
		int			orden;
		bool			frente;
	}info_decoracion;

	void					cargar_mapa();
	void					grabar_mapa();
	void					crear();
	void					seleccionar();
	void					eliminar();
	template<typename T>			
	void					eliminar_helper(std::vector<T>& v)
	{
		v.erase(std::remove_if(std::begin(v), std::end(v), [](T& o){return o.es_borrar();}), std::end(v));
	}
	void					intercambiar_objeto_creado();
	void					cambiar_grid(int);
	ldt::point_2d<double>			punto_desde_pos_pantalla(int x, int y, bool=true);
	void					nuevo_punto(ldt::point_2d<double>);
	void					cerrar_poligono();
	void					crear_punto_inicio(ldt::point_2d<double>);
	void 					crear_punto_bot(ldt::point_2d<double>);
	void					crear_punto_ruta(ldt::point_2d<double>);
	void					crear_generador_items(ldt::point_2d<double>);
	void					localizar_elementos_bajo_cursor();
	template<typename T, typename TV>	
	void 					localizar_elementos_bajo_cursor_helper(T& t, TV& v, ldt::point_2d<double> pt)
	{
			for(auto& o : t) 
				if(o.es_bajo_cursor(pt))
					v.push_back(&o);
	}

	void					mover_seleccion(double, double);
	void					obtener_desde_mapa();
	void					copiar();
	void					pegar();
	void					intercambiar_visibilidad(int, const std::string&);
	void					cambiar_profundidad(int);
	void					reordenar_decoraciones();
	void					click_derecho();

	DLibH::Log_base&			log;
	const ldv::ttf_font&		fuente_akashi;
	const ldv::ttf_font&		fuente_akashi_mensajes;

	ldv::camera				camara;
	Mapa					mapa;
	Widget_mensajes				mensajes;

	std::unique_ptr<Widget_editor_interface>	widget;
	std::vector<Obstaculo_editor>		obstaculos;
	std::vector<Decoracion_editor>		decoraciones;
	std::vector<Punto_inicio_editor>	puntos_inicio;
	std::vector<Punto_bot_editor>		puntos_bot;
	std::vector<Punto_ruta_editor>		puntos_ruta;
	std::vector<Generador_items_editor>	generadores_items;

	DFramework::Input::Posicion_raton	pos_raton;
	Espaciable::tpoligono			poligono_construccion;
	tcolor					color_relleno,
						color_linea;
	int					grid;
	enum flags_ver{fvdeco_frente=1, fvdeco_fondo=2, fvobstaculos=4, fvconexiones=8};
	int					ver_flags;
	enum class tobjetocreado		{obstaculo, decoracion, punto_ruta, inicio, bot, arma} tobjeto;

	std::vector<Objeto_editor *>		objetos_cursor;
	std::vector<Objeto_editor *>		objetos_seleccionados;
	bool					decoracion_frente;
};

}

#endif
