#ifndef BLOQUE_INPUT_H
#define BLOQUE_INPUT_H

namespace App
{

struct Bloque_input
{
	int			giro;
	int			aceleracion;
	bool			disparo;
};

struct Traduccion_input
{
	int			arriba,
				abajo,
				izquierda,
				derecha,
				disparo;
};

}

#endif
