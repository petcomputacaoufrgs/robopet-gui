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


void drawField()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(ARENA_WIDTH/2, BORDER, 0);
	glVertex3f(ARENA_WIDTH/2, ARENA_HEIGHT - BORDER, 0);

	glVertex3f(BORDER, BORDER, 0);
	glVertex3f(BORDER, ARENA_HEIGHT - BORDER, 0);

	glVertex3f(ARENA_WIDTH - BORDER, BORDER, 0);
	glVertex3f(ARENA_WIDTH - BORDER, ARENA_HEIGHT - BORDER, 0);

	glVertex3f(BORDER, BORDER, 0);
	glVertex3f(ARENA_WIDTH - BORDER, BORDER, 0);

	glVertex3f(BORDER, ARENA_HEIGHT - BORDER, 0);
	glVertex3f(ARENA_WIDTH - BORDER, ARENA_HEIGHT - BORDER, 0);

	glVertex3f(BORDER + GOAL_CIRC_RADIUS, ARENA_HEIGHT/2 - GOAL_LINE/2, 0);
	glVertex3f(BORDER + GOAL_CIRC_RADIUS, ARENA_HEIGHT/2 + GOAL_LINE/2, 0);

	glVertex3f(ARENA_WIDTH - BORDER - GOAL_CIRC_RADIUS, ARENA_HEIGHT/2 - GOAL_LINE/2, 0);
	glVertex3f(ARENA_WIDTH - BORDER - GOAL_CIRC_RADIUS, ARENA_HEIGHT/2 + GOAL_LINE/2, 0);
	glEnd();

	drawQuarterCircle(BORDER, ARENA_HEIGHT/2 - GOAL_LINE/2, GOAL_CIRC_RADIUS, 4);
	drawQuarterCircle(BORDER, ARENA_HEIGHT/2 + GOAL_LINE/2, GOAL_CIRC_RADIUS, 1);

	drawQuarterCircle(ARENA_WIDTH - BORDER, ARENA_HEIGHT/2 - GOAL_LINE/2, GOAL_CIRC_RADIUS, 3);
	drawQuarterCircle(ARENA_WIDTH - BORDER, ARENA_HEIGHT/2 + GOAL_LINE/2, GOAL_CIRC_RADIUS, 2);
	drawCircle(ARENA_WIDTH/2, ARENA_HEIGHT/2, HALF_FIELD_RADIUS);
}
void MainWindow::drawPlayers()
{
	for(int team=0; team<2; team++)
		for(int i=0; i<game.getNplayers(team); i++) {
			team==0 ? glColor3f(YELLOW) : glColor3f(BLUE);
			game.players[team][i].draw(i,displaySettings);
		}
}

void guiPlayer::draw(int index, DisplaySettings settings)
{
    //if(this->hasUpdatedInfo) {
   
    double posx = MM_TO_PIX( this->getCurrentPosition().getX() ) + BORDER;
	double posy = MM_TO_PIX( this->getCurrentPosition().getY() ) + BORDER;
	
	if( !settings.isHidePlayerBody() )
		drawBody( posx, posy);
	if( !settings.isHidePlayerAngle() )
		drawAngle(posx, posy, this->getCurrentAngle());
	if( !settings.isHidePlayerIndex() )
		drawIndex(posx, posy, index);
	if( !settings.isHidePlayerFuture() )
		drawVector(posx, posy, MM_TO_PIX( this->getFuturePosition().getX() ) + BORDER,
							   MM_TO_PIX( this->getFuturePosition().getY() ) + BORDER);

    //}
}

void guiPlayer::drawBody(float centerX, float centerY)
{
	drawCircle(centerX, centerY, ROBOT_RADIUS);
}


void guiPlayer::drawAngle(float centerX, float centerY, float angle)
{
	float ang = -angle * 2 * M_PI / 360;

	drawLine(centerX , centerY,
			 centerX + cos(ang) * ROBOT_RADIUS * 1.2 , centerY + sin(ang) * ROBOT_RADIUS * 1.2);
}

void guiPlayer::drawIndex(float centerX, float centerY, int robotNumber)
{
	glColor3f(1, 1, 1);
	glRasterPos2f(centerX + ROBOT_RADIUS, centerY - ROBOT_RADIUS);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, robotNumber + 48);

	//char buffer[200];
	//sprintf(buffer, "%i\n pos: %i,%i", robotNumber, this->getCurrentPosition().getX(), this->getCurrentPosition().getY());        
	//glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, buffer);
}

void guiPlayer::drawVector(float startX, float startY, float endX, float endY)
{
	glColor3f(CIANO);
	glLineStipple(2, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);

	drawLine(startX, startY, endX, endY);
	
	glDisable(GL_LINE_STIPPLE);

	drawLine(endX, endY-ROBOT_RADIUS/2, endX, endY+ROBOT_RADIUS/2);
	drawLine(endX-ROBOT_RADIUS/2, endY, endX+ROBOT_RADIUS/2, endY);
}

void GuiBall::draw(DisplaySettings settings)
{
	if( !settings.isHideBall() )
	{
		glColor3f(1, 0.5, 0);
		drawCircle(MM_TO_PIX( this->getCurrentPosition().getX() ), MM_TO_PIX( this->getCurrentPosition().getY() ), BALL_RADIUS);
	}
}

void drawPath(list<Point> path)
{
    //glBegin(GL_LINE_STRIP);
	for(std::list<Point>::iterator i = path.begin(); i != path.end(); i++)
		drawBox( MM_TO_PIX( CELLS_TO_MM( i->getX() )) + BORDER,
				 MM_TO_PIX( CELLS_TO_MM( i->getY() )) + BORDER,
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
               drawBox( MM_TO_PIX( CELLS_TO_MM(i) ) + BORDER,
						MM_TO_PIX( CELLS_TO_MM(k) ) + BORDER,
						(ARENA_WIDTH_MM/MAX_X)/30 );;
			}                   
}

void MainWindow::drawPathplan()
{
	if( toDrawPathplan ) {
	
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
