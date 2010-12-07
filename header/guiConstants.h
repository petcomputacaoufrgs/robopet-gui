#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#include "constants.h"



// MACROS
#define PRINTVAR(x) cout << #x << " = " << x << endl //imprime vari�vel x com seu nome e seu valor


// MEASURES
#define SCREEN_WIDTH 640

#define ARENA_WIDTH SCREEN_WIDTH
#define ARENA_HEIGHT 5400 * PIX_PER_MM

#define PIX_PER_MM (SCREEN_WIDTH) / (ARENA_WIDTH_MM)
#define MM_PER_PIX (ARENA_WIDTH_MM) / (SCREEN_WIDTH)

#define HALF_FIELD_RADIUS 500 * PIX_PER_MM

#define GOAL_LINE 350 * PIX_PER_MM
#define GOAL_CIRC_RADIUS 500 * PIX_PER_MM
#define GOAL_SIZE 700 * PIX_PER_MM

#define BORDER_MM 675
#define BORDER (BORDER_MM * PIX_PER_MM)

#define ROBOT_RADIUS ROBOT_RADIUS_MM * PIX_PER_MM
#define BALL_RADIUS BALL_RADIUS_MM * PIX_PER_MM / 2

// TRANSFORMATIONS
#define PIX_TO_MM(x) (x * MM_PER_PIX)
#define MM_TO_PIX(x) (x * PIX_PER_MM)


// COLORS
#define DARK_GREEN   0.0, 0.5, 0.0
#define YELLOW  1, 1, 0
#define BLUE  0, 0, 1
#define CIANO  0, 1, 1
#define BLACK  0, 0, 0
#define PURPLE 0.5, 0, 0.5
#define RED 1, 0, 0

#define TIMEOUT_INTERVAL 15


//ASCII
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
	CURSOR_EVENT_PATHPLAN,
	CURSOR_EVENT_ADD_BLUE_ROBOT,
	CURSOR_EVENT_ADD_YELLOW_ROBOT
};



#endif
