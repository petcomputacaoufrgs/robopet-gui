#ifndef CLASS_PATHPLAN
#define CLASS_PATHPLAN

#include <vector>

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#include "point.h"
#include "GUIPlayer.h"
#include "constantes.h"

#include "rrt.h"



class Pathplan
{
	private:
		list<state> pathFull;
		list<state> pathFinal;



		void fillEnv_playerBox(int centerx, int centery, int safetyCells);
		void drawObstacles();

		void runRRT();

	public:
		//posi��es s�o em milimetros
		Point initialpos;
		Point finalpos;
		int pathplanIndex; //indice referente ao algoritmo que foi pedido
		int checkPrintFull;
		bool isDrawn;

		int env[MAX_X][MAX_Y];



		//FUN��ES
		Pathplan();
		Pathplan(Point initialpos,int pathplanIndex,int checkPrintFull);
		~Pathplan();
		
		void runPathplan();

		void draw();
		void fillEnv(vector<GUIPlayer> playersTeam1,vector<GUIPlayer> playersTeam2);



};


#endif