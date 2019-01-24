#include "mensajes.h"

using namespace App;

Widget_mensajes::Widget_mensajes(const ldv::ttf_font& f, size_t maxm, int x, int y)
	:fuente(f), max_mensajes(maxm), x(x), y(x)
{

}

void Widget_mensajes::insertar_mensaje(const std::string& c, float t)
{
	if(mensajes.size()==max_mensajes)
	{
		mensajes.erase(std::begin(mensajes));
	}

	mensajes.push_back(Mensaje{fuente, c, t});
}

void Widget_mensajes::dibujar(ldv::screen& pantalla)
{
	int py=y;
	int margen=fuente.get_size()+2;

	for(auto& m : mensajes)
	{
		m.txt.go_to({x, py});
		m.txt.draw(pantalla);
		py+=margen;
	}
}

void Widget_mensajes::turno(float delta)
{
	auto it=std::begin(mensajes);

	while(it < std::end(mensajes))
	{
		it->tiempo-=delta;
		if(it->tiempo <=0.0f) it=mensajes.erase(it);
		else ++it;
	}
}
