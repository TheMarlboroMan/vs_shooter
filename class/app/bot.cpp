#include "bot.h"

using namespace App;

Bot::Bot()
	:destino(nullptr),
	estado(testados::buscando), angulo(0.0), velocidad(100.0)
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

void Bot::turno(const Mapa& mapa, float delta)
{
	switch(estado)
	{
		case testados::buscando:
			trazar_ruta(mapa);
		break;
		
		case testados::rotando:
			orientar_giro(delta);
		break;
	
		case testados::moviendo:
			mover(delta);
		break;
	
		case testados::nada:

		break;
	}
}

void Bot::trazar_ruta(const Mapa& mapa)
{
	if(!destino)
	{
		return;
	}

	ruta.clear();

	auto 	pt_ini=poligono.acc_centro(),
		pt_fin=destino->acc_poligono().acc_centro();

	const Nodo_ruta * ini=mapa.localizar_nodo_cercano(pt_ini);
	if(!ini) return;

	const Nodo_ruta * fin=mapa.localizar_nodo_cercano(pt_fin);
	if(!fin) return;

	Trazador_ruta t;
	auto res=t.trazar_ruta(*ini, *fin);

	if(res.resuelto) 
	{
		if(!punto_en_poligono(poligono, pt_ini)) ruta.push_back(pt_ini);
		for(const auto& pt: res.ruta) 
		{
			if(!punto_en_poligono(poligono, pt)) 
				ruta.push_back({pt.x, pt.y});
		}
		ruta.push_back(pt_fin);
		estado=testados::rotando;	
	}

}

void Bot::orientar_giro(float delta)
{
	auto pt=poligono.acc_centro();
	auto dest=ruta.front();

	Vector_2d_cartesiano<double> v=obtener_para_puntos_cartesiano(pt.x, pt.y, dest.x, dest.y);

	auto ang=v.angulo_grados();

	const double factor_giro=150.0;
	double giro=factor_giro * delta;

	if(ang > angulo)
	{
		angulo+=giro;
		if(angulo >= ang) 
		{
			angulo=ang;
			giro-=angulo-ang;
			estado=testados::moviendo;
		}
		poligono.rotar(giro);

	}
	else
	{
		angulo-=giro;
		if(angulo <= ang) 
		{
			angulo=ang;
			giro-=ang-angulo;
			estado=testados::moviendo;
		}
		poligono.rotar(-giro);
	}
}

void Bot::mover(float delta)
{
	desplazar_angulo_velocidad(angulo, velocidad*delta);
	auto d=ruta.front();

	if(punto_en_poligono(poligono, d))
	{
		ruta.erase(std::begin(ruta));

		if(ruta.size())
		{
			auto pt=poligono.acc_centro();
			auto dest=ruta.front();

			Vector_2d_cartesiano<double> v=obtener_para_puntos_cartesiano(pt.x, pt.y, dest.x, dest.y);
			auto ang=v.angulo_grados();

			std::cout<<"NUEVO ANGULO "<<ang<<std::endl;


			estado=testados::rotando;
		}
		else
		{
			std::cout<<"FINALIZADO!!!"<<std::endl;
//			destino=nullptr;
//			estado=testados::nada;
			estado=testados::buscando;
		}
	}
}

void Bot::establecer_destino(const Espaciable& e)
{
	destino=&e;
	estado=testados::buscando;
}
