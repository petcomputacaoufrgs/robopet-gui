#ifndef CLASS_PATHPLAN
#define CLASS_PATHPLAN

#include <vector>

#include <gtk/gtk.h>

#include "point.h"
#include "guiPlayer.h"
#include "guiConstants.h"
#include "pathplan.h"


class guiPathplan : public Pathplan
{
	private:
		void drawObstacles();

	public:
		int pathplanIndex; //index of pathplan algorithms avaiable
		int checkPrintFull;
		int checkPrintObstacles;
		bool isDrawn;

		guiPathplan();
		guiPathplan(Point initialpos,int pathplanIndex,int checkPrintFull, int checkPrintObstacles);
		~guiPathplan();

		//void runPathplan();
		void draw();
};


#endif
