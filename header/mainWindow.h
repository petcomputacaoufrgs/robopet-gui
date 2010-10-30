#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_


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

#include "rp_client.h"
#include "rp_server.h"

#include <string>
#include <vector>


//#include "joystick\joystick.h"



using namespace std;




class MainWindow
{
	//GTK Callbacks
	friend void timeoutAdd(GtkWidget *widget, MainWindow* mw);
	friend void timeoutRemove(GtkWidget *widget, MainWindow* mw);
	friend gboolean renderScene(GtkWidget * widget, GdkEvent *event, gpointer data);
	friend gboolean exposeEvent(GtkWidget * widget, GdkEvent *event, gpointer data);
	friend gboolean configureEvent(GtkWidget *widget, MainWindow* mw, GdkEventConfigure *event, gpointer data);
	friend void realize(GtkWidget *widget,  MainWindow* mw, gpointer data);

	//Interface Callbacks
	friend void pathplanButton(GtkWidget *widget, gpointer data);
	friend void setBolaPos(GtkWidget *widget, gpointer data);
	friend void button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer data); //controle de cliques do mouse
        friend void key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer data);

        friend GtkWidget* createLateralMenu(MainWindow* mw);

        private:
            
		GtkWidget *window;
		GtkWidget *soccer_field;
		GdkGLConfig *glconfig;
		guint timeout_handler_id;
		//GtkWidget *statusBar;


		void configuraGL();
		void createDrawingArea();
		void createMWindow(string title);
		void drawWorld();
		void drawPlayers();
                void iterate();
                void generateTextOutput();

		void createInterface();



		//Interface variables
		int cursorEvent;
                GtkWidget* TextOutput;
                GtkWidget* statusBar;

                //Interface functions
                pair<int,int> getSelectedPlayer();
                void pushStatusMessage(string msg);
                void fillTextOutput(char text[]);


                //Communication (communication.cpp)
                RoboPETClient *aitoguiClient;
                void listenToAI();
                RoboPETServer *guitoaiServer;
                void sendToAI();

	public:
		MainWindow();
		~MainWindow() {};
		MainWindow(string title);

                
                //Communication (communication.cpp)
                void communicate();
                void openClient(int port, char* host);
                void closeClient();
                void openServer(int port, char* host);
                void closeServer();
                

                Game game; //game things
		guiPathplan pathplan; //pathplanning configurations (interface between GUI and PathPlanners Codes)
		DisplaySettings displaySettings; //configurations about the information to display on the field
                bool isVerbose;

};


//structure for passing parameters in GTK interface callbacks
struct parametersType {
	vector<GtkWidget*> widgets;
	MainWindow *mw;
};


#endif
