#ifndef CLASS_PATHPLAN
#define CLASS_PATHPLAN

#include <vector>

#include <gtk/gtk.h>


class GuiPathplan
{
	public:
		GuiPathplan();
		~GuiPathplan();
		
		bool toDraw;
		bool isGridBased;

		void draw();
		
	private:
		void drawObstacles();
};


#endif
