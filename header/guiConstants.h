#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#include "constants.h"



// MACROS
#define PRINTVAR(x) cout << #x << " = " << x << endl //imprime vari�vel x com seu nome e seu valor


// MEASURES
#define SCREEN_WIDTH 640


// COLORS
#define DARK_GREEN   0.0, 0.5, 0.0
#define YELLOW  1, 1, 0
#define BLUE  0, 0, 1
#define CIANO  0, 1, 1
#define BLACK  0, 0, 0

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
	CURSOR_EVENT_PATHPLAN
};



#endif
