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
	camara(0, 0, 640, 400)
{
	registrar_info_jugadores();
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

	if(input.es_input_down(Input::j1_disparo) && !info_jugadores[0].es_registrado()) registrar_jugador(0);
	if(input.es_input_down(Input::j2_disparo) && !info_jugadores[1].es_registrado()) registrar_jugador(1);
	if(input.es_input_down(Input::j3_disparo) && !info_jugadores[2].es_registrado()) registrar_jugador(2);
	if(input.es_input_down(Input::NUEVO_BOT)) registrar_bot();

	//Procesar generadores...
	for(auto& g : mapa.generadores_items) g.turno(delta);
	procesar_proyectiles(delta);
	procesar_bots(delta);
	procesar_jugadores(input, delta);
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

	for(const auto& d : mapa.decoraciones_fondo)	d->dibujar(r, pantalla, camara);
	for(const auto& g : mapa.generadores_items) 	g.dibujar(r, pantalla, camara);
	for(const auto& j : jugadores) 			j.dibujar(r, pantalla, camara);
	for(const auto& b : bots) 			b.dibujar(r, pantalla, camara);
	for(const auto& p : proyectiles) 		p->dibujar(r, pantalla, camara);
	for(const auto& d : mapa.decoraciones_frente)	d->dibujar(r, pantalla, camara);

	for(const auto& j : jugadores) 			dibujar_info_jugador(pantalla, j);
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
	if(input.es_input_pulsado(traduccion.habilidad)) res.activar_habilidad=true;
	if(input.es_input_down(traduccion.arriba)) res.habilidad_velocidad=true;
	if(input.es_input_down(traduccion.abajo)) res.habilidad_escudo=true;


	return res;
}

void Controlador_principal::registrar_jugador(int indice)
{
	if(!mapa.puntos_inicio.size())
	{
		return;
	}

	auto& ij=info_jugadores[indice];
	Jugador j={indice, ij.acc_color()};

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
			colision=j.en_colision_con(oj);
			if(colision) break;
		}
			
		if(!colision) 
		{		
			j.establecer_arma(new Jugador_arma_normal());
			jugadores.push_back(std::move(j));
			ij.registrar();
			break;
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

		for(const auto& bot : bots)
		{
			const auto& c=bot.acc_poligono().acc_centro();
			if(c.x < xmin) xmin=c.x;
			else if(c.x > xmax) xmax=c.x;

			if(c.y < ymin) ymin=c.y;
			else if(c.y > ymax) ymax=c.y;
		}		

		double zoom=camara.acc_zoom();

		int xcam=( xmin + ( (xmax-xmin) / 2) ) - (mitad_w_pantalla / zoom);
		int ycam=( ymin + ( (ymax-ymin) / 2) ) + (mitad_h_pantalla / zoom);
		
		//Establecer zoom...
		double 	distx=xmax-xmin,
			disty=ymax-ymin;

		double zoomx=distx < mitad_w_pantalla ? 1.0 : (double)mitad_w_pantalla / distx;
		double zoomy=disty < mitad_h_pantalla ? 1.0 : (double)mitad_h_pantalla / disty;

		zoom=zoomx < zoomy ? zoomx : zoomy;

std::cout<<"ZOOM"<<zoom<<std::endl;

		if(zoom > 1.0) zoom=1.0;
		camara.mut_zoom(zoom);
		camara.enfocar_a(xcam, -ycam);
	}
	else
	{
		//TODO: Enfocar todo el nivel.
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
	const auto& ij=info_jugadores[j.acc_indice()];

	int x=ij.acc_x_hud(), y=ij.acc_y_hud();
	auto jc=ij.acc_color();
	int m=j.acc_municion_restante();

	std::string municion=std::to_string(m);
	std::string texto=std::to_string(j.acc_salud())+" | "+std::to_string(j.acc_energia())+" - "+municion;
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
			colision=bot.en_colision_con(oj);
			if(colision) break;
		}

		if(!colision)
		{
			for(auto& b : bots)
			{
				colision=bot.en_colision_con(b);
				if(colision) break;
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

	info_jugadores[j.acc_indice()].retirar();
}

void Controlador_principal::registrar_info_jugadores()
{
	info_jugadores.push_back(Jugador_info(0, {255, 0, 0, 255}));
	info_jugadores.push_back(Jugador_info(1, {0, 255, 0, 255}));
	info_jugadores.push_back(Jugador_info(2, {0, 0, 255, 255}));
}

void Controlador_principal::procesar_jugadores(DFramework::Input& input, float delta)
{
	//TODO: Separar...
	auto ini_j=std::begin(jugadores);
	while(ini_j < std::end(jugadores))
	{
		//TODO: Disaster.
		auto& j=*ini_j;
		int indice=j.acc_indice();

		auto bl=obtener_bloque_input(input, info_jugadores[indice].acc_traduccion_input());
		j.recibir_input(bl);
		j.turno(delta);

		bool colision=false;

		for(const auto& o : mapa.obstaculos)
		{
			if(j.en_colision_con(o))
			{
				switch(o.acc_tipo())
				{
					case Obstaculo::ttipos::normal: j.colisionar(); break;
					case Obstaculo::ttipos::letal: j.restar_toda_salud(); break;
				}

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

			if(j.es_y_puede_disparar())
			{
				disparadores.push_back(j.disparar());
				if(j.es_arma_agotada() && !j.es_arma_defecto())
				{
					j.establecer_arma(new Jugador_arma_normal());
				}
			}
		}

		++ini_j;
	}
}

void Controlador_principal::procesar_bots(float delta)
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

void Controlador_principal::procesar_proyectiles(float delta)
{
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

		//TODO: Mejorar... 
		if(!borrar)
		{
			for(auto& j : jugadores)
			{
				if(p.en_colision_con(j) && j.acc_salud())
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
				if(p.acc_indice_jugador() >= 0 && p.en_colision_con(bot) && bot.acc_salud())
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
}
