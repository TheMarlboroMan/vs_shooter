#include "widget_editor_decoracion.h"

#include "framework_impl/input.h"

#include <def_video.h>

using namespace App;

Widget_editor_decoracion::Widget_editor_decoracion(const DLibV::Fuente_TTF& fuente, Decoracion& d)
	:elemento(d), color_frente(d.acc_color()), color_linea(d.acc_color_linea()),
	profundidad(d.acc_profundidad()), frente(d.es_frente()),
	cerrar{false}, indice_actual{min_indice}, t_pulsado{0.0f}
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear("data/layout/widget_decoracion.dnot", "layout");

	actualizar_layout();
}
	
void Widget_editor_decoracion::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;

	layout.volcar(pantalla);	
}

void Widget_editor_decoracion::input(DFramework::Input& input, float delta)
{
	if(input.es_input_down(Input::escape)) 
	{
		cerrar=true;
		return;
	}

	if(input.es_input_down(Input::cursor_abajo)) cambiar_seleccion(1);
	else if(input.es_input_down(Input::cursor_arriba)) cambiar_seleccion(-1);
	else if(input.es_input_pulsado(Input::cursor_izquierda)) cambiar_valor(-1, delta);
	else if(input.es_input_pulsado(Input::cursor_derecha)) cambiar_valor(1, delta);
	else t_pulsado=0.0f;
}

bool Widget_editor_decoracion::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_decoracion::cambiar_seleccion(int dir)
{

	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

void Widget_editor_decoracion::cambiar_valor(int dir, float delta)
{
	auto f=[this](int dir, int indice)
	{
		switch(indice)
		{
			case 0: cambiar_numero(dir, min_color, max_color, color_frente.r); break;
			case 1: cambiar_numero(dir, min_color, max_color, color_frente.g); break;
			case 2: cambiar_numero(dir, min_color, max_color, color_frente.b); break;
			case 3: cambiar_numero(dir, min_color, max_color, color_frente.a); break;
			case 4: cambiar_numero(dir, min_color, max_color, color_linea.r); break;
			case 5: cambiar_numero(dir, min_color, max_color, color_linea.g); break;
			case 6: cambiar_numero(dir, min_color, max_color, color_linea.b); break;
			case 7: cambiar_numero(dir, min_color, max_color, color_linea.a); break;
			case 8: cambiar_numero(dir, min_profundidad, max_profundidad, profundidad); break;
			case 9: frente=!frente; break;
		}
	};

	if(t_pulsado==0.0f)
	{
		f(dir, indice_actual);
		t_pulsado+=delta;
	}
	else
	{
		if(t_pulsado > 0.6f)
		{
			f(dir, indice_actual);
		}
		else
		{
			t_pulsado+=delta;
		}
	}

	actualizar_layout();
}

void Widget_editor_decoracion::cambiar_numero(int dir, int vmin, int vmax, int& ref)
{
	if(dir)
	{
		ref+=dir;
		if(ref < vmin) ref=vmin;
		else if(ref > vmax) ref=vmax;
	}
}

void Widget_editor_decoracion::actualizar_layout()
{
	auto actualizar_numero=[this](const std::string& id, int val)
	{
		std::string cad_final=std::to_string(val);
		static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(cad_final);
	};

	auto actualizar_bool=[this](const std::string& id, bool val, const std::string& cad_true, const std::string& cad_false)
	{
		static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(val ? cad_true : cad_false);
	};

	auto f_color=[](DLibV::Representacion_primitiva_caja& rep, tcolor col)
	{
		rep.mut_rgb(col.r, col.g, col.b);
		rep.establecer_alpha(col.a);
	};

	actualizar_numero("txt_r_frente", color_frente.r);
	actualizar_numero("txt_g_frente", color_frente.g);
	actualizar_numero("txt_b_frente", color_frente.b);
	actualizar_numero("txt_a_frente", color_frente.a);
	actualizar_numero("txt_r_linea", color_linea.r);
	actualizar_numero("txt_g_linea", color_linea.g);
	actualizar_numero("txt_b_linea", color_linea.b);
	actualizar_numero("txt_a_linea", color_linea.a);
	actualizar_numero("txt_profundidad", profundidad);
	actualizar_bool("txt_frente_fondo", frente, "Frente", "Fondo");

	DLibV::Representacion_primitiva_caja * fondo=static_cast<DLibV::Representacion_primitiva_caja *>(layout.obtener_por_id("caja_frente"));
	DLibV::Representacion_primitiva_caja * linea=static_cast<DLibV::Representacion_primitiva_caja *>(layout.obtener_por_id("caja_linea"));
	f_color(*fondo, color_frente);
	f_color(*linea, color_linea);
}

void Widget_editor_decoracion::finalizar()
{
	elemento.mut_profundidad(profundidad);
	elemento.mut_color_linea(color_linea);
	elemento.mut_color(color_frente);
	elemento.mut_frente(frente);
}
