#include "principal.h"

#include <fstream>
#include <algorithm>
#include <string>

#include <class/lector_txt.h>

#include "../app/framework_impl/input.h"
#include "estados_controladores.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log)
	:log(log), 
		fuente_akashi("data/fuentes/Akashi.ttf", 16), 
		zoom(1.0), 
		xcam(0), 
		ycam(0),
		grid(20),
		tobjeto(tobjetocreado::inicio)
{
	
}

void  Controlador_principal::preloop(DFramework::Input& input, float delta)
{

}

void  Controlador_principal::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
	}
	else
	{
		if(input.es_input_down(Input::escape))
		{
			if(jugadores.size()) jugadores.clear();
			else abandonar_aplicacion();
			return;
		}

		if(input.es_input_down(Input::tab))
		{
			if(tobjeto==tobjetocreado::inicio) tobjeto=tobjetocreado::arma;
			else tobjeto=tobjetocreado::inicio;
		}

		if(input.es_input_down(Input::cargar_mapa)) cargar_mapa();	
		else if(input.es_input_down(Input::grabar_mapa)) grabar_mapa();	

		pos_raton=input.acc_posicion_raton();

		if(input.es_input_pulsado(Input::zoom_mas)) 
		{
			zoom+=(double)delta;
		}
		else if(input.es_input_pulsado(Input::zoom_menos)) 
		{
			zoom-=(double)delta;
			if(zoom < 0.10) zoom=0.10;
		}

		if(input.es_input_down(Input::j1_registrar)) registrar_jugador(1);
		if(input.es_input_down(Input::j2_registrar)) registrar_jugador(2);
		if(input.es_input_down(Input::j3_registrar)) registrar_jugador(3);

		if(input.es_input_down(Input::click_i))
		{
			nuevo_punto(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		}
		else if(input.es_input_down(Input::click_d))
		{
			if(poligono_construccion.acc_vertices().size())
			{
				cerrar_poligono();
			}
			else
			{
				switch(tobjeto)
				{
					case tobjetocreado::inicio:
						crear_punto_inicio(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
					break;

					case tobjetocreado::arma:
						crear_punto_generador_items(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
					break;
				}
			}
		}

		//Procesar generadores...
		for(auto& g : generadores_items)
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
				for(auto& o : obstaculos)
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

			for(const auto& o : obstaculos)
			{
				if(j.en_colision_con(o))
				{
					j.colisionar();
					colision=true;
					break;
				}
			}

			for(auto& g : generadores_items)
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

			if(!j.acc_salud())
			{
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
}

void  Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);

	Representador r;

	ajustar_camara();

	if(!jugadores.size())
	{
		r.dibujar_rejilla(pantalla, grid, {255, 255, 255, 64}, xcam, ycam, zoom);
	}
	

	for(const auto& o : obstaculos)
	{
		r.dibujar_poligono(pantalla, o.acc_poligono(), o.acc_color(), xcam, ycam, zoom);
	}

	for(const auto& g : generadores_items)
	{
		if(g.es_activo()) r.dibujar_poligono(pantalla, g.acc_poligono(), g.acc_color(), xcam, ycam, zoom);
	}

	for(const auto& j : jugadores)
	{
		r.dibujar_poligono(pantalla, j.acc_poligono(), j.acc_color(), xcam, ycam, zoom);
	}

	for(const auto& p : proyectiles)
	{
		r.dibujar_poligono(pantalla, p->acc_poligono(), p->acc_color(), xcam, ycam, zoom);
	}
	
	//TODO: Quizás dibujar el segmento en construcción tb.
	for(const auto& s : poligono_construccion.acc_segmentos())
	{
		r.dibujar_segmento(pantalla, s, {0, 255, 0, 128}, xcam, ycam, zoom);		
	}

	if(!jugadores.size())
	{
		for(const auto& p : puntos_inicio)
		{
			Espaciable::tpoligono poli={ {{p.x, p.y}, {p.x+10, p.y}, {p.x+10, p.y+10}, {p.x, p.y+10}}, {p.x, p.y}};
			r.dibujar_poligono(pantalla, poli, {255, 0, 0, 128}, xcam, ycam, zoom);
		}

		for(const auto& g : generadores_items)
		{
			r.dibujar_poligono(pantalla, g.acc_poligono(), {0, 0, 255, 128}, xcam, ycam, zoom);
		}	

		auto pt_raton=punto_desde_pos_pantalla(pos_raton.x, pos_raton.y);
		std::string texto=std::to_string((int)pt_raton.x)+","+std::to_string((int)pt_raton.y);
		texto+=tobjeto==tobjetocreado::inicio ? " [spawn]" : " [weapon]";

		DLibV::Representacion_TTF txt(fuente_akashi, {255, 255, 255, 255}, texto);
		txt.ir_a(16, 16);
		txt.volcar(pantalla);

		//Punto actual...
		Espaciable::tpoligono poli={ {{pt_raton.x-5, pt_raton.y-5}, {pt_raton.x-5, pt_raton.y+5}, {pt_raton.x+5, pt_raton.y+5}, {pt_raton.x+5, pt_raton.y-5}}, {pt_raton.x, pt_raton.y}};
		r.dibujar_poligono(pantalla, poli, {255, 255, 255, 128}, xcam, ycam, zoom);
	}

	for(const auto& j : jugadores) dibujar_info_jugador(pantalla, j);
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

void  Controlador_principal::despertar()
{
	log<<"Despertando controlador principal"<<std::endl;
}

void  Controlador_principal::dormir()
{
	log<<"Durmiendo controlador principal"<<std::endl;
}

bool Controlador_principal::es_posible_abandonar_estado() const
{
	return true;
}

Bloque_input Controlador_principal::obtener_bloque_input(DFramework::Input& input, const Traduccion_input& traduccion) const
{
	Bloque_input res{0, 0, false};

	if(input.es_input_pulsado(traduccion.arriba))
	{
		res.aceleracion=1;
	}
	else if(input.es_input_pulsado(traduccion.abajo))
	{
		res.aceleracion=-1;
	}

	if(input.es_input_pulsado(traduccion.izquierda))
	{
		res.giro=1;
	}
	else if(input.es_input_pulsado(traduccion.derecha))
	{
		res.giro=-1;
	}

	if(input.es_input_down(traduccion.disparo))
	{
		res.disparo=true;
	}

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

//TODO: Esto es sólo para prototipo.
DLibH::Punto_2d<double>	Controlador_principal::punto_desde_pos_pantalla(int x, int y)
{
	int px=xcam+(x/zoom);
	int py=ycam-(y/zoom);

	px=round(px / grid) * grid;
	py=round(py / grid) * grid;

	return DLibH::Punto_2d<double>{(double)px, (double)py};
}

//TODO: Esto es sólo para prototipo.
void Controlador_principal::nuevo_punto(DLibH::Punto_2d<double> p)
{
	poligono_construccion.insertar_vertice(p);
}

//TODO: Esto es sólo para prototipo.
void Controlador_principal::cerrar_poligono()
{
	if(poligono_construccion.size() == 3)
	{
		poligono_construccion.cerrar();
		obstaculos.push_back({poligono_construccion, {64, 64, 64, 255}});
	}
	else if(poligono_construccion.size() > 3 && !poligono_construccion.es_concavo())
	{
		poligono_construccion.cerrar();
		obstaculos.push_back({poligono_construccion, {64, 64, 64, 255}});
	}

	poligono_construccion=Poligono_2d<double>{};
}

void Controlador_principal::registrar_jugador(int indice)
{
	if(!puntos_inicio.size())
	{
		return;
	}

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

		Herramientas_proyecto::Generador_int gen(0, puntos_inicio.size()-1);

		//Comprobar que está vacío (posible bucle infinito).
		while(true)
		{
			auto pt=puntos_inicio[gen()];
			j.establecer_posicion(pt.x, pt.y);

			bool colision=false;
			for(auto& oj : jugadores)
			{
				if(j.en_colision_con(oj))
				{
					colision=true;
					break;
				}
			}

			if(!colision) break;
		};

		j.establecer_arma(new Jugador_arma_normal());
		jugadores.push_back(std::move(j));
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

		if(jugadores.size()==1)
		{
			const auto& c=jugadores[0].acc_poligono().acc_centro();
			xcam=c.x - (mitad_w_pantalla / zoom), 
			ycam=c.y + (mitad_h_pantalla / zoom);
		}
		else 
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

			xcam=( xmin + ( (xmax-xmin) / 2) ) - (mitad_w_pantalla / zoom), 
			ycam=( ymin + ( (ymax-ymin) / 2) ) + (mitad_h_pantalla / zoom);

			
			//Establecer zoom...
			double 	distx=xmax-xmin,
				disty=ymax-ymin;

			double zoomx=distx < mitad_w_pantalla ? 1.0 : (double)mitad_w_pantalla / distx;
			double zoomy=disty < mitad_h_pantalla ? 1.0 : (double)mitad_h_pantalla / disty;

			zoom=zoomx < zoomy ? zoomx : zoomy;
			if(zoom > 1.0) zoom=1.0;
		}
	}
	
}

void Controlador_principal::crear_punto_inicio(DLibH::Punto_2d<double> pt)
{
	puntos_inicio.push_back(pt);
}

void Controlador_principal::crear_punto_generador_items(DLibH::Punto_2d<double> pt)
{
	Generador_items gi(pt);
	gi.reiniciar();
	generadores_items.push_back(gi);
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

void Controlador_principal::cargar_mapa()
{
	obstaculos.clear();
	puntos_inicio.clear();
	generadores_items.clear();

	Importador importador;
	importador.importar("mapa.dat", obstaculos, puntos_inicio, generadores_items);
}

void Controlador_principal::grabar_mapa()
{
	std::ofstream fichero("mapa.dat");
	Exportador exportador;
	fichero<<exportador.serializar(obstaculos, puntos_inicio, generadores_items);
}
