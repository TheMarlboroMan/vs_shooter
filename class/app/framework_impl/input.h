#ifndef STRUCT_INPUT_FRAMEWORK_H
#define STRUCT_INPUT_FRAMEWORK_H

namespace App
{

struct Input
{
	enum inputs{
escape=0,
tab,

//Editor...

tecla_1,
tecla_2,
tecla_3,
tecla_4,
ayuda,
cambio_logica,
seleccion_color,
copiar_color,
pegar_color,
cargar_mapa,
grabar_mapa,
cursor_izquierda,
cursor_derecha,
cursor_arriba,
cursor_abajo,
suprimir,
control_izquierdo,
zoom_mas,
zoom_menos,
grid_mas,
grid_menos,
profundidad_mas,
profundidad_menos,
click_i,
click_d,

//De juego...
cambio_controles,
j1_disparo,
j1_habilidad,
j1_izquierda,
j1_derecha,
j1_arriba,
j1_abajo,
j2_disparo,
j2_habilidad,
j2_izquierda,
j2_derecha,
j2_arriba,
j2_abajo,
j3_disparo,
j3_habilidad,
j3_izquierda,
j3_derecha,
j3_arriba,
j3_abajo,

//Tests...
TEST_VISIBILIDAD,
NUEVO_BOT
};

};

}

#endif
