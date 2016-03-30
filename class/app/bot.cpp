#include "bot.h"

using namespace App;

#include <herramientas/log_base/log_base.h>
extern DLibH::Log_base LOG;

//TODO: Hacer una limpieza masiva de todo esto... Está muy cerdo todo.

Bot::Bot()
	:destino(nullptr),
	estado(testados::buscando), angulo(0.0), angulo_anterior(0.0), 
	grados_a_girar(0.0),
	distancia_anterior(0.0), direccion_giro(0), velocidad(100.0), 
	t_disparo(0.0f), t_reevaluar(0.0f), t_comprobar_visual(0.0f),
	salud(40)
{
	formar_poligono();
}

void Bot::formar_poligono()
{
	poligono.insertar_vertice({20.0, 0.0});
	poligono.insertar_vertice({0.0, -7.0});
	poligono.insertar_vertice({0.0, 7.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Bot::turno(const Mapa& mapa, std::vector<Disparador>& vd, float delta)
{
	if(!destino)
	{
		return;
	}
	
	//TODO: Reevaluar a un blanco más cercano o a uno que esté a la vista...

	t_disparo+=delta;
	const auto& v=destino->acc_poligono().acc_centro();
	const auto& c=poligono.acc_centro();

	if(ruta.size())
	{
		t_comprobar_visual+=delta;
		t_reevaluar+=delta;

		//Cada segundo comprobamos si hay una línea hasta el jugador.
		//Si la hay, anularemos la ruta y pasaremos a otro estado.
		if(t_comprobar_visual > 1.0f)
		{
			t_comprobar_visual=0.0f;
			if(mapa.visibilidad_entre_puntos(c, v)) estado=testados::posicionar_ataque;
		}
	
		//Cada tres segundos evaluaremos de nuevo la ruta...
		if(t_reevaluar > 3.0f)
		{
			t_reevaluar=0.0f;
			trazar_ruta(mapa);
		}
	}

	//TODO: This sucks. El planteamiento en general no está muy bien.
	else if(estado!=testados::rotando)
	{
		if(mapa.visibilidad_entre_puntos(c, v)) estado=testados::posicionar_ataque;
		else estado=testados::buscando;
	}

	//Cada segundo evaluamos si podríamos disparar...	
	if(t_disparo > 1.0f)
	{
		t_disparo=0.0f;
		intento_disparo(mapa, vd);
	}

	switch(estado)
	{
		case testados::buscando:		trazar_ruta(mapa);break;
		case testados::rotando:			orientar_giro(delta); break;
		case testados::moviendo: 		mover(delta); break;
		case testados::posicionar_ataque: 	trazar_hasta_posicion_ataque(); break;
		case testados::nada: break;
	}

	for(const auto& o : mapa.obstaculos)
	{
		if(en_colision_con(o))
		{
			colisionar(mapa);
			break;
		}
	}
}

void Bot::intento_disparo(const Mapa& mapa, std::vector<Disparador>& vd)
{
	const auto& v=destino->acc_poligono().acc_centro();
	const auto& c=poligono.acc_centro();

	Vector_2d_cartesiano<double> vect=obtener_para_puntos_cartesiano(c.x, c.y, v.x, v.y);
	auto ang=vect.angulo_grados();
	if(ang < 0.0) ang+=360.0;
	auto dif=ang > angulo ? ang-angulo : angulo-ang;
		
	if(dif < 45.0 && mapa.visibilidad_entre_puntos(c, v))
	{
		Disparador res;

		auto pt=c;
		pt.x+=vect.x * 25.0f;
		pt.y+=vect.y * 25.0f;

		Herramientas_proyecto::Generador_int gen(-20, 20);
		//TODO: -1 como const.
		res.disparos.push_back({Disparador::tproyectiles::normal, -1, pt, ang+gen()});

		vd.push_back(res);				
	}
}

void Bot::trazar_ruta(const Mapa& mapa)
{
	ruta.clear();

	auto pt_fin=destino->acc_poligono().acc_centro();


//	const Nodo_ruta * ini=mapa.localizar_nodo_cercano(pt_ini);
	Punto_ruta pr{poligono.acc_centro(), false};
	auto ini=mapa.crear_inicio_temporal(pr);

	const Nodo_ruta * fin=mapa.localizar_nodo_cercano(pt_fin);
	if(!fin) return;

	Trazador_ruta t;
	auto res=t.trazar_ruta(ini, *fin);

	//Ni el punto de inicio ni el punto de fin se añaden a la ruta: 
	//el punto de inicio es donde estamos. El punto de fin es el blanco.
	//Supuestamente, cuando lleguemos, tendremos el punto fin a la vista!.

	if(res.resuelto) 
	{
		res.ruta.erase(std::begin(res.ruta)); //Eliminar el primer nodo: es la posición del bot.

		for(const auto& pt: res.ruta) 
		{
			if(!punto_en_poligono(poligono, pt)) 
			{
				ruta.push_back({pt.x, pt.y});
			}
		}

		calcular_angulo_destino(ruta.front());
		estado=testados::rotando;

//		LOG<<"TRAZADA RUTA CON "<<ruta.size()<<" PUNTOS"<<std::endl;
	}
	else
	{
//LOG<<"NO PUEDO ENCONTRAR NADA!!"<<std::endl;
	}

}

void Bot::orientar_giro(float delta)
{
	angulo_anterior=angulo;

	const double factor_giro=150.0;

	//Se calcula la cantidad de giro en este turno. 
	double giro=factor_giro * delta;

	//Obtener la cantidad de giro. Restar de los grados restantes.
	if(giro > grados_a_girar) giro=grados_a_girar;
	grados_a_girar-=giro;

	//Orientar con respecto a la dirección del giro...
	giro=giro*direccion_giro;
	angulo+=giro;

	poligono.rotar(giro);

	if(grados_a_girar <= 0.0) //Angulo de medio grado de márgen...
	{
		estado=testados::moviendo;
	}
}

void Bot::mover(float delta)
{
	posicion_anterior=poligono;
	desplazar_angulo_velocidad(angulo, velocidad*delta);

	auto pt=poligono.acc_centro();
	auto d=ruta.front();

	auto dist=pt.distancia_hasta(d);

	//Comparamos las distancias para saber cuando hemos llegado al final de la ruta.
	if(dist - distancia_anterior <= 0.0)
	{
		distancia_anterior=dist;
	}
	else
	{
		ruta.erase(std::begin(ruta));

//LOG<<"QUEDAN "<<ruta.size()<<" PUNTOS"<<std::endl;

		if(ruta.size())
		{
			calcular_angulo_destino(ruta.front());
			estado=testados::rotando;
		}
		else
		{
//LOG<<"DESTINO ALCANZADO"<<std::endl;
		}			
		//No hay else... Si se ha acabado en el siguiente loop haremos algo.
	}
}

void Bot::establecer_destino(const Espaciable& e)
{
	destino=&e;
	estado=testados::buscando;
}

void Bot::calcular_angulo_destino(DLibH::Punto_2d<double> dest)
{
	if(angulo > 360.0) angulo-=360.0;
	else if(angulo < 0.0) angulo+=360.0;

	auto pt=poligono.acc_centro();
	auto angulo_destino=obtener_para_puntos_cartesiano(pt.x, pt.y, dest.x, dest.y).angulo_grados();

	if(isnan(angulo_destino))
	{
		//TODO : ¿Como podemos hacer esto?. Forzar el siguiente punto de ruta?.
LOG<<"NAN "<<pt.x<<","<<pt.y<<" "<<dest.x<<","<<dest.y<<std::endl;
	}
	
	//Colocar el ángulo en formato 0 / 360 (está en -180 / 180).
	if(angulo_destino < 0.0) angulo_destino+=360.0;
	if(angulo_destino==360.0) angulo_destino=0.0;

	//Determinar la dirección del giro: lo que quede más cerca.
	if(angulo_destino > angulo) 
	{
		if(angulo_destino - angulo > 180.0) 
		{
			direccion_giro=-1;
			grados_a_girar=360.0 - angulo_destino + angulo;
		}
		else 
		{
			direccion_giro=1;
			grados_a_girar=angulo_destino-angulo;

		}
	}
	else
	{
		if(angulo - angulo_destino > 180.0) 
		{
			direccion_giro=-1;
			grados_a_girar=360.0+angulo_destino-angulo;
		}
		else 
		{
			direccion_giro=-1;
			grados_a_girar=angulo-angulo_destino;
		}
	}

//LOG<<"PARA IR DE "<<angulo<<" A "<<angulo_destino<<" "<<direccion_giro<<" PARA UN TOTAL DE "<<grados_a_girar<<std::endl;

	distancia_anterior=pt.distancia_hasta(dest);
}

void Bot::trazar_hasta_posicion_ataque()
{
//LOG<<"TRAZANDO POSICION ATAQUE..."<<std::endl;

	ruta.clear();
	auto pt_fin=destino->acc_poligono().acc_centro();
	auto c=poligono.acc_centro();

	//La posición de ataque debe estar a una cierta distancia del jugador.

	double dist=c.distancia_hasta(c);
	
	if(dist > 250.0)
	{
		Vector_2d_cartesiano<double> vect=obtener_para_puntos_cartesiano(pt_fin.x, pt_fin.y, c.x, c.y);
		pt_fin.x+=vect.x * 250.0f;
		pt_fin.y+=vect.y * 250.0f;
		ruta.push_back({pt_fin.x, pt_fin.y});
	}

	estado=testados::rotando;
	calcular_angulo_destino(pt_fin);
}

void Bot::colisionar(const Mapa& m)
{
//LOG<<"COLISION"<<std::endl;

	poligono=posicion_anterior;
	angulo=angulo_anterior;
	trazar_ruta(m);
}

void Bot::restar_salud(int v)
{
	salud-=v;
	if(salud < 0) salud=0;
}

void Bot::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	r.dibujar_poligono_lineas(pantalla, poligono, {255, 255, 255, 255}, camara);
}
