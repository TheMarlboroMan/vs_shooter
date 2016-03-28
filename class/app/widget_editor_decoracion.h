#ifndef WIDGET_EDITOR_DECORACION_H
#define WIDGET_EDITOR_DECORACION_H

#include <class/compositor_vista.h>

#include "widget_editor_interface.h"
#include "decoracion.h"

namespace App
{

class Widget_editor_decoracion: 
	public Widget_editor_interface
{
	public:

	Widget_editor_decoracion(const DLibV::Fuente_TTF&, Decoracion&);
	
	virtual void 	dibujar(DLibV::Pantalla&);
	virtual void	input(DFramework::Input&, float);
	virtual bool	es_cerrar() const;
	virtual void	finalizar();

	private:

	void		actualizar_layout();
	void		cambiar_seleccion(int);
	void		cambiar_valor(int, float);
	void		cambiar_numero(int, int, int, int&);

	Herramientas_proyecto::Compositor_vista		layout;
	Decoracion&	elemento;
	tcolor		color_frente, 
			color_linea;
	int		profundidad;
	bool		frente;
	bool		cerrar;

	int		indice_actual;
	float		t_pulsado;

	static const int	min_indice=0;
	static const int 	max_indice=9;
	static const int	min_color=0;
	static const int	max_color=255;
	static const int	min_profundidad=0;
	static const int	max_profundidad=200;
};

}

#endif
