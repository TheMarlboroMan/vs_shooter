#include "principal.h"

#include <algorithm>

#include <class/generador_numeros.h>

#include "../app/jugador_arma_triple.h"
#include "../app/jugador_arma_normal.h"
#include "../app/jugador_arma_explosivo.h"
#include "../app/jugador_arma_trasero.h"
#include "../app/jugador_arma_divide.h"

//TODO: Quitar con factoría de disparos.
#include "../app/proyectil_normal.h"
#include "../app/proyectil_peq.h"
#include "../app/proyectil_explosivo.h"
#include "../app/proyectil_divide.h"

#include "../app/representador.h"

#include "../app/importador.h"

#include "../app/framework_impl/input.h"
#include "estados_controladores.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log, const Fuentes& f)
	:log(log),
	fuente_akashi(f.obtener_fuente("akashi", 16)),
	struct_camara({1.0, 0, 0})
{

}

void Controlador_principal::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_principal::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
		return;
	}

	if(input.es_input_down(Input::cambio_logica))
	{
		solicitar_cambio_estado(editor);
		return;
	}

	if(input.es_input_down(Input::cambio_controles))
	{
		solicitar_cambio_estado(controles);
		return;
	}

	if(input.es_input_down(Input::j1_registrar)) registrar_jugador(1);
	if(input.es_input_down(Input::j2_registrar)) registrar_jugador(2);
	if(input.es_input_down(Input::j3_registrar)) registrar_jugador(3);
	if(input.es_input_down(Input::NUEVO_BOT)) registrar_bot();

	//Procesar generadores...
	for(auto& g : mapa.generadores_items)
	{
		g.turno(delta);
	}

	//TODO: Separar

	auto p_ini=std::begin(proyectiles);
	while(p_ini < std::end(proyectiles))
	{			
		//TODO: Desastre.
		auto& p=**p_ini;

		p.turno(delta);

		bool borrar=false;

		if(!p.es_activo())
		{
			p.extinguir(disparadores);
			borrar=true;
		}

		//TODO: Mejorar... Esto quizás deba estar en el código
		//del jugador?
		if(!borrar)
		{
			for(auto& j : jugadores)
			{
				if(p.en_colision_con(j))
				{
					j.restar_salud(p.acc_potencia());
					p.colisionar(disparadores);		
					borrar=true;
				}	
			}
		}

		if(!borrar)
		{
			for(auto& bot : bots)
			{
				//Los proyectiles de los bots no afectan a otros bots.
				if(p.acc_indice_jugador() && p.en_colision_con(bot))
				{
					bot.restar_salud(p.acc_potencia());
					p.colisionar(disparadores);		
					borrar=true;
				}
			}
		}

		if(!borrar)
		{
			for(auto& o : mapa.obstaculos)
			{
				if(p.en_colision_con(o))
				{
					p.colisionar(disparadores);
					borrar=true;
				}
			}
		}

		if(borrar) p_ini=proyectiles.erase(p_ini);
		else ++p_ini;
	}

	{
		auto inib=std::begin(bots);
		while(inib < std::end(bots))
		{
			auto& bot=*inib;

			if(!bot.es_activo())
			{
				if(jugadores.size())
				{
					//TODO: Código repetido.
					Herramientas_proyecto::Generador_int genj(0, jugadores.size()-1);
					bot.establecer_destino(jugadores[genj()]);
				}
				else
				{
					//TODO: Forzar vuelta a algún punto???
					//TODO: La vuelta debería ser "pacifica", que no dispare...
				}
			}
			

			if(!bot.acc_salud())
			{
				inib=bots.erase(inib);
			}
			else
			{	
				bot.turno(mapa, disparadores, delta);
				++inib;
			}
		}
	}

	//TODO: Separar...
	auto ini_j=std::begin(jugadores);
	while(ini_j < std::end(jugadores))
	{
		//TODO: Disaster.
		auto& j=*ini_j;			

		auto bl=obtener_bloque_input(input, obtener_traduccion_input(j.acc_indice()));
		j.recibir_input(bl);
		j.turno(delta);

		bool colision=false;

		for(const auto& o : mapa.obstaculos)
		{
			if(j.en_colision_con(o))
			{
				j.colisionar();
				colision=true;
				break;
			}
		}

		for(auto& g : mapa.generadores_items)
		{
			if(g.es_activo() && j.en_colision_con(g))
			{
				switch(g.acc_tipo())
				{
					case Generador_items::titems::triple:
						j.establecer_arma(new Jugador_arma_triple());
					break;
					case Generador_items::titems::explosivo:
						j.establecer_arma(new Jugador_arma_explosivo());
					break;
					case Generador_items::titems::trasero:
						j.establecer_arma(new Jugador_arma_trasero());
					break;
					case Generador_items::titems::divide:
						j.establecer_arma(new Jugador_arma_divide());
					break;
				}

				g.reiniciar();
			}
		}

		for(auto& oj : jugadores)
		{
			if(oj.acc_indice()!=j.acc_indice() && j.en_colision_con(oj))
			{
				//TODO: Si uno está parado no recibe daños.
				j.colisionar();
				oj.colisionar();
				colision=true;
				break;
			}
		}

		for(auto& ob : bots)
		{
			if(j.en_colision_con(ob))
			{
				j.colisionar();
				colision=true;
				break;
			}
		}

		if(!j.acc_salud())
		{
			pre_eliminar_jugador(j);
			ini_j=jugadores.erase(ini_j);
			continue;
		}
		else
		{
			if(!colision) j.confirmar_movimiento();

			//Inelegante...
			if(j.es_y_puede_disparar())
			{
				disparadores.push_back(j.disparar());

				if(j.es_arma_agotada())
				{
					j.establecer_arma(new Jugador_arma_normal());
				}
			}
		}

		++ini_j;
	}

	procesar_disparadores();
}

void  Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);

	Representador r;
	ajustar_camara();

	for(const auto& o : mapa.obstaculos)
		r.dibujar_poligono(pantalla, o.acc_poligono(), o.acc_color(), struct_camara);

	for(const auto& g : mapa.generadores_items)
		if(g.es_activo()) r.dibujar_poligono(pantalla, g.acc_poligono(), g.acc_color(), struct_camara);

	for(const auto& j : jugadores)
		r.dibujar_poligono(pantalla, j.acc_poligono(), j.acc_color(), struct_camara);

	for(const auto& bot : bots)
	{
		r.dibujar_poligono_lineas(pantalla, bot.acc_poligono(), {255, 255, 255, 255}, struct_camara);
/*
auto cuadrado=[](double x, double y, int rad)
{
	return Espaciable::tpoligono { {{x-rad, y-rad}, {x-rad, y+rad}, {x+rad, y+rad}, {x+rad, y-rad}}, {x, y}};
};

		auto& pts=bot.acc_ruta();
		for(const auto& pt : pts)
		{
			r.dibujar_poligono(pantalla, cuadrado(pt.x, pt.y, 10), {255, 0, 0,255}, struct_camara);
		}
*/
	}


	for(const auto& p : proyectiles)
		r.dibujar_poligono(pantalla, p->acc_poligono(), p->acc_color(), struct_camara);
	
	for(const auto& j : jugadores) 
		dibujar_info_jugador(pantalla, j);
}

void  Controlador_principal::despertar()
{
	mapa.inicializar();
}

void  Controlador_principal::dormir()
{
	mapa.limpiar();
	jugadores.clear();
	proyectiles.clear();
	disparadores.clear();
}

bool Controlador_principal::es_posible_abandonar_estado() const
{
	return true;
}

////////////////

Bloque_input Controlador_principal::obtener_bloque_input(DFramework::Input& input, const Traduccion_input& traduccion) const
{
	Bloque_input res{0, 0, false};

	if(input.es_input_pulsado(traduccion.arriba)) res.aceleracion=1;
	else if(input.es_input_pulsado(traduccion.abajo)) res.aceleracion=-1;

	if(input.es_input_pulsado(traduccion.izquierda)) res.giro=1;
	else if(input.es_input_pulsado(traduccion.derecha)) res.giro=-1;

	if(input.es_input_down(traduccion.disparo)) res.disparo=true;

	return res;
}

Traduccion_input Controlador_principal::obtener_traduccion_input(int indice) const
{
	switch(indice)
	{
		case 1:
		default: 
			return Traduccion_input{Input::j1_arriba, Input::j1_abajo, Input::j1_izquierda, Input::j1_derecha, Input::j1_disparo};
		break;
		case 2:
			return Traduccion_input{Input::j2_arriba, Input::j2_abajo, Input::j2_izquierda, Input::j2_derecha, Input::j2_disparo};
		break;
		case 3:
			return Traduccion_input{Input::j3_arriba, Input::j3_abajo, Input::j3_izquierda, Input::j3_derecha, Input::j3_disparo};
		break;
	}
}

void Controlador_principal::registrar_jugador(int indice)
{
	if(!mapa.puntos_inicio.size())
	{
		return;
	}

	//TODO: cambiar por un simple vector de lookup... Esto es un infierno así!!!.
	if(!std::any_of(std::begin(jugadores), std::end(jugadores), [indice](const Jugador& j) {return j.acc_indice()==indice;}))
	{
		tcolor color={255, 0, 0, 255};
		switch(indice)
		{
			default:
			case 1: color={255, 0, 0, 255}; break;
			case 2: color={0, 255, 0, 255}; break;
			case 3: color={0, 0, 255, 255}; break;
		}

		Jugador j={indice, color};

		size_t capacidad=mapa.puntos_inicio.size()-1;
		std::vector<size_t> indices(capacidad);
		std::iota(std::begin(indices), std::end(indices), 0);
		std::random_shuffle(std::begin(indices), std::end(indices));

		for(size_t indice : indices)
		{
			bool colision=false;

			auto pt=mapa.puntos_inicio[indice];
			j.establecer_posicion(pt.x, pt.y);

			for(auto& oj : jugadores)
			{
				if(j.en_colision_con(oj))
				{
					colision=true;
					break;
				}
			}
				
			if(!colision) 
			{		
				j.establecer_arma(new Jugador_arma_normal());
				jugadores.push_back(std::move(j));
				break;
			}
		}
	}
}

void Controlador_principal::ajustar_camara()
{
	//TODO... serían configurables?
	const int w_pantalla=640, 
		h_pantalla=400,
		mitad_w_pantalla=w_pantalla / 2,
		mitad_h_pantalla=h_pantalla / 2;

	if(jugadores.size())
	{
		int xmin=jugadores[0].acc_poligono().acc_centro().x, xmax=xmin,
			ymin=jugadores[0].acc_poligono().acc_centro().y, ymax=ymin;

		//Localizar punto central.
		for(const auto&j : jugadores)
		{
			const auto& c=j.acc_poligono().acc_centro();
			if(c.x < xmin) xmin=c.x;
			else if(c.x > xmax) xmax=c.x;

			if(c.y < ymin) ymin=c.y;
			else if(c.y > ymax) ymax=c.y;
		}

		//TODO: Experimental: centrando con bot.		
		for(const auto& bot : bots)
		{
			const auto& c=bot.acc_poligono().acc_centro();
			if(c.x < xmin) xmin=c.x;
			else if(c.x > xmax) xmax=c.x;

			if(c.y < ymin) ymin=c.y;
			else if(c.y > ymax) ymax=c.y;
		}		

		struct_camara.xcam=( xmin + ( (xmax-xmin) / 2) ) - (mitad_w_pantalla / struct_camara.zoom), 
		struct_camara.ycam=( ymin + ( (ymax-ymin) / 2) ) + (mitad_h_pantalla / struct_camara.zoom);

		
		//Establecer zoom...
		double 	distx=xmax-xmin,
			disty=ymax-ymin;

		double zoomx=distx < mitad_w_pantalla ? 1.0 : (double)mitad_w_pantalla / distx;
		double zoomy=disty < mitad_h_pantalla ? 1.0 : (double)mitad_h_pantalla / disty;

		struct_camara.zoom=zoomx < zoomy ? zoomx : zoomy;
		if(struct_camara.zoom > 1.0) struct_camara.zoom=1.0;
	}
}

void Controlador_principal::procesar_disparadores()
{
	//TODO: Quitar con factoría de disparos.

	for(auto& d : disparadores)
	{
		for(const auto& i : d.disparos)
		{
			std::unique_ptr<Proyectil>	p(nullptr);
			switch(i.tipo)
			{
				case Disparador::tproyectiles::normal:
					p.reset(new Proyectil_normal(i.indice));
				break;
				case Disparador::tproyectiles::peq:
					p.reset(new Proyectil_peq(i.indice));
				break;
				case Disparador::tproyectiles::explosivo:
					p.reset(new Proyectil_explosivo(i.indice));
				break;
				case Disparador::tproyectiles::divide:
					p.reset(new Proyectil_divide(i.indice));
				break;
			}
			
			p->preparar(i.angulo, i.pt);
			proyectiles.push_back(std::move(p));
		}
	}

	disparadores.clear();
}

void Controlador_principal::dibujar_info_jugador(DLibV::Pantalla& pantalla, const Jugador& j)
{
	int x=16, y=16;
	auto jc=j.acc_color();
//	auto centro=j.acc_poligono().centro();

	switch(j.acc_indice())
	{
		case 1:
		default:
		break;

		case 2:
			x=250;
		break;

		case 3:
			x=500;
		break;
	}

	int m=j.acc_municion_restante();

	std::string municion=m==-1 ? "#" : std::to_string(m);
	std::string texto=std::to_string(j.acc_salud())+" / 100 - "+municion;
	DLibV::Representacion_TTF txt(fuente_akashi, {(Uint8)jc.r, (Uint8)jc.g, (Uint8)jc.b, 192}, texto);
	txt.ir_a(x, y);
	txt.volcar(pantalla);
}

void Controlador_principal::registrar_bot()
{
	if(!mapa.puntos_bot.size()) return;
		
	Bot bot;
	Herramientas_proyecto::Generador_int gen(0, mapa.puntos_bot.size()-1);

	if(jugadores.size())
	{
		Herramientas_proyecto::Generador_int genj(0, jugadores.size()-1);
		bot.establecer_destino(jugadores[genj()]);
	}

	//Comprobar que no hay jugadores ni bots.
	size_t capacidad=mapa.puntos_bot.size()-1;
	std::vector<size_t> indices(capacidad);
	std::iota(std::begin(indices), std::end(indices), 0);
	std::random_shuffle(std::begin(indices), std::end(indices));

	for(size_t indice : indices)
	{	
		bool colision=false;

		auto pt=mapa.puntos_bot[indice];
		bot.establecer_posicion(pt.x, pt.y);

		for(auto& oj : jugadores)
		{
			if(bot.en_colision_con(oj))
			{
				colision=true;
				break;
			}
		}

		if(!colision)
		{
			for(auto& b : bots)
			{
				if(bot.en_colision_con(b))
				{
					colision=true;
					break;
				}
			}
		}

		if(!colision)
		{
			bots.push_back(bot);
			break;
		}
	}
}

void Controlador_principal::pre_eliminar_jugador(const Jugador& j)
{
	for(auto& bot : bots)
		if(bot.es_destino(j)) 
			bot.anular_destino();
}
