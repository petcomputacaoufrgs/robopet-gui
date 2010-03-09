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

#include <string>
#include <vector>


//#include "joystick\joystick.h"



using namespace std;




class MainWindow
{
	//Callbacks
	friend void timeoutAdd(GtkWidget *widget, MainWindow* mw);
	friend void timeoutRemove(GtkWidget *widget, MainWindow* mw);
	friend gboolean renderScene(GtkWidget * widget, GdkEvent *event, gpointer data);
	friend gboolean exposeEvent(GtkWidget * widget, GdkEvent *event, gpointer data);
	friend gboolean configureEvent(GtkWidget *widget, MainWindow* mw, GdkEventConfigure *event, gpointer data);
	friend void realize(GtkWidget *widget,  MainWindow* mw, gpointer data);

	//Callbacks de Interface (onde p�r essas biroscas???)
	friend void pathplanButton(GtkWidget *widget, gpointer data);
	friend void playerManualControl(GtkWidget *widget, gpointer data);
	friend void setBolaPos(GtkWidget *widget, gpointer data);
	friend void button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer data); //controle de cliques do mouse


	//Fim Callbacks
	private:
		GtkWidget *window;
		GtkWidget *soccer_field;
		GdkGLConfig *glconfig;
		guint timeout_handler_id;
		GtkWidget *statusBar;


		void configuraGL();
		void createDrawingArea();
		void createMWindow(string title);
		void drawWorld();
		void drawPlayers();

		void createInterface();


		//globais feias
		int cursorEvent; //indica um "estado" da interface pendente de um clique do mouse

		//Callback estranha, conferir se precisa mesmo:
		//gboolean visibilityNotifyEvent(GtkWidget *widget, GdkEventVisibility *event, gpointer data);

	public:
		MainWindow();
		~MainWindow() {};
		MainWindow(string title);

                void openCommunication(int port, char* host);
                void closeCommunication();
                void listenToAI();

                GUIClient * communicationClient;

		//itens de jogo
		GuiBall ball;
		GUIPlayer playersTeam1[MAX_JOGADORES];
		GUIPlayer playersTeam2[MAX_JOGADORES];
		Pathplan pathplan; //armazena configura��es referentes ao pathplanning
		DisplaySettings displaySettings;

};


//estrutura para passagem de par�metros nas callbacks da interface
struct typeParameters {
	vector<GtkWidget*> widgets;
	MainWindow *mw;
};





#endif
