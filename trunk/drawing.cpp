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
	int w = fieldWidth, h = fieldHeight;
	
	// WARNING: Hardcoded scaleFactor setting!
	scaleFactorLength = 1;
	scaleFactorWidth = 1;
	
	cairo_set_line_width( cr, 2);
	
	// green background
	cairo_set_source_rgb( cr, DARK_GREEN);
	cairo_rectangle (cr, 0, 0, w, h);
	cairo_fill (cr);
	
	// field lines	
	cairo_set_source_rgb( cr, WHITE);
		
	cairo_move_to(cr, w/2, BORDER_PIX);
	cairo_line_to(cr, w/2, h - BORDER_PIX);

	cairo_move_to(cr, BORDER_PIX, BORDER_PIX);
	cairo_line_to(cr, BORDER_PIX, h - BORDER_PIX);

	cairo_move_to(cr, w - BORDER_PIX, BORDER_PIX);
	cairo_line_to(cr, w - BORDER_PIX, h - BORDER_PIX);

	cairo_move_to(cr, BORDER_PIX, BORDER_PIX);
	cairo_line_to(cr, w - BORDER_PIX, BORDER_PIX);

	cairo_move_to(cr, BORDER_PIX, h - BORDER_PIX);
	cairo_line_to(cr, w - BORDER_PIX, h - BORDER_PIX);

	cairo_stroke (cr);
	
	// goal areas
	cairo_arc(cr, BORDER_PIX, h/2 - GOAL_LINE/2, GOAL_CIRC_RADIUS, -M_PI/2, 0);
	cairo_arc(cr, BORDER_PIX, h/2 + GOAL_LINE/2, GOAL_CIRC_RADIUS, 0, M_PI/2);
	cairo_stroke (cr);

	cairo_arc(cr, w - BORDER_PIX, h/2 + GOAL_LINE/2, GOAL_CIRC_RADIUS, M_PI/2, M_PI);
	cairo_arc(cr, w - BORDER_PIX, h/2 - GOAL_LINE/2, GOAL_CIRC_RADIUS, M_PI, -M_PI/2);
	cairo_stroke (cr);
	
	// field center
	cairo_arc(cr, w/2, h/2, HALF_FIELD_RADIUS, 0, 2*M_PI);
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
			drawPlayer( &game.players[team][i] );
		}
}

void MainWindow::drawPlayer(GuiPlayer* p)
{
    //if(this->hasUpdatedInfo) {
   
		double posx = MM_TO_PIX( p->getCurrentPosition().getX() ) + BORDER_PIX;
		double posy = MM_TO_PIX( p->getCurrentPosition().getY() ) + BORDER_PIX;
		
		cairo_set_line_width(cr, 2);
		
		if( !displaySettings.isHidePlayerBody() ) {
			// draw body circle
			cairo_arc(cr, posx, posy, ROBOT_RADIUS*scaleFactorLength, 0, 2*M_PI);
			cairo_stroke (cr);
			
			// draw angle
			float ang = -p->getCurrentAngle() * 2 * M_PI / 360;

			cairo_move_to(cr, posx , posy);
			cairo_line_to(cr, posx + cos(ang) * ROBOT_RADIUS*scaleFactorLength * 1.2 , posy + sin(ang) * ROBOT_RADIUS*scaleFactorLength * 1.2);
			cairo_stroke (cr);
		}		
		if( !displaySettings.isHidePlayerData() ) {
			char text[16], tmp[8];
			int fontSize = 10;
			cairo_set_font_size(cr, fontSize);
			
			cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
			cairo_move_to(cr, posx - fontSize - ROBOT_RADIUS*scaleFactorLength,
							  posy - ROBOT_RADIUS*scaleFactorLength);
			itoa(p->getId(),text,fontSize);
			cairo_show_text(cr, text);
			
			cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
			
			// angle data
			fontSize = 8;
			cairo_set_font_size(cr, fontSize);
			cairo_move_to(cr, posx + fontSize + ROBOT_RADIUS*scaleFactorLength,
							  posy - ROBOT_RADIUS*scaleFactorLength);
			itoa(p->getCurrentAngle(),text,10);
			strcat(text,"º");
			cairo_show_text(cr, text);
			
			// position data
			fontSize = 7;
			cairo_set_font_size(cr, fontSize);
			itoa(p->getCurrentPosition().getX(),text,10);
			strcat(text,",");
			itoa(p->getCurrentPosition().getY(),tmp,10);
			strcat(text,tmp);
			cairo_move_to(cr, posx - strlen(text)*fontSize/4,
							  posy + fontSize + ROBOT_RADIUS*scaleFactorLength);
			cairo_show_text(cr, text);
			
			//sprintf(text+strlen(text),"\n- %i: %.0f,%.0f (%.0f°)",game.players[1][i].getId(), game.players[1][i].getCurrentPosition().getX(),game.players[1][i].getCurrentPosition().getY(),);
			
			cairo_stroke (cr);
		}
		
		if( !displaySettings.isHidePlayerFuture() ) {
			if( p->getFuturePosition().getX()!=-1 && p->getFuturePosition().getY()!=-1 ) {
					float endX = MM_TO_PIX( p->getFuturePosition().getX() ) + BORDER_PIX;
					float endY = MM_TO_PIX( p->getFuturePosition().getY() ) + BORDER_PIX;
					
					double dashes[] = {2.5};
					int    ndash  = sizeof (dashes)/sizeof(dashes[0]);
					cairo_set_dash (cr, dashes, ndash, 0);
						cairo_set_line_width( cr, 1);
						cairo_move_to(cr, posx, posy);
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
		}
		
		if( !displaySettings.isHidePlayerPath() )
			drawLinedPath(p->path);
    
    //}
}


void drawCircle(cairo_t *cr, float posx, float posy, float radius)
{	
	cairo_arc(cr, posx, posy, radius, 0, 2*M_PI);
	cairo_stroke(cr);
}

void MainWindow::drawLinedPath(vector<Point> path)
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

void MainWindow::drawPath(vector<Point> path)
{
	for (unsigned int i=0; i<path.size(); i++)		
		drawCircle ( cr, MM_TO_PIX( path[i].getX() ) + BORDER_PIX,
					  MM_TO_PIX( path[i].getY() ) + BORDER_PIX,
				  3);
}

void MainWindow::drawBall()
{
	if( !displaySettings.isHideBall() ) {
		cairo_set_source_rgb(cr, ORANGE);
		cairo_arc(cr, MM_TO_PIX( game.ball.getCurrentPosition().getX() ) + BORDER_PIX,
					  MM_TO_PIX( game.ball.getCurrentPosition().getY() ) + BORDER_PIX,
					  BALL_RADIUS*scaleFactorLength, 0, 2*M_PI);
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

		// RRT only: full pathplan drawing
		if( getPrintFullPathplan() && gtk_toggle_button_get_active( (GtkToggleButton*)this->useRrt) ) {
			cairo_set_source_rgb(cr, BLACK);
			cairo_set_line_width( cr, 1);
			drawPath(((Rrt*)pathplan)->fullPath);
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
						   drawCircle( cr, MM_TO_PIX(i*(FIELD_WIDTH_MM/(float)pp->getEnvMatrixX())) + BORDER_PIX,
										MM_TO_PIX(k*(FIELD_HEIGHT_MM/(float)pp->getEnvMatrixY())) + BORDER_PIX,
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

			/*for(unsigned int i=0; i<g->obstacles.size(); i++)
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
			}*/

			cairo_set_line_width( cr, 2);
			cairo_set_source_rgb( cr, BLACK);
			
			/*float colors[8][3]={{WHITE},{YELLOW},{BLUE},{CIANO},{BLACK},{PURPLE}
			,{RED},{ORANGE}};*/
			
			if(gtk_spin_button_get_value_as_int((GtkSpinButton*)gstarPath)==-1)
			for(unsigned int i=0; i<g->paths.size(); i++)
			{
			//	cairo_set_source_rgb( cr, colors[i%8][0], colors[i%8][1], colors[i%8][2]);
				drawLinedPath(g->paths[i]);
			}
			else
				if(gtk_spin_button_get_value_as_int((GtkSpinButton*)gstarPath)<g->paths.size())
				{
					//cairo_set_source_rgb( cr, colors[gtk_spin_button_get_value_as_int((GtkSpinButton*)gstarPath)%8][0], colors[gtk_spin_button_get_value_as_int((GtkSpinButton*)gstarPath)%8][1], colors[gtk_spin_button_get_value_as_int((GtkSpinButton*)gstarPath)%8][2]);
					drawLinedPath(g->paths[gtk_spin_button_get_value_as_int((GtkSpinButton*)gstarPath)]);
				}
			
//			for(unsigned int i=0; i<g->path.size(); i+=2)
//			{
//				temp.clear();
//				temp.push_back(g->path[i]);
//				temp.push_back(g->path[i+1]);
//				drawLinedPath( cr, temp);
//			}
		}
		
		// draw the path
		cairo_set_source_rgb(cr, CIANO);
		drawLinedPath(pathplan->path);
		
	}
}
