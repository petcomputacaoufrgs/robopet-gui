#include <iostream>
#include <list>

#include "drawing.h"
#include "mainWindow.h"

#define drawLine(x1,y1,x2,y2) glBegin(GL_LINES); glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2)); glEnd();


void MainWindow::drawField()
{
	//cairo_set_line_width( cr, 1);
	
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
				cairo_set_source_rgb(cr, YELLOW);
			else
				cairo_set_source_rgb(cr, BLUE);
			game.players[team][i].draw(cr,i,displaySettings);
		}
}

void guiPlayer::draw(cairo_t *cr, int index, DisplaySettings settings)
{
    //if(this->hasUpdatedInfo) {
   
    double posx = MM_TO_PIX( this->getCurrentPosition().getX() ) + BORDER;
	double posy = MM_TO_PIX( this->getCurrentPosition().getY() ) + BORDER;
	
	if( !settings.isHidePlayerBody() )
		drawBody(cr, posx, posy);
	if( !settings.isHidePlayerAngle() )
		drawAngle(cr, posx, posy, this->getCurrentAngle());
	if( !settings.isHidePlayerIndex() )
		drawIndex(cr, posx, posy, index);
	if( !settings.isHidePlayerFuture() )
		drawVector(cr, posx, posy, MM_TO_PIX( this->getFuturePosition().getX() ) + BORDER,
							   MM_TO_PIX( this->getFuturePosition().getY() ) + BORDER);
    //}
}

void guiPlayer::drawBody(cairo_t *cr, float centerX, float centerY)
{
	cairo_arc(cr, centerX, centerY, ROBOT_RADIUS, 0, 2*M_PI);
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

void guiPlayer::drawIndex(cairo_t *cr, float centerX, float centerY, int robotNumber)
{
	/*cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 10.0);
	cairo_move_to(cr, centerX, centerY);
	
	char text[2];
	text[0] = robotNumber + 48;
	text[1] = '\0';
	cairo_show_text(cr, text);*/
}

void guiPlayer::drawVector(cairo_t *cr, float startX, float startY, float endX, float endY)
{
	double dashes[] = {5.0};
	int    ndash  = sizeof (dashes)/sizeof(dashes[0]);
	cairo_set_dash (cr, dashes, ndash, 0);

	cairo_set_line_width( cr, 1);
	cairo_move_to(cr, startX, startY);
	cairo_line_to(cr, endX, endY);
	cairo_stroke(cr);
	cairo_set_line_width( cr, 2);
	
	cairo_set_dash (cr, dashes, 0, 0);
}

void GuiBall::draw(cairo_t *cr, DisplaySettings settings)
{
	if( !settings.isHideBall() )
	{
		cairo_arc(cr, MM_TO_PIX( this->getCurrentPosition().getX() ) + BORDER, MM_TO_PIX(this->getCurrentPosition().getY()) + BORDER, BALL_RADIUS, 0, 2*M_PI);
		cairo_stroke (cr);
	}
}

void drawPath(cairo_t *cr, list<Node> path)
{
    //glBegin(GL_LINE_STRIP);
	for(std::list<Node>::iterator i = path.begin(); i != path.end(); i++)
		drawBox( cr, MM_TO_PIX( CELLS_TO_MM_X( i->getX() )) + BORDER,
				 MM_TO_PIX( CELLS_TO_MM_Y( i->getY() )) + BORDER,
				(ARENA_WIDTH_MM/MAX_X)/30 );
		//glVertex2f(MM_TO_PIX( CELLS_TO_MM( i->getX() )) , MM_TO_PIX( CELLS_TO_MM( i->getY() )) );
    //glEnd();
}

void MainWindow::drawObstacles()
{
	cairo_set_source_rgb( cr, RED);
	
    for(int i=0;i<MAX_X;i++)
        for(int k=0;k<MAX_Y;k++)
            if( pathplan->env[i][k] == OBSTACLE) 
            {
               drawBox( cr, MM_TO_PIX( CELLS_TO_MM_X(i) ) + BORDER,
						MM_TO_PIX( CELLS_TO_MM_Y(k) ) + BORDER,
						(ARENA_WIDTH_MM/MAX_X)/30 );
			}
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
		
		drawMatrixLimits( cr, BORDER, BORDER, MM_TO_PIX(CELLS_TO_MM_X(MAX_X)), MM_TO_PIX(CELLS_TO_MM_X(MAX_Y)) );
	
		if( getPrintFullPathplan() )
			drawPath(cr, pathplan->pathFull);
		cairo_set_source_rgb( cr, CIANO);
		drawPath(cr, pathplan->pathFinal);

		if( getPrintObstacles() )
			drawObstacles();
	}
}
