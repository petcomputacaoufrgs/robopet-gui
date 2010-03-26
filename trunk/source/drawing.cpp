#include <iostream>
#include <list>

#include "drawing.h"
#include "mainWindow.h"

#define drawLine(x1,y1,x2,y2) glBegin(GL_LINES); glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2)); glEnd();




void drawQuarterCircle(float centerX, float centerY, float radius, int quadrante)
{
	const int q = 1000;
    float ang = 2 * PI / q;
   
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
	vector<GUIPlayer>::iterator it;
        it = game.playersTeam1.begin();
        for(int i=0; it<game.playersTeam1.end(); it++, i++) {
		glColor3f(YELLOW);
		(*it).draw(i,displaySettings);
	}

        it = game.playersTeam2.begin();
        for(int i=0; it<game.playersTeam2.end(); it++, i++) {
		glColor3f(BLUE);
		(*it).draw(i,displaySettings);
	}
}

void GUIPlayer::draw(int index, DisplaySettings settings)
{
    //if(this->hasUpdatedInfo) {

        double posx = MM_TO_PIX( this->getCurrentPosition().getX() );
	double posy = MM_TO_PIX( this->getCurrentPosition().getY() );


	if( settings.isCheckPlayerBody() )
		drawBody( posx, posy);
	if( settings.isCheckPlayerAngle() )
		drawAngle(posx, posy, this->getCurrentAngle());
	if( settings.isCheckPlayerIndex() )
		drawIndex(posx, posy, index);
	if( settings.isCheckPlayerFuture() )
		drawVector(posx, posy, MM_TO_PIX( this->getFuturePosition().getX() ) , MM_TO_PIX( this->getFuturePosition().getY() ));

    //}
}

void GUIPlayer::drawBody(float centerX, float centerY)
{
	drawCircle(centerX, centerY, ROBOT_RADIUS);
}


void GUIPlayer::drawAngle(float centerX, float centerY, float angle)
{
	float ang = angle * 2 * PI / 360;

	drawLine(centerX , centerY,
			 centerX + cos(ang) * ROBOT_RADIUS * 1.2 , centerY + sin(ang) * ROBOT_RADIUS * 1.2);
}

void GUIPlayer::drawIndex(float centerX, float centerY, int robotNumber)
{
	glColor3f(1, 1, 1);
	glRasterPos2f(centerX + ROBOT_RADIUS, centerY - ROBOT_RADIUS);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, robotNumber + 48);
}

void GUIPlayer::drawVector(float startX, float startY, float endX, float endY)
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
	if( settings.isCheckBallShow() )
	{
		glColor3f(1, 0.5, 0);
		drawCircle(MM_TO_PIX( this->getCurrentPosition().getX() ), MM_TO_PIX( this->getCurrentPosition().getY() ), BALL_RADIUS);
	}
}


void drawPath(list<Point> path)
{
	//glBegin(GL_LINE_STRIP);

	for(std::list<state>::iterator i = path.begin(); i != path.end(); i++){
		
		drawCircle(MM_TO_PIX( CELLS_TO_MM( i->getX() )) , MM_TO_PIX( CELLS_TO_MM( i->getY() )), 1);
		//glVertex2f(MM_TO_PIX( CELLS_TO_MM( i->getX() )) , MM_TO_PIX( CELLS_TO_MM( i->getY() )) );
	}
	
	//glEnd();
}


void Pathplan::drawObstacles()
{
	for(int i=0;i<MAX_X;i++)
			for(int k=0;k<MAX_Y;k++)
				if( env[i][k] == OBSTACULO)
					drawCircle(MM_TO_PIX (CELLS_TO_MM( i )), MM_TO_PIX(CELLS_TO_MM( k )), BALL_RADIUS*2);
}

void Pathplan::draw()
{
	if( this->isDrawn ){ //flag que diz se � pra estar imprimindo o ultimo pathplanning configurado


		//itera na lista de nodos caminhoSolucao
		if(this->checkPrintFull){
			glColor3f(BLACK);
			drawPath(pathFull);
		}
		glColor3f(CIANO);
		drawPath(pathFinal);

		//drawObstacles();


		//imprime posi��es final e inicial
		drawCircle(MM_TO_PIX( this->initialpos.getX() ), MM_TO_PIX( this->initialpos.getY() ), BALL_RADIUS*2);
		drawCircle(MM_TO_PIX( this->initialpos.getX() ), MM_TO_PIX( this->initialpos.getY() ), BALL_RADIUS*4);
		drawCircle(MM_TO_PIX( this->finalpos.getX() ), MM_TO_PIX( this->finalpos.getY() ), BALL_RADIUS*2);
		drawCircle(MM_TO_PIX( this->finalpos.getX() ), MM_TO_PIX( this->finalpos.getY() ), BALL_RADIUS*4);
	}
}