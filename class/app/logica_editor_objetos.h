#ifndef LOGICA_EDITOR_OBJETOS
#define LOGICA_EDITOR_OBJETOS

namespace App
{

class Objeto_editor
{
	public:

	typedef DLibH::Punto_2d<double> punto;

	virtual void		mover(double, double)=0;
	virtual void		colorear(const tcolor&, const tcolor&)=0;
	virtual void		dibujar(Representador&, DLibV::Pantalla&, const Struct_camara&, bool=false) const=0;
	virtual bool		es_bajo_cursor(punto) const=0;

	void			borrar() {para_borrar=true;}
	bool			es_borrar() const {return para_borrar;}
	
	static Espaciable::tpoligono 	cuadrado(double x, double y, int rad)
	{
		return Espaciable::tpoligono { {{x-rad, y-rad}, {x-rad, y+rad}, {x+rad, y+rad}, {x+rad, y-rad}}, {x, y}};
	}

	static const tcolor	color_punto_ruta_editor;
	static const tcolor	color_generador_items_editor;
	static const tcolor 	color_punto_inicio_editor;
	static const tcolor	color_punto_bot_editor;
	static const tcolor	color_seleccion;

	protected:

	static void 		dibujar_poligono(
			Representador& r,DLibV::Pantalla& pantalla, 
			const DLibH::Poligono_2d<double> poligono, 
			const tcolor& color_fondo,
			const tcolor& color_linea,
			const Struct_camara& struct_camara, 
			bool seleccionado)
	{
		if(seleccionado)
		{
			r.dibujar_poligono(pantalla, poligono, color_fondo, struct_camara);
			r.dibujar_poligono_lineas(pantalla, poligono, color_seleccion, struct_camara);
		}
		else
		{
			if(color_fondo==color_linea)
			{
				r.dibujar_poligono(pantalla, poligono, color_fondo, struct_camara);
			}
			else
			{
				r.dibujar_poligono(pantalla, poligono, color_fondo, struct_camara);
				r.dibujar_poligono_lineas(pantalla, poligono, color_linea, struct_camara);
			}
		}
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
	virtual void		colorear(const tcolor& f, const tcolor& l)
	{
		elemento.mut_color(f);
		elemento.mut_color_linea(l);
	}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}	
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const Struct_camara& struct_camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, elemento.acc_poligono(), elemento.acc_color(), elemento.acc_color_linea(), struct_camara, seleccionado);
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
	virtual void		colorear(const tcolor&, const tcolor&) {}
	virtual void		mover(double x, double y) 
	{
		elemento.x+=x; elemento.y+=y;
		poligono=cuadrado(elemento.x, elemento.y, 6);
	}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(poligono, pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const Struct_camara& struct_camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, poligono, color_punto_inicio_editor, color_punto_inicio_editor, struct_camara, seleccionado);
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
	virtual void		colorear(const tcolor&, const tcolor&) {}
	virtual void		mover(double x, double y) {elemento.mover(x, y);}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(elemento.acc_poligono(), pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const Struct_camara& struct_camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, elemento.acc_poligono(), color_generador_items_editor, color_generador_items_editor, struct_camara, seleccionado);
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
	virtual void		colorear(const tcolor&, const tcolor&) {}
	virtual void		mover(double x, double y) 
	{
		elemento.pt.x+=x; elemento.pt.y+=y;
		poligono=cuadrado(elemento.pt.x, elemento.pt.y, 6);
	}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(poligono, pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const Struct_camara& struct_camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, poligono, color_punto_ruta_editor, color_punto_ruta_editor, struct_camara, seleccionado);
	}

	private:

	Espaciable::tpoligono	poligono;
};

class Punto_bot_editor:
	public Objeto_editor
{
	public:

	Punto_bot_editor(const Espaciable::tpunto& pi)
		:elemento(pi), poligono(cuadrado(pi.x, pi.y, 6))
	{
	}

	Espaciable::tpunto	elemento;
	virtual void		colorear(const tcolor&, const tcolor&) {}
	virtual void		mover(double x, double y) 
	{
		elemento.x+=x; elemento.y+=y;
		poligono=cuadrado(elemento.x, elemento.y, 6);
	}
	virtual bool		es_bajo_cursor(punto pt) const {return punto_en_poligono(poligono, pt);}
	virtual void		dibujar(Representador& r,DLibV::Pantalla& pantalla, const Struct_camara& struct_camara, bool seleccionado=false) const
	{
		dibujar_poligono(r, pantalla, poligono, color_punto_bot_editor, color_punto_bot_editor, struct_camara, seleccionado);
	}

	private:

	Espaciable::tpoligono	poligono;
};


}

#endif