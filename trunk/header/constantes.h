#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#include "rrt.h"





// MACROS

#define PRINTVAR(x) cout << #x << " = " << x << endl //imprime vari�vel x com seu nome e seu valor








// MEDIDAS

#define PI 3.14159265

#define SCREEN_WIDTH 640
#define ARENA_WIDTH_MM 7400
#define ARENA_HEIGHT_MM 5400

#define PIX_PER_MM SCREEN_WIDTH / ARENA_WIDTH_MM
#define MM_PER_PIX ARENA_WIDTH_MM / SCREEN_WIDTH


//Transforma��es de unidades
#define PIX_TO_MM(x) x * MM_PER_PIX
#define MM_TO_PIX(x) x * PIX_PER_MM

//unidades do pathplanning
#define CELLS_PER_MM  MAX_X / ARENA_WIDTH_MM //MAX_X e MAX_Y s�o as dimens�es da matriz que far� a abstra��o do campo
#define MM_PER_CELLS  ARENA_WIDTH_MM / MAX_X

#define MM_TO_CELLS(x) x * CELLS_PER_MM

#define CELLS_TO_MM(x) x * MM_PER_CELLS



#define ARENA_WIDTH SCREEN_WIDTH
#define ARENA_HEIGHT 5400 * PIX_PER_MM

#define FIELD_WIDTH 6050 * PIX_PER_MM
#define FIELD_HEIGHT 4050 * PIX_PER_MM

#define FIELD_CENTER_X FIELD_WIDTH/2 + BORDER
#define FIELD_CENTER_Y FIELD_HEIGHT/2 + BORDER

#define HALF_FIELD_RADIUS 500 * PIX_PER_MM

#define GOAL_LINE 350 * PIX_PER_MM
#define GOAL_CIRC_RADIUS 500 * PIX_PER_MM
#define GOAL_SIZE 700 * PIX_PER_MM

#define BORDER 675 * PIX_PER_MM


#define ROBOT_RADIUS_MM 90
#define BALL_RADIUS_MM 43

#define ROBOT_RADIUS ROBOT_RADIUS_MM * PIX_PER_MM
#define BALL_RADIUS BALL_RADIUS_MM * PIX_PER_MM / 2







//CONFIGURAÇÔES
//#define IS_VERBOSE 0






//CORES
#define DARK_GREEN   0.0, 0.5, 0.0
#define YELLOW  1, 1, 0
#define BLUE  0, 0, 1
#define CIANO  0, 1, 1
#define BLACK  0, 0, 0

#define TIMEOUT_INTERVAL 15


//c�digos ASCII de interface
enum{
	ENTER = 13,
	ESC = 27,
	U_A = 72,
	D_A = 80,
	L_A = 75,
	R_A = 77
};


//flags para o controle de callback do click de mouse
enum{
	CURSOR_EVENT_NOTHING,
	CURSOR_EVENT_PATHPLAN
};

//index da combobox dos tipos de pathplanning
enum{
	PATHPLAN_RRT    = 0,
	PATHPLAN_ASTAR  = 1
};




#endif
