#ifndef CLASS_PATHPLAN
#define CLASS_PATHPLAN

#include <vector>

#include <gtk/gtk.h>

#include "point.h"
#include "guiPlayer.h"
#include "guiConstants.h"
#include "discretePathplan.h"


class GuiPathplan : public DiscretePathplan //, public ContinuousPathplan
{
	private:
		void drawObstacles();

	public:
		int pathplanIndex; //index of pathplan algorithms avaiable
		int checkPrintFull;
		int checkPrintObstacles;
		bool isDrawn;

		GuiPathplan();
		GuiPathplan(Point initialpos,int pathplanIndex,int checkPrintFull, int checkPrintObstacles);
		~GuiPathplan();

		//void runPathplan();
		void draw();
};


#endif
