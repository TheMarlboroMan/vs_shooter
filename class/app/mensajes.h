#ifndef MENSAJES_H
#define MENSAJES_H

#include <string>
#include <vector>

#include <def_video.h>

namespace App
{

class Widget_mensajes
{
	public:

					Widget_mensajes(const ldv::ttf_font&, size_t, int, int);
	void				insertar_mensaje(const std::string&, float);
	void				dibujar(ldv::screen&);
	void				turno(float);

	private:

	struct Mensaje
	{
		Mensaje(const ldv::ttf_font& f, const std::string& c, float t)
			:tiempo(t), txt(f, {255, 255, 255, 255}, c)
		{}

		float						tiempo;
		ldv::ttf_representation 	txt;
	};

	const ldv::ttf_font&	fuente;
	std::vector<Mensaje> 		mensajes;
	size_t				max_mensajes;
	int 				x, y;
};

}

#endif
