#ifndef JUGADOR_H
#define JUGADOR_h

#include <map>

#include "color.h"
#include "espaciable.h"
#include "representable.h"
#include "bloque_input.h"

#include "disparador.h"
#include "jugador_arma.h"

namespace App
{


//TODO: A otro header...
struct Habilidad_base
{
	struct bloque_efecto
	{
		float&		velocidad;
		int&		divisor_salud;
		bloque_efecto(float &v, int& s):velocidad(v), divisor_salud(s) {}
	};

	enum class 		ttipo{velocidad, escudo};

	virtual ttipo		obtener_tipo() const=0;
	virtual float		obtener_cooloff() const=0;
	virtual void		activar(bloque_efecto&)=0;
	virtual void		desactivar(bloque_efecto&)=0;

	bool			es_activa() const {return tiempo_restante > 0.0f;}
	void			turno(float delta)
	{
		tiempo_restante-=delta;
		if(tiempo_restante < 0.0f) tiempo_restante=0.0f;
	}

	float			tiempo_restante;	
};

struct Habilidad_velocidad:
	public Habilidad_base
{
	virtual ttipo		obtener_tipo() const {return ttipo::velocidad;}
	virtual float		obtener_cooloff() const {return 4.0f;}
	virtual void		activar(bloque_efecto& bl)
	{
		tiempo_restante=1.0f;
		bl.velocidad=500.0f;
	}

	virtual void		desactivar(bloque_efecto& bl)
	{

	}
};

struct Habilidad_escudo:
	public Habilidad_base
{
	virtual ttipo		obtener_tipo() const {return ttipo::escudo;}
	virtual float		obtener_cooloff() const {return 4.0f;}
	virtual void		activar(bloque_efecto& bl)
	{
		tiempo_restante=2.0f;
		bl.divisor_salud=10;
	}

	virtual void		desactivar(bloque_efecto& bl)
	{
		bl.divisor_salud=1;
	}
};

class Jugador:
	public Espaciable,
	public Representable
{
	public:

					Jugador(int, tcolor);

	int				acc_indice() const {return indice;}
	int				acc_salud() const {return salud;}
	int 				acc_energia() const {return floor(energia);}
	tcolor				acc_color() const {return color;}

	bool				es_y_puede_disparar() const {return input_actual.disparo && arma!=nullptr && arma->es_preparada();}

	bool				es_arma_defecto() const;	
	bool				es_arma_agotada() const;	
	int				acc_municion_restante() const;

	Disparador			disparar();
	void				recibir_input(const Bloque_input&);
	void				turno(float);
	void				confirmar_movimiento();
	void				colisionar();
	void				restar_salud(int);
	void				establecer_arma(Jugador_arma * a);

	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const Struct_camara&) const;

	private:

	enum class ttipo_pulsacion {velocidad, escudo};
	struct Pulsacion_input
	{
		float			t;
		Pulsacion_input():t(0.0f) {}
	};
	
	void				formar_poligono();
	void				cambiar_velocidad(int, float);
	void				detener_velocidad(float);
	void				girar(int, float);
	void				movimiento_tentativo(float);
	bool				es_max_energia() const {return energia==100.0f;}

	std::unique_ptr<Jugador_arma>	arma;
	std::unique_ptr<Habilidad_base>	habilidad;
	std::map<ttipo_pulsacion, Pulsacion_input>	pulsaciones_input;
	Bloque_input			input_actual;
	tpoligono			posicion_anterior;

	//TODO: Hacer esto bien... Con una clase que rote y tal...
	mutable tpoligono		poligono_escudo;

	int				indice;	//Indice del input que lo controla.
	double 				angulo, angulo_anterior;
	float				velocidad;
	float				energia, cooloff_energia;
	int				salud, divisor_salud;
	tcolor				color;
};

}

#endif
