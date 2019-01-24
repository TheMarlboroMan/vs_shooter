#ifndef WIDGET_EDITOR_COLOR_H
#define WIDGET_EDITOR_COLOR_H

#include <class/view_composer.h>

#include "widget_editor_interface.h"
#include "color.h"

namespace App
{

class Widget_editor_color:
	public Widget_editor_interface
{
	public:

	Widget_editor_color(const ldv::ttf_font&, tcolor&, tcolor&);

	virtual void 	dibujar(ldv::screen&);
	virtual void	inicializar(DFramework::Input&);
	virtual void	input(DFramework::Input&, float);
	virtual void	finalizar(DFramework::Input&);
	virtual bool	es_cerrar() const;

	private:

	void		actualizar_layout();
	void		actualizar_colores();
	void		cambiar_seleccion(int);
	void		cambiar_color(int, float);
	void		cambiar(int dir, int& ref, const std::string&);
	void		cambiar_por_indice(int indice, int val);
	int		valor_por_indice(int indice) const;

	tools::view_composer		layout;
	tcolor&		color_fondo;
	tcolor&		color_linea;
	bool		cerrar;

	int		indice_actual;
	float		t_pulsado;

	static const int	min_indice=0;
	static const int 	max_indice=7;
};

}

#endif
