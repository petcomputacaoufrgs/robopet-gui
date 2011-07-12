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
	friend void updateSceneCB(GtkWidget *widget, GdkEventExpose *event, MainWindow* mw);
	friend void deletePlayerButton(GtkWidget *widget, gpointer data);
	
	public:
		MainWindow();
		~MainWindow() {};

		//Interface functions
		guiPlayer		*getSelectedPlayer();
		void 			pushStatusMessage(string msg);
		void 			fillTextOutput(char text[]);
		int 			getStepsize();
		int 			getPrintFullPathplan();
		int				getPrintObstacles();
		
		//Communication (communication.cpp)
		void 			communicate();
		void 			openClient(int port, char* host);
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
		guint timeout_handler_id;
		//GtkWidget *statusBar;

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
		int 			joystickFd;
		GtkWidget		*rrtTimeLimit, *rrtGoalProb, *rrtStepsize, *gstarPath;

		GtkWidget		*useRrt, *useAstar, *useGstar;
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
