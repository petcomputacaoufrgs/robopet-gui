#include <iostream>
#include <list>

#include "drawing.h"
#include "mainWindow.h"
#include "rrt.h"


extern double scaleFactorLength;
extern double scaleFactorWidth;

void MainWindow::drawField()
{
	cairo_set_line_width( cr, 2);
	
	// green background
	cairo_set_source_rgb( cr, DARK_GREEN);
	cairo_rectangle (cr, 0, 0, ARENA_WIDTH, ARENA_HEIGHT);
	cairo_fill (cr);
	
	// field lines	
	cairo_set_source_rgb( cr, WHITE);
		
	cairo_move_to(cr, ARENA_WIDTH/2, BORDER);
	cairo_line_to(cr, ARENA_WIDTH/2, ARENA_HEIGHT - BORDER);

	cairo_move_to(cr, BORDER, BORDER);
	cairo_line_to(cr, BORDER, ARENA_HEIGHT - BORDER);

	cairo_move_to(cr, ARENA_WIDTH - BORDER, BORDER);
	cairo_line_to(cr, ARENA_WIDTH - BORDER, ARENA_HEIGHT - BORDER);

	cairo_move_to(cr, BORDER, BORDER);
	cairo_line_to(cr, ARENA_WIDTH - BORDER, BORDER);

	cairo_move_to(cr, BORDER, ARENA_HEIGHT - BORDER);
	cairo_line_to(cr, ARENA_WIDTH - BORDER, ARENA_HEIGHT - BORDER);

	cairo_stroke (cr);
	
	// goal areas
	cairo_arc(cr, BORDER, ARENA_HEIGHT/2 - GOAL_LINE/2, GOAL_CIRC_RADIUS, -M_PI/2, 0);
	cairo_arc(cr, BORDER, ARENA_HEIGHT/2 + GOAL_LINE/2, GOAL_CIRC_RADIUS, 0, M_PI/2);
	cairo_stroke (cr);

	cairo_arc(cr, ARENA_WIDTH - BORDER, ARENA_HEIGHT/2 + GOAL_LINE/2, GOAL_CIRC_RADIUS, M_PI/2, M_PI);
	cairo_arc(cr, ARENA_WIDTH - BORDER, ARENA_HEIGHT/2 - GOAL_LINE/2, GOAL_CIRC_RADIUS, M_PI, -M_PI/2);
	cairo_stroke (cr);
	
	// field center
	cairo_arc(cr, ARENA_WIDTH/2, ARENA_HEIGHT/2, HALF_FIELD_RADIUS, 0, 2*M_PI);
	cairo_stroke (cr);
	
}
void MainWindow::drawPlayers()
{
	for(int team=0; team<2; team++)
		for(int i=0; i<game.getNplayers(team); i++) {
			if(team==0)
				cairo_set_source_rgb(cr, BLUE);
			else
				cairo_set_source_rgb(cr, YELLOW);
			game.players[team][i].draw(cr, game.players[team][i].getId() ,displaySettings);
		}
}

void guiPlayer::draw(cairo_t *cr, int index, DisplaySettings settings)
{
    //if(this->hasUpdatedInfo) {
   
		double posx = MM_TO_PIX( this->getCurrentPosition().getX() ) + BORDER;
		double posy = MM_TO_PIX( this->getCurrentPosition().getY() ) + BORDER;
		
		cairo_set_line_width(cr, 2);
		
		if( !settings.isHidePlayerBody() )
			drawBody(cr, posx, posy);
		
		if( !settings.isHidePlayerAngle() )
			drawAngle(cr, posx, posy, this->getCurrentAngle());
		
		if( !settings.isHidePlayerIndex() )
			drawIndex(cr, posx, posy, index);
		
		if( !settings.isHidePlayerFuture() )
			if( this->getFuturePosition().getX()!=-1 && this->getFuturePosition().getY()!=-1 )
				drawVector(cr, posx, posy, MM_TO_PIX( this->getFuturePosition().getX() ) + BORDER,
									   MM_TO_PIX( this->getFuturePosition().getY() ) + BORDER);
		
		if( !settings.isHidePlayerPath() )
			drawLinedPath(cr, this->path);
    
    //}
}

void guiPlayer::drawBody(cairo_t *cr, float centerX, float centerY)
{
	cairo_arc(cr, centerX, centerY, ROBOT_RADIUS*scaleFactorLength, 0, 2*M_PI);
	cairo_stroke (cr);
}

void drawBox(cairo_t *cr, float centerX, float centerY, float side)
{	
	//cairo_rectangle(cr, centerX-side/2, centerY-side/2, centerX+side/2, centerY+side/2);
	cairo_arc(cr, centerX, centerY, ROBOT_RADIUS/3., 0, 2*M_PI);
	cairo_stroke(cr);
}

void guiPlayer::drawAngle(cairo_t *cr, float centerX, float centerY, float angle)
{
	float ang = -angle * 2 * M_PI / 360;

	cairo_move_to(cr, centerX , centerY);
	cairo_line_to(cr, centerX + cos(ang) * ROBOT_RADIUS * 1.2 , centerY + sin(ang) * ROBOT_RADIUS * 1.2);
	cairo_stroke (cr);
}

void drawLinedPath(cairo_t *cr, vector<Point> path)
{
	if(path.size()>0) {
		 
		cairo_set_line_width (cr, 1);
		
		cairo_move_to (cr, MM_TO_PIX( path[0].getX() ) + BORDER,
							MM_TO_PIX( path[0].getY() ) + BORDER );
									
		for (unsigned int i=0; i<path.size(); i++) {
					
					cairo_line_to (cr, MM_TO_PIX( path[i].getX() ) + BORDER,
									  MM_TO_PIX( path[i].getY() ) + BORDER);
					
					drawBox ( cr, MM_TO_PIX( path[i].getX() ) + BORDER,
							 MM_TO_PIX( path[i].getY() ) + BORDER,
							2 );
							
					if (i != path.size()-1)
						cairo_move_to (cr, MM_TO_PIX( path[i].getX() ) + BORDER,
										  MM_TO_PIX( path[i].getY() ) + BORDER);
				}
	}
}

char* itoa(int value, char* result, int base) {
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

void guiPlayer::drawIndex(cairo_t *cr, float centerX, float centerY, int robotNumber)
{
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 10.0);
	cairo_move_to(cr, centerX, centerY+17);
	
	char text[3];
	itoa(robotNumber,text,10);
	cairo_show_text(cr, text);
	
	cairo_stroke (cr);
}

void guiPlayer::drawVector(cairo_t *cr, float startX, float startY, float endX, float endY)
{
	double dashes[] = {2.5};
	int    ndash  = sizeof (dashes)/sizeof(dashes[0]);
	cairo_set_dash (cr, dashes, ndash, 0);
		cairo_set_line_width( cr, 1);
		cairo_move_to(cr, startX, startY);
		cairo_line_to(cr, endX, endY);
		cairo_stroke(cr);	
	cairo_set_dash (cr, dashes, 0, 0);
	
	#define CROSS_SIZE 3
	cairo_move_to(cr, endX-CROSS_SIZE, endY-CROSS_SIZE);
	cairo_line_to(cr, endX+CROSS_SIZE, endY+CROSS_SIZE);
	cairo_move_to(cr, endX+CROSS_SIZE, endY-CROSS_SIZE);
	cairo_line_to(cr, endX-CROSS_SIZE, endY+CROSS_SIZE);
	cairo_stroke(cr);
	
	cairo_set_line_width( cr, 2);
}

void GuiBall::draw(cairo_t *cr, DisplaySettings settings)
{
	if( !settings.isHideBall() )
	{
		cairo_arc(cr, MM_TO_PIX( this->getCurrentPosition().getX() ) + BORDER, MM_TO_PIX(this->getCurrentPosition().getY()) + BORDER, BALL_RADIUS, 0, 2*M_PI);
		cairo_stroke (cr);
	}
}

void drawPath(cairo_t *cr, list<Point> path)
{
    //glBegin(GL_LINE_STRIP);
	/*for(std::list<Point>::iterator i = path.begin(); i != path.end(); i++)
		drawBox( cr, MM_TO_PIX( CELLS_TO_MM_X( i->getX() )) + BORDER,
				 MM_TO_PIX( CELLS_TO_MM_Y( i->getY() )) + BORDER,
				3 );*/
		//glVertex2f(MM_TO_PIX( CELLS_TO_MM( i->getX() )) , MM_TO_PIX( CELLS_TO_MM( i->getY() )) );
    //glEnd();
}

void MainWindow::drawObstacles()
{

}

void drawMatrixLimits(cairo_t *cr, float x1, float y1, float x2, float y2)
{
	// draw Pathplanning matrix limits
	double dashes[] = {10.0};
	int    ndash  = sizeof (dashes)/sizeof(dashes[0]);
	
	cairo_set_dash (cr, dashes, ndash, 0);
	cairo_set_source_rgb(cr, BLACK);
	cairo_rectangle(cr, x1, y1, x2, y2 );
	cairo_stroke(cr);
	
	cairo_set_dash (cr, dashes, 0, 0);
}

void MainWindow::drawPathplan()
{
	if( toDrawPathplan ) {
		
		// limits of the environment matrix
		drawMatrixLimits( 	cr, BORDER, BORDER,
							MM_TO_PIX(pathplan->getEnvMatrixX()),
							MM_TO_PIX(pathplan->getEnvMatrixY()) );
	
		// full pathplan
		if( getPrintFullPathplan() ) {
			cairo_set_source_rgb(cr, BLACK);
			for(unsigned int i = 0; i < ((Rrt*)pathplan)->fullPath.size(); i++)
				drawBox( cr, MM_TO_PIX( ((Rrt*)pathplan)->fullPath[i].getX() ) + BORDER,
							 MM_TO_PIX( ((Rrt*)pathplan)->fullPath[i].getY() ) + BORDER,
						3 );
		}
		
		// pathplan solution
		cairo_set_source_rgb(cr, CIANO);
		drawLinedPath(cr, pathplan->path);
		for(unsigned int i = 0; i < pathplan->path.size(); i++)
				drawBox( cr, MM_TO_PIX( pathplan->path[i].getX() ) + BORDER,
							 MM_TO_PIX( pathplan->path[i].getY() ) + BORDER,
						3 );
		drawLinedPath(cr, pathplan->path);

		// obstacles
		if( getPrintObstacles() ) {
			cairo_set_line_width( cr, 2);
			cairo_set_source_rgb( cr, RED);
			for(int i=0;i<pathplan->getEnvMatrixX();i++)
				for(int k=0;k<pathplan->getEnvMatrixY();k++)
					if( pathplan->env[i][k] == OBSTACLE)  {
					   drawBox( cr, MM_TO_PIX(i*(FIELD_WIDTH/(float)pathplan->getEnvMatrixX())) + BORDER,
									MM_TO_PIX(k*(FIELD_HEIGHT/(float)pathplan->getEnvMatrixY())) + BORDER,
								3 );
					}
		}
	}
}
