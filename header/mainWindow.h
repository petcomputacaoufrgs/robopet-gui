#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_


#include <stdlib.h>
#include <math.h>
#include "constantes.h"


#include <glut.h>
#include "drawing.h"
#include "GUIPlayer.h"
#include "GuiBall.h"
//#include "interface.h"
#include "pathplan.h"
#include "displaySettings.h"
#include "point.h"
#include "communication.h"
#include "game.h"

#include "ssl_client.h"
#include "ssl_server.h"

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
	friend void playerManualControl(GtkWidget *widget, gpointer data);
	friend void setBolaPos(GtkWidget *widget, gpointer data);
	friend void button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer data); //controle de cliques do mouse

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
                void generateOutput();

		void createInterface();



		//Interface variables
		int cursorEvent; //indica um "estado" da interface pendente de um clique do mouse
                GtkWidget* textView;
                GtkWidget* statusBar;

                //Interface functions
                int getSelectedPlayer(int &playerTeam);
                void pushStatusMessage(string msg);
                void fillTextView(char text[]);


                //Communication (communication.cpp)
                RoboCupSSLClient *aitoguiClient;
                void listenToAI();
                RoboCupSSLServer *guitoaiServer;
                void sendToAI();

	public:
		MainWindow();
		~MainWindow() {};
		MainWindow(string title);

                
                //Communication (communication.cpp)
                void comunicate();
                void openClient(int port, char* host);
                void closeClient();
                void openServer(int port, char* host);
                void closeServer();
                

                Game game; //game things
		Pathplan pathplan; //pathplanning configurations (interface between GUI and PathPlanners Codes)
		DisplaySettings displaySettings; //configurations about the information to display on the field
                bool isVerbose;

};


//estrutura para passagem de par�metros nas callbacks da interface
struct typeParameters {
	vector<GtkWidget*> widgets;
	MainWindow *mw;
};





#endif
