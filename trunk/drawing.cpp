#include <iostream>
#include <list>

#include "drawing.h"
#include "mainWindow.h"
#include "rrt.h"
#include "gstar.h"


extern double scaleFactorLength;
extern double scaleFactorWidth;

void MainWindow::drawField()
{
	// WARNING: Hardcoded scaleFactor setting!
	scaleFactorLength = 1;
	scaleFactorWidth = 1;
	
	cairo_set_line_width( cr, 2);
	
	// green background
	cairo_set_source_rgb( cr, DARK_GREEN);
	cairo_rectangle (cr, 0, 0, ARENA_WIDTH, ARENA_HEIGHT);
	cairo_fill (cr);
	
	// field lines	
	cairo_set_source_rgb( cr, WHITE);
		
	cairo_move_to(cr, ARENA_WIDTH/2, BORDER_PIX);
	cairo_line_to(cr, ARENA_WIDTH/2, ARENA_HEIGHT - BORDER_PIX);

	cairo_move_to(cr, BORDER_PIX, BORDER_PIX);
	cairo_line_to(cr, BORDER_PIX, ARENA_HEIGHT - BORDER_PIX);

	cairo_move_to(cr, ARENA_WIDTH - BORDER_PIX, BORDER_PIX);
	cairo_line_to(cr, ARENA_WIDTH - BORDER_PIX, ARENA_HEIGHT - BORDER_PIX);

	cairo_move_to(cr, BORDER_PIX, BORDER_PIX);
	cairo_line_to(cr, ARENA_WIDTH - BORDER_PIX, BORDER_PIX);

	cairo_move_to(cr, BORDER_PIX, ARENA_HEIGHT - BORDER_PIX);
	cairo_line_to(cr, ARENA_WIDTH - BORDER_PIX, ARENA_HEIGHT - BORDER_PIX);

	cairo_stroke (cr);
	
	// goal areas
	cairo_arc(cr, BORDER_PIX, ARENA_HEIGHT/2 - GOAL_LINE/2, GOAL_CIRC_RADIUS, -M_PI/2, 0);
	cairo_arc(cr, BORDER_PIX, ARENA_HEIGHT/2 + GOAL_LINE/2, GOAL_CIRC_RADIUS, 0, M_PI/2);
	cairo_stroke (cr);

	cairo_arc(cr, ARENA_WIDTH - BORDER_PIX, ARENA_HEIGHT/2 + GOAL_LINE/2, GOAL_CIRC_RADIUS, M_PI/2, M_PI);
	cairo_arc(cr, ARENA_WIDTH - BORDER_PIX, ARENA_HEIGHT/2 - GOAL_LINE/2, GOAL_CIRC_RADIUS, M_PI, -M_PI/2);
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
				cairo_set_source_rgb(cr, YELLOW);
			else
				cairo_set_source_rgb(cr, BLUE);
			game.players[team][i].draw(cr, game.players[team][i].getId() ,displaySettings);
		}
}

void guiPlayer::draw(cairo_t *cr, int index, DisplaySettings settings)
{
    //if(this->hasUpdatedInfo) {
   
		double posx = MM_TO_PIX( this->getCurrentPosition().getX() ) + BORDER_PIX;
		double posy = MM_TO_PIX( this->getCurrentPosition().getY() ) + BORDER_PIX;
		
		cairo_set_line_width(cr, 2);
		
		if( !settings.isHidePlayerBody() )
			drawBody(cr, posx, posy);
		
		if( !settings.isHidePlayerAngle() )
			drawAngle(cr, posx, posy, this->getCurrentAngle());
		
		if( !settings.isHidePlayerIndex() )
			drawIndex(cr, posx, posy, index);
		
		if( !settings.isHidePlayerFuture() )
			if( this->getFuturePosition().getX()!=-1 && this->getFuturePosition().getY()!=-1 )
				drawVector(cr, posx, posy, MM_TO_PIX( this->getFuturePosition().getX() ) + BORDER_PIX,
									   MM_TO_PIX( this->getFuturePosition().getY() ) + BORDER_PIX);
		
		if( !settings.isHidePlayerPath() )
			drawLinedPath(cr, this->path);
    
    //}
}

void guiPlayer::drawBody(cairo_t *cr, float centerX, float centerY)
{
	cairo_arc(cr, centerX, centerY, ROBOT_RADIUS*scaleFactorLength, 0, 2*M_PI);
	cairo_stroke (cr);
}

void drawCircle(cairo_t *cr, float centerX, float centerY, float radius)
{	
	cairo_arc(cr, centerX, centerY, radius, 0, 2*M_PI);
	cairo_stroke(cr);
}

void guiPlayer::drawAngle(cairo_t *cr, float centerX, float centerY, float angle)
{
	float ang = -angle * 2 * M_PI / 360;

	cairo_move_to(cr, centerX , centerY);
	cairo_line_to(cr, centerX + cos(ang) * ROBOT_RADIUS*scaleFactorLength * 1.2 , centerY + sin(ang) * ROBOT_RADIUS*scaleFactorLength * 1.2);
	cairo_stroke (cr);
}

void drawLinedPath(cairo_t *cr, vector<Point> path)
{
	if(path.size()>0) {
		 
		cairo_set_line_width (cr, 1);
		
		cairo_move_to (cr, MM_TO_PIX( path[0].getX() ) + BORDER_PIX,
							MM_TO_PIX( path[0].getY() ) + BORDER_PIX );
									
		for (unsigned int i=0; i<path.size(); i++) {
					
			cairo_line_to (cr, MM_TO_PIX( path[i].getX() ) + BORDER_PIX,
							  MM_TO_PIX( path[i].getY() ) + BORDER_PIX);
			
			drawCircle ( cr, MM_TO_PIX( path[i].getX() ) + BORDER_PIX,
						  MM_TO_PIX( path[i].getY() ) + BORDER_PIX,
					  3);
					
			if (i != path.size()-1)
				cairo_move_to (cr, MM_TO_PIX( path[i].getX() ) + BORDER_PIX,
								   MM_TO_PIX( path[i].getY() ) + BORDER_PIX);
		}
	}
}

void drawPath(cairo_t *cr, vector<Point> path)
{
	for (unsigned int i=0; i<path.size(); i++)		
		drawCircle ( cr, MM_TO_PIX( path[i].getX() ) + BORDER_PIX,
					  MM_TO_PIX( path[i].getY() ) + BORDER_PIX,
				  3);
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
	cairo_move_to(cr, centerX, centerY + 1.5*ROBOT_RADIUS*scaleFactorLength);
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
	if( !settings.isHideBall() ) {
		cairo_arc(cr, MM_TO_PIX( this->getCurrentPosition().getX() ) + BORDER_PIX, MM_TO_PIX(this->getCurrentPosition().getY()) + BORDER_PIX, BALL_RADIUS*scaleFactorLength, 0, 2*M_PI);
		cairo_fill(cr);
	}
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
	if( pathplanSettings.toDraw ) {
			
		// draw the path
		cairo_set_source_rgb(cr, CIANO);
		drawLinedPath(cr, pathplan->path);

		// RRT only: full pathplan drawing
		if( getPrintFullPathplan() && gtk_toggle_button_get_active( (GtkToggleButton*)this->useRrt) ) {
			cairo_set_source_rgb(cr, BLACK);
			cairo_set_line_width( cr, 1);
			drawPath(cr, ((Rrt*)pathplan)->fullPath);
		}
		
		// grid-based pathplans only drawings
		if( pathplanSettings.isGridBased ) {
			
			DiscretePathplan* pp = (DiscretePathplan*) pathplan;

			// limits of the environment matrix
			drawMatrixLimits( 	cr, BORDER_PIX, BORDER_PIX,
								MM_TO_PIX(pp->getEnvMatrixX()),
								MM_TO_PIX(pp->getEnvMatrixY()) );
			// draw obstacles
			if( getPrintObstacles() ) {
				cairo_set_line_width( cr, 2);
				cairo_set_source_rgb( cr, RED);
				for(int i=0;i<pp->getEnvMatrixX();i++)
					for(int k=0;k<pp->getEnvMatrixY();k++)
						if( pp->env[i][k] == OBSTACLE)  {
						   drawCircle( cr, MM_TO_PIX(i*(FIELD_WIDTH/(float)pp->getEnvMatrixX())) + BORDER_PIX,
										MM_TO_PIX(k*(FIELD_HEIGHT/(float)pp->getEnvMatrixY())) + BORDER_PIX,
									3 );
						}
			}
		}
		else//GStar
		{
			GStar* g = (GStar*) pathplan;
			
			Obstacle o;
			vector<Point> temp;

			cairo_set_line_width( cr, 2);
			cairo_set_source_rgb( cr, BLACK);

			for(int i=0; i<g->obstacles.size(); i++)
			{
				o = g->getObstacle(i);

				if(o.p[0].getX()!=-1)
				{
					for(int j=0; j<4; j++)
					{
						//o.p[j].setX(((o.center.getX()-o.p[j].getX())*scaleFactorLength)+o.center.getX());
						//o.p[j].setY(((o.center.getY()-o.p[j].getY())*scaleFactorWidth)+o.center.getY());
						drawCircle( cr, MM_TO_PIX(o.p[j].getX()) + BORDER_PIX, MM_TO_PIX(o.p[j].getY()) + BORDER_PIX, 1);
					}
				}
			}

			cairo_set_line_width( cr, 2);
			cairo_set_source_rgb( cr, RED);

//			for(unsigned int i=0; i<g->path.size(); i+=2)
//			{
//				temp.clear();
//				temp.push_back(g->path[i]);
//				temp.push_back(g->path[i+1]);
//				drawLinedPath( cr, temp);
//			}
		}
	}
}
