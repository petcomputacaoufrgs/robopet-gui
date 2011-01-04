#include <iostream>
#include <list>

#include "drawing.h"
#include "mainWindow.h"

#define drawLine(x1,y1,x2,y2) glBegin(GL_LINES); glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2)); glEnd();




void drawQuarterCircle(float centerX, float centerY, float radius, int quadrante)
{
    const int q = 1000;
    float ang = 2 * M_PI / q;
   
    glBegin(GL_LINE_STRIP);
        for(int i = (quadrante - 1) * q / 4; i <= quadrante * q/4; i++)
        {
            glVertex2f(centerX + cos(i * ang) * radius, centerY + sin(i * ang) * radius);
        }
    glEnd();
}

void drawCircle(float centerX, float centerY, float radius)
{
	for(int i=1; i<5; i++)
		drawQuarterCircle(centerX, centerY, radius, i);
}

void drawBox(float centerX, float centerY, float side)
{
	glBegin(GL_LINE_LOOP);
            glVertex2f(centerX - side/2, centerY - side/2);
            glVertex2f(centerX + side/2 -1, centerY - side/2);
            glVertex2f(centerX + side/2 -1, centerY + side/2 -1);
            glVertex2f(centerX - side/2, centerY + side/2 -1);
        glEnd();
}


void MainWindow::drawField()
{
	cairo_t *cr = gdk_cairo_create( pixmap );
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
	cairo_t *cr = gdk_cairo_create( pixmap );
	
	for(int team=0; team<2; team++)
		for(int i=0; i<game.getNplayers(team); i++) {
			cairo_set_source_rgb( cr, team==0?YELLOW:BLUE );
			game.players[team][i].draw(cr,i,displaySettings);
		}
}

void guiPlayer::draw(cairo_t *cr, int index, DisplaySettings settings)
{
    //if(this->hasUpdatedInfo) {
   
    double posx = MM_TO_PIX( this->getCurrentPosition().getX() ) + BORDER;
	double posy = MM_TO_PIX( this->getCurrentPosition().getY() ) + BORDER;
	
	if( !settings.isHidePlayerBody() )
		drawBody( cr, posx, posy);
	if( !settings.isHidePlayerAngle() )
		drawAngle( cr, posx, posy, this->getCurrentAngle());
	if( !settings.isHidePlayerIndex() )
		drawIndex( cr, posx, posy, index);
	if( !settings.isHidePlayerFuture() )
		drawVector( cr, posx, posy, MM_TO_PIX( this->getFuturePosition().getX() ) + BORDER,
							   MM_TO_PIX( this->getFuturePosition().getY() ) + BORDER);

    //}
}

void guiPlayer::drawBody(cairo_t *cr, float centerX, float centerY)
{
	cairo_arc(cr, centerX, centerY, ROBOT_RADIUS, 0, 2*M_PI);
	cairo_stroke (cr);
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
	glColor3f(1, 1, 1);
	glRasterPos2f(centerX + ROBOT_RADIUS, centerY - ROBOT_RADIUS);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, robotNumber + 48);

	//char buffer[200];
	//sprintf(buffer, "%i\n pos: %i,%i", robotNumber, this->getCurrentPosition().getX(), this->getCurrentPosition().getY());        
	//glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, buffer);
}

void guiPlayer::drawVector(cairo_t *cr, float startX, float startY, float endX, float endY)
{
	glColor3f(CIANO);
	glLineStipple(2, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);

	drawLine(startX, startY, endX, endY);
	
	glDisable(GL_LINE_STIPPLE);

	drawLine(endX, endY-ROBOT_RADIUS/2, endX, endY+ROBOT_RADIUS/2);
	drawLine(endX-ROBOT_RADIUS/2, endY, endX+ROBOT_RADIUS/2, endY);
}

void GuiBall::draw(cairo_t *cr, DisplaySettings settings)
{
	if( !settings.isHideBall() )
	{
		cairo_arc(cr, MM_TO_PIX( this->getCurrentPosition().getX() ) + BORDER, MM_TO_PIX(this->getCurrentPosition().getY()) + BORDER, BALL_RADIUS, 0, 2*M_PI);
		cairo_stroke (cr);
	}
}

void drawPath(list<Node> path)
{
    //glBegin(GL_LINE_STRIP);
	for(std::list<Node>::iterator i = path.begin(); i != path.end(); i++)
		drawBox( MM_TO_PIX( CELLS_TO_MM_X( i->getX() )) + BORDER,
				 MM_TO_PIX( CELLS_TO_MM_Y( i->getY() )) + BORDER,
				(ARENA_WIDTH_MM/MAX_X)/30 );
		//glVertex2f(MM_TO_PIX( CELLS_TO_MM( i->getX() )) , MM_TO_PIX( CELLS_TO_MM( i->getY() )) );
    //glEnd();
}

void MainWindow::drawObstacles()
{
    for(int i=0;i<MAX_X;i++)
        for(int k=0;k<MAX_Y;k++)
            if( pathplan->env[i][k] == OBSTACLE) 
            {
               drawBox( MM_TO_PIX( CELLS_TO_MM_X(i) ) + BORDER,
						MM_TO_PIX( CELLS_TO_MM_Y(k) ) + BORDER,
						(ARENA_WIDTH_MM/MAX_X)/30 );;
			}                   
}

void MainWindow::drawPathplan()
{
	if( toDrawPathplan ) {
	
		// draw Pathplanning Envirnomnet
		glBegin(GL_LINE_LOOP);
            glVertex2f( MM_TO_PIX(CELLS_TO_MM_X(0)) + BORDER, MM_TO_PIX(CELLS_TO_MM_X(0)) + BORDER);
            glVertex2f( MM_TO_PIX(CELLS_TO_MM_X(MAX_X)) + BORDER, MM_TO_PIX(CELLS_TO_MM_X(0)) + BORDER );
            glVertex2f( MM_TO_PIX(CELLS_TO_MM_X(MAX_X)) + BORDER, MM_TO_PIX(CELLS_TO_MM_X(MAX_Y)) + BORDER );
            glVertex2f( MM_TO_PIX(CELLS_TO_MM_X(0)) + BORDER, MM_TO_PIX(CELLS_TO_MM_X(MAX_Y)) + BORDER);
        glEnd();
	
		if( getPrintFullPathplan() ) {
			glColor3f(BLACK);
			drawPath(pathplan->pathFull);
		}
		glColor3f(CIANO);
		drawPath(pathplan->pathFinal);

		if( getPrintObstacles() ) {
			glColor3f(RED);
			drawObstacles();
		}
	}
}
