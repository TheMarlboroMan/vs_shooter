#ifndef LOGICA_EDITOR_H
#define LOGICA_EDITOR_H

#include <vector>
#include <algorithm>

#include <def_video.h>

#include "logica_interface.h"

#include "struct_camara.h"
#include "mapa.h"
#include "trazador_ruta.h"
#include "representador.h"

namespace App
{

//TODO: Move into another header...
class Objeto_editor
{
	public:

	typedef DLibH::Punto_2d<double> punto;

	virtual void		mover(double, double)=0;
	virtual void		dibujar(Representador&, DLibV::Pantalla&, const Struct_camara&, bool=true) const=0;
	virtual bool		es_bajo_cursor(punto) const=0;
	void			borrar() {para_borrar=true;}
	bool			es_borrar() const {return para_borrar;}
	
	static Espaciable::tpoligono 	cuadrado(double x, double y, int rad)
	{
		return Espaciable::tpoligono { {{x-rad, y-rad}, {x-rad, y+rad}, {x+rad, y+rad}, {x+rad, y-rad}}, {x, y}};
	}

	private:

	bool			para_borrar=false;
};

class Obstaculo_editor:
	public Objeto_editor
{
	public:

	Obstaculo_editor(const Obstaculo& o):elemento(o) {}

	Obstaculo		elemento;
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}	
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const Struct_camara& struct_camara, bool rellenar=true) const
	{
		r.dibujar_poligono(pantalla, elemento.acc_poligono(), rellenar ? elemento.acc_color() : tcolor{255, 255, 255, 255}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom, rellenar);
	}
	
};

class Punto_inicio_editor:
	public Objeto_editor
{
	public:

	Punto_inicio_editor(const Espaciable::tpunto& pi)
		:elemento(pi), poligono(cuadrado(pi.x, pi.y, 6))
	{
	}

	Espaciable::tpunto	elemento;
	virtual void		mover(double x, double y) 
	{
		elemento.x+=x; elemento.y+=y;
		poligono=cuadrado(elemento.x, elemento.y, 6);
	}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(poligono, pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const Struct_camara& struct_camara, bool rellenar=true) const
	{
		r.dibujar_poligono(pantalla, poligono, rellenar ? tcolor{255, 0, 0, 128} : tcolor{255, 255, 255, 255}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom, rellenar);
	}

	private:

	Espaciable::tpoligono	poligono;
};

class Generador_items_editor:
	public Objeto_editor
{
	public:

	Generador_items_editor(const Generador_items& gi):elemento(gi) {}

	Generador_items		elemento;
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const Struct_camara& struct_camara, bool rellenar=true) const
	{
		r.dibujar_poligono(pantalla, elemento.acc_poligono(), rellenar ? tcolor{0, 0, 255, 128} : tcolor{255, 255, 255, 255}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom, rellenar);
	}
};

class Punto_ruta_editor:
	public Objeto_editor
{
	public:

	Punto_ruta_editor(const Punto_ruta& pr)
		:elemento(pr), poligono(cuadrado(pr.pt.x, pr.pt.y, 6))
	{}

	Punto_ruta		elemento;
	virtual void		mover(double x, double y) 
	{
		elemento.pt.x+=x; elemento.pt.y+=y;
		poligono=cuadrado(elemento.pt.x, elemento.pt.y, 6);
	}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(poligono, pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const Struct_camara& struct_camara, bool rellenar=true) const
	{
		r.dibujar_poligono(pantalla, poligono, rellenar ? tcolor{255, 255, 25, 128} : tcolor{255,255,255,255}, struct_camara.xcam, struct_camara.ycam, struct_camara.zoom, rellenar);
	}

	private:

	Espaciable::tpoligono	poligono;
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
	void					crear();
	void					seleccionar();
	void					eliminar();
	template<typename T>			
	void					eliminar_helper(std::vector<T>& v)
	{
		v.erase(std::remove_if(std::begin(v), std::end(v), [](T& o){return o.es_borrar();}), std::end(v));
	}
	void					intercambiar_objeto_creado();
	DLibH::Punto_2d<double>			punto_desde_pos_pantalla(int x, int y, bool=true);
	void					nuevo_punto(DLibH::Punto_2d<double>);
	void					cerrar_poligono();
	void					crear_punto_inicio(DLibH::Punto_2d<double>);
	void					crear_punto_ruta(DLibH::Punto_2d<double>);
	void					crear_generador_items(DLibH::Punto_2d<double>);
	void					localizar_elementos_bajo_cursor();
	template<typename T>	
	void 					localizar_elementos_bajo_cursor_helper(T& t, DLibH::Punto_2d<double> pt)
	{
			for(auto& o : t) 
				if(o.es_bajo_cursor(pt))
					objetos_cursor.push_back(&o);
	}
	void					mover_seleccion(double, double);
	void					obtener_desde_mapa();
	void					aplicar_a_mapa();
	void					do_crazy_pathfind();

	Mapa&					mapa;

	std::vector<Obstaculo_editor>		obstaculos;
	std::vector<Punto_inicio_editor>	puntos_inicio;
	std::vector<Punto_ruta_editor>		puntos_ruta;
	std::vector<Generador_items_editor>	generadores_items;

	//TODO: Usar un único gestor de fuentes...
	DLibV::Fuente_TTF			fuente_akashi;
	DFramework::Input::Posicion_raton	pos_raton;
	Struct_camara				struct_camara;
	Espaciable::tpoligono			poligono_construccion;
	int					grid;
	bool					ver_conexiones;
	enum class tobjetocreado		{vertice, punto_ruta, inicio, arma} tobjeto;

	

	std::vector<DLibH::Punto_2d<double>>	ruta;
	std::vector<Objeto_editor *>		objetos_cursor;
	std::vector<Objeto_editor *>		objetos_seleccionados;

};
}

#endif
