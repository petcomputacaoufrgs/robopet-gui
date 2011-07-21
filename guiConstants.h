#pragma once

#include "constants.h"



// MEASURES
#define HALF_FIELD_RADIUS_MM 500

#define GOAL_LINE_MM 350
#define GOAL_CIRC_RADIUS_MM 500
#define GOAL_SIZE_MM 700

#define BORDER_MM 675

#define FIELD_RATIOXY (ARENA_HEIGHT_MM/(float)ARENA_WIDTH_MM)


// COLORS
#define WHITE   1.0, 1.0, 1.0
#define DARK_GREEN   0.0, 0.5, 0.0
#define YELLOW  1, 1, 0
#define BLUE  0, 0, 1
#define CIANO  0, 1, 1
#define BLACK  0, 0, 0
#define PURPLE 0.5, 0, 0.5
#define RED 1, 0, 0
#define ORANGE 1, 0.5, 0


//flags para o controle de callback do click de mouse
enum{
	CURSOR_EVENT_NOTHING,
	CURSOR_EVENT_PATHPLAN,
	CURSOR_EVENT_ADD_BLUE_ROBOT,
	CURSOR_EVENT_ADD_YELLOW_ROBOT,
	CURSOR_EVENT_SET_BALL
};
