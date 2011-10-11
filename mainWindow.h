#pragma once


#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "guiConstants.h"
#include "drawing.h"
#include "guiPlayer.h"
#include "guiBall.h"
#include "guiPathplan.h"
#include "displaySettings.h"
#include "communication.h"
#include "game.h"

#include "pathplan.h"

#include "rp_client.h"
#include "rp_server.h"

#include <string>
#include <vector>



using namespace std;



class MainWindow
{
	//GTK Callbacks
	friend void timeoutAdd(GtkWidget *widget, MainWindow* mw);
	friend void timeoutRemove(GtkWidget *widget, MainWindow* mw);
	friend gint expose_event(GtkWidget *widget, GdkEventExpose *event, MainWindow* mw);
	friend gint configure_event(GtkWidget *widget, GdkEventConfigure *event, MainWindow* mw);
	friend void realize(GtkWidget *widget, MainWindow* mw);

	//Interface Callbacks
	// these functions aren't methods of the class because GTK does not accept this (they must be out of and namespace).
	friend void pathplanButton(GtkWidget *widget, gpointer data);
	friend void setBolaPos(GtkWidget *widget, gpointer data);
	friend void button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer data); //controle de cliques do mouse
	friend void key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer data);
	friend void setBallButton(GtkWidget *widget, gpointer data);
	friend void addYellowPlayerButton(GtkWidget *widget, gpointer data);
	friend void addBluePlayerButton(GtkWidget *widget, gpointer data);
	friend void clientCommunicationButton(GtkWidget *widget, gpointer data);
	friend void serverCommunicationButton(GtkWidget *widget, gpointer data);
	friend void openJoystick(GtkWidget *widget, gpointer data);
	friend void loadStateButton(GtkWidget *widget, gpointer data);
	friend void deletePlayerButton(GtkWidget *widget, gpointer data);
	friend void sizeRequestCB(GtkWidget *widget, GdkEventExpose *event, MainWindow* mw);
	friend void expanderCB(GtkWidget *widget, MainWindow* mw);
	friend void validatePathplanButtonCB(GtkWidget *widget, gpointer data);
	friend void iterateCB(MainWindow *mw);
	friend void updateSceneCB(MainWindow* mw);
	
	public:
		MainWindow();
		~MainWindow() {};

		//Interface functions
		GuiPlayer		*getSelectedPlayer();
		void 			pushStatusMessage(string msg);
		void 			fillTextOutput(char text[]);
		int 			getStepsize();
		int 			getPrintFullPathplan();
		int				getPrintObstacles();
		
		//Communication (communication.cpp)
		void 			communicate();
		void 			openClient(int port);
		void 			closeClient();
		void 			openServer(int port, char* host);
		void 			closeServer();

		Game 			game;
		Pathplan*		pathplan;
		GuiPathplan		pathplanSettings;
		DisplaySettings displaySettings;
		
		bool 			isVerbose;
		
	private:
            
		GtkWidget *window;
		GtkWidget *soccer_field;
		GdkPixmap *pixmap;
		int fieldWidth, fieldHeight;
		
		double BORDER_PIX;
		double GOAL_LINE;
		double GOAL_CIRC_RADIUS;
		double HALF_FIELD_RADIUS;
		double ROBOT_RADIUS;
		double BALL_RADIUS;

		void updateScene();
		void createDrawingArea();
		void iterate();
        void generateTextOutput();
		void createInterface();
		void joystick();
		
		//Drawing functions
		void drawWorld();
		void drawRobot(float centerX, float centerY, float angle, int robotNumber);
		void drawBall(float centerX, float centerY);
		void drawField();
		void drawPositionMarker(int x, int y);
        void drawObstacles();
		void drawPlayers();
		void drawPathplan();
		void drawBall();
		void drawPlayer(GuiPlayer* p);
		void drawLinedPath(vector<Point> path);
		void drawPath(vector<Point> path);
		
		// TRANSFORMATIONS
		double PIX_TO_MM(double x);
		double MM_TO_PIX(double x);

		//Interface variables
		int 			cursorEvent;
		GtkWidget* 		TextOutput;
		GtkWidget* 		statusBar;
		GtkWidget* 		stepsize;
		GtkWidget* 		pathplanBox;
		GtkWidget* 		printFullPathplan;
		GtkWidget* 		printObstacles;
		GtkWidget		*clientHost, *serverHost;
		GtkWidget		*clientPort, *serverPort;
		GtkWidget		*pathplanGridX, *pathplanGridY;
		GtkWidget*		obstaculesRadius;
		GtkWidget*		validatePathplanButton;
		int 			joystickFd;
		GtkWidget		*rrtTimeLimit, *rrtGoalProb, *rrtStepsize, *gstarPath;

		GtkWidget		*useRrt, *useAstar, *useGstar, *useDijkstra;
		cairo_t 		*cr;

		//Communication (communication.cpp)
		RoboPETClient 	*aitoguiClient;
		void 			listenToAI();
		RoboPETServer 	*guitoaiServer;
		void 			sendToAI();
};


//structure for passing parameters in GTK interface callbacks
struct parametersType {
	vector<GtkWidget*> widgets;
	MainWindow *mw;
};
