#include "exportador.h"

using namespace App;

tools::dnot_token::t_vector Exportador::generar_punto(double x, double y)
{
	using namespace tools;
	dnot_token::t_vector arr;
	arr.push_back(dnot_token(x));
	arr.push_back(dnot_token(y));
	return arr;
}

tools::dnot_token::t_vector Exportador::generar_color(int r, int g, int b, int a)
{
	using namespace tools;
	dnot_token::t_vector arr;
	arr.push_back(dnot_token(r));
	arr.push_back(dnot_token(g));
	arr.push_back(dnot_token(b));
	arr.push_back(dnot_token(a));
	return arr;
}

tools::dnot_token Exportador::serializar_obstaculo(const Obstaculo& o)
{
	using namespace tools;

	dnot_token::t_vector puntos;
	for(const auto& v : o.acc_poligono().get_vertices())
		puntos.push_back(dnot_token(generar_punto(v.x, v.y)));

	auto centro=o.acc_poligono().get_rotation_center();

	dnot_token::t_map propiedades;

	int tipo=0;
	switch(o.acc_tipo())
	{
		case Obstaculo::ttipos::normal: tipo=0; break;
		case Obstaculo::ttipos::letal: tipo=1; break;
		case Obstaculo::ttipos::inocuo: tipo=2; break;
	}

	propiedades["t"].set(tipo);

	dnot_token::t_map mapa_objeto;
	mapa_objeto["p"].set(puntos);
	mapa_objeto["cen"].set(generar_punto(centro.x, centro.y));
	mapa_objeto["pr"].set(propiedades);

	return dnot_token(mapa_objeto);
}

tools::dnot_token Exportador::serializar_decoracion(const Decoracion& o)
{
	using namespace tools;

	dnot_token::t_vector puntos;
	for(const auto& v : o.acc_poligono().get_vertices())
		puntos.push_back(dnot_token(generar_punto(v.x, v.y)));

	auto centro=o.acc_poligono().get_rotation_center();
	auto color=o.acc_color();
	auto colorl=o.acc_color_linea();

	dnot_token::t_map mapa_polig;
	mapa_polig["p"].set(puntos);
	mapa_polig["cen"].set(generar_punto(centro.x, centro.y));

	auto ncolor=generar_color(color.r, color.g, color.b, color.a);
	auto ncolorl=generar_color(colorl.r, colorl.g, colorl.b, colorl.a);

	dnot_token::t_vector arr_colores;
	arr_colores.push_back(dnot_token(ncolor));
	arr_colores.push_back(dnot_token(ncolorl));

	dnot_token::t_map propiedades;
	propiedades["fr"].set(o.es_frente());
	propiedades["pr"].set(o.acc_profundidad());

	mapa_polig["col"].set(arr_colores);
	mapa_polig["pr"].set(propiedades);

	return dnot_token(mapa_polig);
}

tools::dnot_token Exportador::serializar_punto(ldt::point_2d<double> pt)
{
	using namespace tools;

	dnot_token::t_vector puntos;
	puntos.push_back(dnot_token(pt.x));
	puntos.push_back(dnot_token(pt.y));

	return dnot_token(puntos);
}

tools::dnot_token Exportador::serializar_generador(const Generador_items& it)
{
	using namespace tools;
	auto c=it.acc_poligono().get_rotation_center();

	dnot_token::t_vector puntos;
	puntos.push_back(dnot_token(c.x));
	puntos.push_back(dnot_token(c.y));

	return dnot_token(puntos);
}

std::string Exportador::serializar(const std::vector<Obstaculo>& obs,
		const std::vector<Decoracion>& decs,
		const std::vector<ldt::point_2d<double>>& inicios,
		const std::vector<ldt::point_2d<double>>& bots,
		const std::vector<Punto_ruta>& puntos_ruta,
		const std::vector<Generador_items>& generadores)
{
	using namespace tools;

	dnot_token::t_vector vobstaculos;
	for(const auto& o : obs) vobstaculos.push_back(serializar_obstaculo(o));

	dnot_token::t_vector vdecoraciones;
	for(const auto& d : decs) vdecoraciones.push_back(serializar_decoracion(d));

	dnot_token::t_vector vinicios;
	for(const auto& i : inicios) vinicios.push_back(serializar_punto(i));

	dnot_token::t_vector vbots;
	for(const auto& i : bots) vbots.push_back(serializar_punto(i));

	dnot_token::t_vector vpuntosruta;
	for(const auto& i : puntos_ruta)  vpuntosruta.push_back(serializar_punto(i.pt));

	dnot_token::t_vector vgeneradores;
	for(const auto& g : generadores) vgeneradores.push_back(serializar_generador(g));

	dnot_token tok_obstaculos(vobstaculos);
	dnot_token tok_decoraciones(vdecoraciones);
	dnot_token tok_inicios(vinicios);
	dnot_token tok_bots(vbots);
	dnot_token tok_puntos_ruta(vpuntosruta);
	dnot_token tok_generadores(vgeneradores);

	dnot_token::t_map mapa_final;
	mapa_final["geometria"]=tok_obstaculos;
	mapa_final["decoracion"]=tok_decoraciones;
	mapa_final["inicios"]=tok_inicios;
	mapa_final["bots"]=tok_bots;
	mapa_final["puntos_ruta"]=tok_puntos_ruta;
	mapa_final["generadores"]=tok_generadores;

	dnot_token base(mapa_final);
	return base.serialize();
}
