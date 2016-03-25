#ifndef BOT_H
#define BOT_H

#include <class/generador_numeros.h>
#include "espaciable.h"
#include "trazador_ruta.h"
#include "mapa.h"
#include "disparador.h"

namespace App
{

class Bot:
	public Espaciable
{
	public:

					Bot();
	void				turno(const Mapa&, std::vector<Disparador>&, float);
	void				establecer_destino(const Espaciable& e);
	bool				es_destino(const Espaciable& e) const {return destino==&e;}
	bool				es_activo() const {return destino!=nullptr;}
	void				anular_destino() {destino=nullptr; estado=testados::nada;}
	void				colisionar(const Mapa&);
	void				restar_salud(int);
	int				acc_salud() const {return salud;}
	const std::vector<Espaciable::tpunto>&	acc_ruta() const {return ruta;}

	private:

	enum class testados {nada, buscando, rotando, moviendo, posicionar_ataque};

	void				formar_poligono();
	void				trazar_ruta(const Mapa&);
	void				trazar_hasta_posicion_ataque();
	void				orientar_giro(float);
	void				mover(float);
	void				calcular_angulo_destino(Punto_2d<double>);
	void				intento_disparo(const Mapa&, std::vector<Disparador>&);

	std::vector<Espaciable::tpunto>	ruta;
	const Espaciable *		destino;

	tpoligono			posicion_anterior;
	testados			estado;
	double				angulo, angulo_anterior, grados_a_girar;
	double				distancia_anterior;
	int 				direccion_giro;
	float				velocidad;
	float				t_disparo, t_reevaluar, t_comprobar_visual;
	int				salud;
};

}

#endif
