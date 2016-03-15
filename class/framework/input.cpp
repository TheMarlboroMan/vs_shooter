#include "input.h"

using namespace DFramework;

void Input::turno()
{
	controles_sdl.recoger();
}

Input::Resultado_lookup Input::obtener(unsigned int i) const
{
	//Esto puede dar problemas en el futuro si queremos usar el mismo
	//input para dos cosas distintas.
	//Vamos a cachear cada input en el dispositivo que le pertenece... 

	auto it=lookup.find(i);

	if(it!=lookup.end())
	{
		return it->second;
	}
	else
	{
		Resultado_lookup resultado(Resultado_lookup::NADA);

		auto f=[&resultado, this, i](const tipo_mapa& mapa, unsigned int tipo, unsigned int indice)
		{
			resultado.mapa=tipo;
			auto it=mapa.equal_range(i);
			for(auto r=it.first; r!=it.second; ++r)
			{
				resultado.val.push_back({r->second.sdl_clave, r->second.indice_dispositivo});
			}

			lookup.insert(std::make_pair(i, resultado));
		};

		//Buscamos en cada uno de los mapas si algo se corresponde con
		//la clave de aplicación de entrada. Si existe se meterá
		//en el mapa de lookup el dispositivo y el valor SDL. La
		//lambda además actualiza el resultado para devolverlo.

		if(mapa_teclado.count(i))
		{
			f(mapa_teclado, Resultado_lookup::TECLADO, 0); 
		}
		else if(mapa_raton.count(i))
		{
			f(mapa_raton, Resultado_lookup::RATON, 0); 
		}
		else if(mapa_joystick.count(i))
		{
			//TODO: El indice del joystick!!! 
			f(mapa_joystick, Resultado_lookup::JOYSTICK, 0);
		}

		return resultado;
	}
}

bool Input::es_senal_salida() const
{
	return controles_sdl.es_senal_salida();
}

bool Input::es_input_down(unsigned int i) const
{
	Resultado_lookup rl=obtener(i);
	switch(rl.mapa)
	{
		case Resultado_lookup::TECLADO:
			for(auto val : rl.val) 
				if(controles_sdl.es_tecla_down(val.val)) return true;
		break;
		case Resultado_lookup::RATON:
			for(auto val : rl.val) 
				if(controles_sdl.es_boton_down(val.val)) return true;
		break;
		case Resultado_lookup::JOYSTICK:
			for(auto val : rl.val) 
				if(controles_sdl.es_joystick_boton_down(val.indice, val.val)) return true;
		break;
		default: break;
	}

	return false;
}

bool Input::es_input_up(unsigned int i) const
{
	Resultado_lookup rl=obtener(i);
	switch(rl.mapa)
	{
		case Resultado_lookup::TECLADO:
			for(auto val : rl.val) 
				if(controles_sdl.es_tecla_up(val.val)) return true;
		break;
		case Resultado_lookup::RATON:
			for(auto val : rl.val) 
				if(controles_sdl.es_boton_up(val.val)) return true;
		break;
		case Resultado_lookup::JOYSTICK:
			for(auto val : rl.val) 
				if(controles_sdl.es_joystick_boton_up(val.indice, val.val)) return true;
		break;
		default: break;
	}

	return false;
}

bool Input::es_input_pulsado(unsigned int i) const
{
	Resultado_lookup rl=obtener(i);
	switch(rl.mapa)
	{
		case Resultado_lookup::TECLADO:
			for(auto val : rl.val) 
				if(controles_sdl.es_tecla_pulsada(val.val)) return true;
		break;
		case Resultado_lookup::RATON:
			for(auto val : rl.val)
				if(controles_sdl.es_boton_pulsado(val.val)) return true;
		break;
		case Resultado_lookup::JOYSTICK:
			for(auto val : rl.val) 
				if(controles_sdl.es_joystick_boton_pulsado(val.indice, val.val)) return true;
		break;
		default: break;
	}

	return false;
}

//Esta es la función a la que se llama en primer lugar desde el Kernel. 
//Cargará todos los inputs registrados por la aplicación.
//Se mira primero el tipo de input y luego se hace corresponder la
//clave de aplicación con la clave SDL.

void Input::configurar(const std::vector<Par_input>& v)
{
	for(const auto& i : v)
	{
		tinput ti={i.sdl_clave, i.indice_dispositivo};
		auto par=std::make_pair(i.clave, ti);

		switch(i.tipo)
		{
			case Par_input::tipos::teclado: 	mapa_teclado.insert(par); break;
			case Par_input::tipos::raton:		mapa_raton.insert(par); break;
			case Par_input::tipos::joystick:	mapa_joystick.insert(par); break;
		}
	}
}
