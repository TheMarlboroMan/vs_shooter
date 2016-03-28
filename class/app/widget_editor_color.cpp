#include "widget_editor_color.h"

#include "framework_impl/input.h"

#include <def_video.h>

using namespace App;

Widget_editor_color::Widget_editor_color(const DLibV::Fuente_TTF& fuente, tcolor& f, tcolor& l)
	:color_fondo(f), color_linea(l), cerrar(false), indice_actual(min_indice), t_pulsado(0.0f)
{
	layout.mapear_fuente("akashi", fuente);
	layout.parsear("data/layout/widget_color.dnot", "layout");

	actualizar_layout();
}
	
void Widget_editor_color::dibujar(DLibV::Pantalla& pantalla)
{
	using namespace DLibH;

	layout.volcar(pantalla);	
}

void Widget_editor_color::input(DFramework::Input& input, float delta)
{
	if(input.es_input_down(Input::escape)) 
	{
		cerrar=true;
		return;
	}

	if(input.es_input_down(Input::cursor_abajo)) cambiar_seleccion(1);
	else if(input.es_input_down(Input::cursor_arriba)) cambiar_seleccion(-1);
	else if(input.es_input_pulsado(Input::cursor_izquierda)) cambiar_color(-1, delta);
	else if(input.es_input_pulsado(Input::cursor_derecha)) cambiar_color(1, delta);
	else t_pulsado=0.0f;
}

bool Widget_editor_color::es_cerrar() const
{
	return cerrar;
}

void Widget_editor_color::cambiar_seleccion(int dir)
{
	indice_actual+=dir;
	if(indice_actual < min_indice) indice_actual=min_indice;
	else if(indice_actual > max_indice) indice_actual=max_indice;

	int y=layout.const_int("y_selector")+(indice_actual * layout.const_int("salto_selector"));
	layout.obtener_por_id("selector")->ir_a(layout.const_int("x_selector"), y);
}

void Widget_editor_color::cambiar_color(int dir, float delta)
{
	auto f=[this](int dir, int indice)
	{
		switch(indice)
		{
			case 0: cambiar(dir, color_fondo.r, "txt_r_frente", "R"); break;
			case 1: cambiar(dir, color_fondo.g, "txt_g_frente", "G"); break;
			case 2: cambiar(dir, color_fondo.b, "txt_b_frente", "B"); break;
			case 3: cambiar(dir, color_fondo.a, "txt_a_frente", "A"); break;
			case 4: cambiar(dir, color_linea.r, "txt_r_linea", "R"); break;
			case 5: cambiar(dir, color_linea.g, "txt_g_linea", "G"); break;
			case 6: cambiar(dir, color_linea.b, "txt_b_linea", "B"); break;
			case 7: cambiar(dir, color_linea.a, "txt_a_linea", "A"); break;
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
}

void Widget_editor_color::cambiar(int dir, int& ref, const std::string& id, const std::string& cad)
{
	if(dir)
	{
		ref+=dir;
		if(ref < 0) ref=0;
		else if(ref > 255) ref=255;
	}

	std::string cad_final=cad+" : "+std::to_string(ref);
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id(id))->asignar(cad_final);

	if(dir)
	{
		actualizar_colores();
	}
}

void Widget_editor_color::actualizar_layout()
{
	cambiar(0, color_fondo.r, "txt_r_frente", "R");
	cambiar(0, color_fondo.g, "txt_g_frente", "G");
	cambiar(0, color_fondo.b, "txt_b_frente", "B");
	cambiar(0, color_fondo.a, "txt_a_frente", "A");
	cambiar(0, color_linea.r, "txt_r_linea", "R");
	cambiar(0, color_linea.g, "txt_g_linea", "G");
	cambiar(0, color_linea.b, "txt_b_linea", "B");
	cambiar(0, color_linea.a, "txt_a_linea", "A");

	actualizar_colores();
}

void Widget_editor_color::actualizar_colores()
{
	DLibV::Representacion_primitiva_caja * fondo=static_cast<DLibV::Representacion_primitiva_caja *>(layout.obtener_por_id("caja_frente"));
	DLibV::Representacion_primitiva_caja * linea=static_cast<DLibV::Representacion_primitiva_caja *>(layout.obtener_por_id("caja_linea"));

	auto f=[](DLibV::Representacion_primitiva_caja& rep, tcolor col)
	{
		rep.mut_rgb(col.r, col.g, col.b);
		rep.establecer_alpha(col.a);
	};

	f(*fondo, color_fondo);
	f(*linea, color_linea);
}

void Widget_editor_color::finalizar()
{
	//No se hace nada.
}
