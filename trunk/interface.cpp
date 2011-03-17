#include<iostream>
#include <fstream>

#include "gdk/gdkkeysyms.h"

#include "interface.h"
#include "rrt.h"
#include "astar.h"
#include "game.h"

#define CLIENT_INITIAL_PORT PORT_AI_TO_GUI
#define CLIENT_INITIAL_HOST IP_AI_TO_GUI

#define SERVER_INITIAL_PORT PORT_GUI_TO_AI
#define SERVER_INITIAL_HOST IP_GUI_TO_AI




//////////////////////////////////////////////////
///////////////               ////////////////////
///////////////   callbacks   ////////////////////
///////////////               ////////////////////
//////////////////////////////////////////////////


void button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer data)
//CALLBACK which captures mouse clicks
{
	MainWindow* mw = (MainWindow*) data;
	
	if (event->button == 1 && mw->cursorEvent != CURSOR_EVENT_NOTHING){

		if( ( event->x < ARENA_WIDTH - BORDER) && (event->x > BORDER) 
			&& (event->y < ARENA_HEIGHT - BORDER) && (event->y > BORDER)) { 
	  
		    switch(mw->cursorEvent) {

				case CURSOR_EVENT_PATHPLAN:	
						mw->pathplan->setFinalPos( Point(PIX_TO_MM(event->x)-BORDER_MM, PIX_TO_MM(event->y)-BORDER_MM) ); //convert screen coordinates into mm, which is the what setFinalPos receives
						mw->pathplan->run();
					
						mw->toDrawPathplan = true;
						//mw->cursorEvent = CURSOR_EVENT_NOTHING;
						break;
					
				case CURSOR_EVENT_ADD_YELLOW_ROBOT: 
						mw->game.addPlayer( 0, Point(PIX_TO_MM(event->x)-BORDER_MM,PIX_TO_MM(event->y)-BORDER_MM) );
						mw->cursorEvent = CURSOR_EVENT_NOTHING;
						break;
					
				case CURSOR_EVENT_ADD_BLUE_ROBOT:
						mw->game.addPlayer( 1, Point(PIX_TO_MM(event->x)-BORDER_MM,PIX_TO_MM(event->y)-BORDER_MM) );
						mw->cursorEvent = CURSOR_EVENT_NOTHING;
						break;
						
				case CURSOR_EVENT_SET_BALL:
						mw->game.ball.setCurrentPosition(Point(PIX_TO_MM(event->x)-BORDER_MM, PIX_TO_MM(event->y)-BORDER_MM));
						mw->cursorEvent = CURSOR_EVENT_NOTHING;
						break;
			}
		} 
	}
	
	//gtk_widget_draw(widget, NULL);
}

void key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer data)
//CALLBACK which captures keyboard keys
{
    MainWindow* mw = (MainWindow*) data;

    //add players
	switch(  event->keyval  )
	{
		case GDK_z: mw->cursorEvent = CURSOR_EVENT_ADD_YELLOW_ROBOT;
		break;
		 
		case GDK_x: mw->cursorEvent = CURSOR_EVENT_ADD_BLUE_ROBOT;
		break;
	}
        
    
    // players movement
    int stepsize = mw->getStepsize();

    guiPlayer *selected = mw->getSelectedPlayer();

    if( selected ) {
        switch(  event->keyval  ) {
            case GDK_s: selected->setCurrentPositionY( selected->getCurrentPosition().getY() + stepsize ); break;

            case GDK_w: selected->setCurrentPositionY( selected->getCurrentPosition().getY() - stepsize ); break;

            case GDK_a: selected->setCurrentPositionX( selected->getCurrentPosition().getX() - stepsize ); break;

            case GDK_d: selected->setCurrentPositionX( selected->getCurrentPosition().getX() + stepsize ); break;

            case GDK_q: selected->setCurrentAngle( selected->getCurrentAngle() + 10 ); 
						if (selected->getCurrentAngle() >= 360)
							selected->setCurrentAngle(0);
						break;

            case GDK_e: selected->setCurrentAngle( selected->getCurrentAngle() - 10 ); 
						if (selected->getCurrentAngle() <= -360)
							selected->setCurrentAngle(0);
						break;

        }
    }
	
	//gtk_widget_draw(widget, NULL);
}

int MainWindow::getStepsize()
{
	return gtk_spin_button_get_value_as_int((GtkSpinButton*)stepsize);
}

int MainWindow::getPrintFullPathplan()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)printFullPathplan);
}

int	MainWindow::getPrintObstacles()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)printObstacles);
}

void MainWindow::fillTextOutput(char text[])
{
	GtkTextBuffer *text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(this->TextOutput));
	gtk_text_buffer_set_text (text_buffer, text, -1);
}

void pathplanButton(GtkWidget *widget, gpointer data)

//creates a "hanging click" to wait until the user clicks in the drawing area
{
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;

	if( gtk_toggle_button_get_active((GtkToggleButton*)widget) ){

		// parameters decoding
		guiPlayer *selected = mw->getSelectedPlayer();

		if( selected ) //has a player selected
		{
			// which pathplan instance to create?
			if( gtk_toggle_button_get_active( (GtkToggleButton*)mw->useRrt) )
				mw->pathplan = new Rrt();
			else if( gtk_toggle_button_get_active( (GtkToggleButton*)mw->useAstar) )
					mw->pathplan = new AStar();
			
			// set environment matrix dimensions
			mw->pathplan->setEnvDimensions( gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->pathplanGridX),
											gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->pathplanGridY));
											
			// set the radius of the obstacules
			mw->pathplan->setRadius( gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->obstaculesRadius) );
			
			// set initial position to selected player's position
			mw->pathplan->setInitialPos( Point( selected->getCurrentPosition().getX(),	
												selected->getCurrentPosition().getY() )	);
			
			// sets obstacules in the environment
			vector<RP::Point> positions;
			for(int team=0; team<2; team++)
				for(int i=0; i<mw->game.getNplayers(team); i++)
					if( selected != &(mw->game.players[team][i]) )
							positions.push_back(mw->game.players[team][i].getCurrentPosition());
			positions.push_back(mw->game.ball.getCurrentPosition());
			mw->pathplan->fillEnv(positions);
			
			// GUI settings
			gtk_button_set_label((GtkButton*)widget, "Running...");
			mw->cursorEvent = CURSOR_EVENT_PATHPLAN;
			mw->pushStatusMessage("Pathplanning is running.");
		}
	}
	else{
		gtk_button_set_label((GtkButton*)widget, "Set Destination");
		delete mw->pathplan;
		mw->cursorEvent = CURSOR_EVENT_NOTHING;
		mw->toDrawPathplan = false;
		//mw->pushStatusMessage("Waiting for destination definition.");
	}

}


guiPlayer* MainWindow::getSelectedPlayer()
{
		int index, team;

		int comboBoxIndex = gtk_combo_box_get_active((GtkComboBox*)this->game.playersComboBox);
		int nPlayersTeam1 = this->game.getNplayers(0);

		team = comboBoxIndex < nPlayersTeam1?  0 : 1;
		index = team==0 ? comboBoxIndex : comboBoxIndex - nPlayersTeam1;

		if(index==-1)
			return NULL;
		else
			return &(game.players[team][index]);
}


void isVerboseButton(GtkWidget *widget, gpointer data)
{
    // parameters
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;

	if ( gtk_toggle_button_get_active((GtkToggleButton*)widget) )
		mw->isVerbose = true;
	else
		mw->isVerbose = false;

}


void clientCommunicationButton(GtkWidget *widget, gpointer data)
{
    // parameters
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
	int port = gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->clientPort);
	char *host = (char*) gtk_entry_get_text((GtkEntry*)mw->clientHost);

	if( gtk_toggle_button_get_active((GtkToggleButton*)widget) ) {
		mw->openClient(port, host);
		gtk_button_set_label((GtkButton*)widget, "Disconnect");
	}
	else {
		mw->closeClient();
		gtk_button_set_label((GtkButton*)widget, "Open Connection");
	}
}


void serverCommunicationButton(GtkWidget *widget, gpointer data)
{
    // parameters
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
	int port = gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->serverPort);
	char *host = (char*) gtk_entry_get_text((GtkEntry*)mw->serverHost);

	if( gtk_toggle_button_get_active((GtkToggleButton*)widget)  ) {
		mw->openServer(port, host);
		gtk_button_set_label((GtkButton*)widget, "Disconnect");
	}
	else{
		mw->closeServer();
		gtk_button_set_label((GtkButton*)widget, "Open Connection");
	}
}


void getLocalIPButton(GtkWidget *widget, gpointer data)
{
    // parameters
	parametersType* parametros = (parametersType*) data;
    GtkWidget* whereToPut = (GtkWidget*)parametros->widgets[0];

	//use linux command to get the local IP
	//system( "ifconfig -a | grep 'inet end.:' | cut -f13-13 -d' ' > ip.txt");
	if( !system( "wget -q http://icanhazip.com/ -O ip.txt") ) {
		ifstream file;
		file.open ("ip.txt", fstream::out);
		char ip[20]; file.getline(ip,20);
		file.close();
		
		//system( "rm ip.txt");
		gtk_entry_set_text((GtkEntry*)whereToPut,ip);
	}
}


void addYellowPlayerButton(GtkWidget *widget, gpointer data)
{
	// parameters
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;

	mw->cursorEvent = CURSOR_EVENT_ADD_YELLOW_ROBOT;

	//mw->pushStatusMessage("Added 1 Yellow Player.");
}


void addBluePlayerButton(GtkWidget *widget, gpointer data)
{
	// parameters
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
	
	mw->cursorEvent = CURSOR_EVENT_ADD_BLUE_ROBOT;

	//mw->pushStatusMessage("Added 1 Blue Player.");
}

void setBallButton(GtkWidget *widget, gpointer data)
{
	// parameters
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
	
	mw->cursorEvent = CURSOR_EVENT_SET_BALL;
}

void saveStateButton(GtkWidget *widget, gpointer data)
{	
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
		
	FILE *arq = fopen("status.txt","w");
	if (arq != NULL)
	{
		fprintf(arq,"yellow\n");
		for (int i=0; i<2; i++)
		{
			if (i)
			{
				fprintf(arq,"blue\n");
			}
			for (int j=0; j<mw->game.getNplayers(i); j++)
			{
				fprintf(arq,"%.0lf,%.0lf,%.0lf\n",mw->game.players[i][j].getCurrentPosition().getX(),	
											      mw->game.players[i][j].getCurrentPosition().getY(),
											      mw->game.players[i][j].getCurrentAngle());
			}
		}
		
		if(mw->isVerbose)
			cout << "Saved game state." << endl;
	}
	fclose(arq);
}

void loadStateButton(GtkWidget *widget, gpointer data)
{
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
	
	mw->game.updateNplayers(0,0);
	mw->game.updateNplayers(1,0);
	
	FILE *arq = fopen("status.txt","r");
	
	char buffer[20];
	int x,y,theta;
	int i=0,t=0;
	
	if (arq != NULL){
		while (!feof(arq)){
			
			fscanf(arq,"%s\n",buffer);

			if (strcmp(buffer,"yellow") != 0 && strcmp(buffer,"blue") != 0)
			{
				x = atoi(strtok(buffer,","));
				y = atoi(strtok(NULL,","));
				theta = atoi(strtok(NULL,","));
				mw->game.addPlayer(i,Point(x,y));
				mw->game.players[i][t].setCurrentAngle(theta);
				t++;
			}
			if (strcmp(buffer,"blue") == 0)
			{
				i++;
				t=0;
			}
		}
		if(mw->isVerbose){
			cout << "Loaded game state." << endl;
		}	
		fclose(arq);
	}	
}

string getParam( gpointer data )
{
	parametersType* parametros = (parametersType*) data;
	return " " + string( (char*)gtk_entry_get_text((GtkEntry*)parametros->widgets[0]) );
}

void launch(string command )
{
	command = "gnome-terminal -e \"" + command + "\"&";
    if(!system(command.c_str()));
}

void launchAiButton(GtkWidget *widget, gpointer data)
{
    cout << "Launching AI..." << endl;
	launch("../ai/ai" + getParam(data));
}

void launchRadioButton(GtkWidget *widget, gpointer data)
{
    cout << "Launching Radio..." << endl;
	launch("../radio/radio" + getParam(data));
}

void launchTrackerButton(GtkWidget *widget, gpointer data)
{
    cout << "Launching Tracker..." << endl;
	launch("../tracker/tracker" + getParam(data));
}

void launchSimButton(GtkWidget *widget, gpointer data)
{
    cout << "Launching Simulator..." << endl;
	launch("../simulator/simulator" + getParam(data));
}

void launchComButton(GtkWidget *widget, gpointer data)
{
    cout << "Launching Communication Test..." << endl;
	launch("../communication/communication_test" + getParam(data));
}

void MainWindow::pushStatusMessage(string msg)
{
    if( statusBar )
        gtk_statusbar_push((GtkStatusbar*)statusBar, gtk_statusbar_get_context_id((GtkStatusbar*)statusBar,"general use"), msg.c_str());
}








///////////////////////////////////////////////
///////////////               /////////////////
///////////////   structure   /////////////////
///////////////               /////////////////
///////////////////////////////////////////////


void about()
{
    GtkWidget *dialog = gtk_message_dialog_new
	    (NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
	     ("RoboPET GUI\n\nwww.inf.ufrgs.br/pet\nrobopet-gui.googlecode.com"));
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

void MainWindow::createInterface()
{
	GtkBuilder *builder;
	
	// read Glade XML
	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "gui.glade", NULL);

	// store widget pointers from the XML
	this->window = GTK_WIDGET( gtk_builder_get_object(builder, "guiwindow") );
	this->game.playersComboBox = GTK_WIDGET( gtk_builder_get_object(builder, "playerscombobox") );
		GtkListStore *store = gtk_list_store_new (1, G_TYPE_STRING);
		gtk_combo_box_set_model (GTK_COMBO_BOX(this->game.playersComboBox), (GtkTreeModel *)store);
		GtkCellRenderer * cell = gtk_cell_renderer_text_new();
		gtk_cell_layout_pack_start( GTK_CELL_LAYOUT( this->game.playersComboBox ), cell, TRUE );
		gtk_cell_layout_set_attributes( GTK_CELL_LAYOUT( this->game.playersComboBox ), cell, "text", 0, NULL );
	this->soccer_field = GTK_WIDGET( gtk_builder_get_object(builder,"field") );
	this->TextOutput = GTK_WIDGET( gtk_builder_get_object(builder,"textview") );
	this->displaySettings.checkPlayerBody = GTK_WIDGET( gtk_builder_get_object(builder,"checkplayerbody") );
	this->displaySettings.checkPlayerAngle = GTK_WIDGET( gtk_builder_get_object(builder,"checkplayerangle") );
	this->displaySettings.checkPlayerIndex = GTK_WIDGET( gtk_builder_get_object(builder,"checkplayerindex") );
	this->displaySettings.checkPlayerFuture = GTK_WIDGET( gtk_builder_get_object(builder,"checkfuturepos") );
	this->displaySettings.checkBall = GTK_WIDGET( gtk_builder_get_object(builder,"checkball") );
	this->stepsize = GTK_WIDGET( gtk_builder_get_object(builder,"stepsize") );
		gtk_spin_button_set_value((GtkSpinButton*)stepsize,300);
	this->useRrt = GTK_WIDGET( gtk_builder_get_object(builder,"userrt") );
	this->useAstar = GTK_WIDGET( gtk_builder_get_object(builder,"useastar") );
	this->printFullPathplan = GTK_WIDGET( gtk_builder_get_object(builder,"showfullpath") );
	this->printObstacles = GTK_WIDGET( gtk_builder_get_object(builder,"showobstacles") );
	this->clientHost = GTK_WIDGET( gtk_builder_get_object(builder,"clienthost") );
	this->serverHost = GTK_WIDGET( gtk_builder_get_object(builder,"serverhost") );
	this->clientPort = GTK_WIDGET( gtk_builder_get_object(builder,"clientport") );
		gtk_spin_button_set_value((GtkSpinButton*)clientPort, CLIENT_INITIAL_PORT);
	this->serverPort = GTK_WIDGET( gtk_builder_get_object(builder,"serverport") );
		gtk_spin_button_set_value((GtkSpinButton*)serverPort, SERVER_INITIAL_PORT);
	this->statusBar = GTK_WIDGET( gtk_builder_get_object(builder,"statusBar") );
	this->pathplanGridX = GTK_WIDGET( gtk_builder_get_object(builder,"pathplanGridXSpin") );
		gtk_spin_button_set_value((GtkSpinButton*)pathplanGridX, ENV_MATRIX_SIZE_X);
	this->pathplanGridY = GTK_WIDGET( gtk_builder_get_object(builder,"pathplanGridYSpin") );
		gtk_spin_button_set_value((GtkSpinButton*)pathplanGridY, (int)(ENV_MATRIX_SIZE_X * (FIELD_HEIGHT/(float)FIELD_WIDTH) + 1));
	this->obstaculesRadius = GTK_WIDGET( gtk_builder_get_object(builder,"obstaculesRadiusSpin") );
		gtk_spin_button_set_value((GtkSpinButton*)obstaculesRadius, OBSTACULE_RADIUS);
		
	// SIGNALS (over the air, over the air)	
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"aboutmenu")), "activate", G_CALLBACK(about), NULL);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"exitmenu")), "activate", G_CALLBACK(gtk_main_quit), NULL);
	
	static parametersType args;
	args.mw = this;
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"savemenu")), "activate", G_CALLBACK(saveStateButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"openmenu")), "activate", G_CALLBACK(loadStateButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"checkverbose")), "clicked", G_CALLBACK(isVerboseButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"yellowpp")), "clicked", G_CALLBACK(addYellowPlayerButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"bluepp")), "clicked", G_CALLBACK(addBluePlayerButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"setballpos")), "clicked", G_CALLBACK(setBallButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"setdestination")), "clicked", G_CALLBACK(pathplanButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"openclient")), "clicked", G_CALLBACK(clientCommunicationButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"openserver")), "clicked", G_CALLBACK(serverCommunicationButton), &args);
		
	static parametersType args2;
	args2.mw = this;
	args2.widgets.push_back( GTK_WIDGET(gtk_builder_get_object(builder,"argsentry")) );
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"aiButton")), "clicked", G_CALLBACK(launchAiButton), &args2);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"radioButton")), "clicked", G_CALLBACK(launchRadioButton), &args2);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"trackerButton")), "clicked", G_CALLBACK(launchTrackerButton), &args2);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"simButton")), "clicked", G_CALLBACK(launchSimButton), &args2);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"comButton")), "clicked", G_CALLBACK(launchComButton), &args2);

	static parametersType args3;
	args3.mw = this;
	args3.widgets.push_back(clientHost);
	g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder,"icanhazip1")), "clicked", G_CALLBACK(getLocalIPButton), &args3);
	
	static parametersType args4;
	args4.mw = this;
	args4.widgets.push_back(serverHost);
	g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder,"icanhazip2")), "clicked", G_CALLBACK(getLocalIPButton), &args4);
	
	// main window signals
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_signal_connect(GTK_OBJECT(window), "key_press_event", (GtkSignalFunc) key_press_event, this);
}
