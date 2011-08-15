#include <iostream>
#include <fstream>

#include "gdk/gdkkeysyms.h"

#include <fcntl.h>
#include <linux/joystick.h>

#include "interface.h"
#include "rrt.h"
#include "astar.h"
#include "gstar.h"
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
	
	if (event->button == 1 && mw->cursorEvent != CURSOR_EVENT_NOTHING // click with left button
		&& (event->x < mw->fieldWidth - mw->BORDER_PIX) && (event->x > mw->BORDER_PIX)  // if click is inside arena area
		&& (event->y < mw->fieldHeight - mw->BORDER_PIX) && (event->y > mw->BORDER_PIX)) { 
	  
		switch(mw->cursorEvent) {

			case CURSOR_EVENT_PATHPLAN:	
			// GUI was waiting for click event to set final pathplan position
					mw->pushStatusMessage("Running pathplanning...");
					//while(gtk_events_pending()) gtk_main_iteration(); //force gtk to update the StatusBar widget before running the algorithm
					
					mw->pathplan->setFinalPos( Point(mw->PIX_TO_MM(event->x)-BORDER_MM, mw->PIX_TO_MM(event->y)-BORDER_MM) ); //convert screen coordinates into mm, which is the what setFinalPos receives
					mw->pathplan->run();
					
					switch(mw->pathplan->status) {
						case SUCCESS:
							{
								char msg[128];
								sprintf(msg,"Pathplan DONE (%f sec)", mw->pathplan->elapsedTime);
								mw->pushStatusMessage(msg);
							}
							break;
							
						case ERROR_TIMELIMIT:
							mw->pushStatusMessage("Pathplan ERROR: time limit exceeded."); break;
							
						case ERROR_UNKNOWN:
							mw->pushStatusMessage("Pathplan ERROR: unkown cause."); break;
							
						case ERROR_UNREACHABLE:
							mw->pushStatusMessage("Pathplan ERROR: objective is unreachable."); break;
							
						case NOTHING:
							mw->pushStatusMessage("Pathplan completed with no status flag."); break;
					}

					if(mw->pathplan->status == SUCCESS)
						gtk_widget_show( mw->validatePathplanButton );
					else
						gtk_widget_hide( mw->validatePathplanButton );
				
					mw->pathplanSettings.toDraw = true;
					break;
				
			case CURSOR_EVENT_ADD_YELLOW_ROBOT: 
			// GUI was waiting for click event to put a yellow robot
					mw->game.addPlayer( TEAM_YELLOW, Point(mw->PIX_TO_MM(event->x)-BORDER_MM,mw->PIX_TO_MM(event->y)-BORDER_MM) );
					mw->cursorEvent = CURSOR_EVENT_NOTHING;
					break;
				
			case CURSOR_EVENT_ADD_BLUE_ROBOT:
			// GUI was waiting for click event to put a blue robot
					mw->game.addPlayer( TEAM_BLUE, Point(mw->PIX_TO_MM(event->x)-BORDER_MM,mw->PIX_TO_MM(event->y)-BORDER_MM) );
					mw->cursorEvent = CURSOR_EVENT_NOTHING;
					break;
					
			case CURSOR_EVENT_SET_BALL:
			// GUI was waiting for click event to set the ball position
					mw->game.ball.setCurrentPosition(Point(mw->PIX_TO_MM(event->x)-BORDER_MM, mw->PIX_TO_MM(event->y)-BORDER_MM));
					mw->cursorEvent = CURSOR_EVENT_NOTHING;
					break;
		}
		
		mw->updateScene();
	} 	
}

void key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data)
//CALLBACK which captures keyboard keys
{
    MainWindow* mw = (MainWindow*) data;

    //add players
	switch(event->keyval) {
		case GDK_z:
			mw->cursorEvent = CURSOR_EVENT_ADD_YELLOW_ROBOT;
			break;
		case GDK_x:
			mw->cursorEvent = CURSOR_EVENT_ADD_BLUE_ROBOT;
			break;
	}
        
    // players movement
    int stepsize = mw->getStepsize();

    GuiPlayer *selected = mw->getSelectedPlayer();

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

	mw->updateScene();
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
// Handles the two-states Pathplan button.
{
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;

	if( gtk_toggle_button_get_active((GtkToggleButton*)widget) ){
		// pathplan button is deactivated

		GuiPlayer *selected = mw->getSelectedPlayer();

		if( selected ) //has a player selected
		{
			// which pathplan instance to create?
			if( gtk_toggle_button_get_active( (GtkToggleButton*)mw->useRrt) ) {
				mw->pathplan = new Rrt();
				mw->pathplanSettings.isGridBased = true;
				((Rrt*)mw->pathplan)->stepsize = gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->rrtStepsize);
				((Rrt*)mw->pathplan)->timeLimit = gtk_spin_button_get_value_as_float((GtkSpinButton*)mw->rrtTimeLimit);
				((Rrt*)mw->pathplan)->goalProb = gtk_spin_button_get_value_as_float((GtkSpinButton*)mw->rrtGoalProb);
			}
			else if( gtk_toggle_button_get_active( (GtkToggleButton*)mw->useAstar) ) {
					mw->pathplan = new AStar();
					mw->pathplanSettings.isGridBased = true;
			}
			else if( gtk_toggle_button_get_active( (GtkToggleButton*)mw->useGstar) ) {
					mw->pathplan = new GStar();
					mw->pathplanSettings.isGridBased = false;
			}
			
			if(mw->pathplanSettings.isGridBased) {
				// set environment matrix dimensions
				((DiscretePathplan*)mw->pathplan)->setEnvXY( gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->pathplanGridX),
										gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->pathplanGridY));
												
				// set environment matrix dimensions
				((DiscretePathplan*)mw->pathplan)->setEnvXY( gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->pathplanGridX),
										gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->pathplanGridY));
												
				// set the radius of the obstacules
				((DiscretePathplan*)mw->pathplan)->setRadius( gtk_spin_button_get_value_as_int((GtkSpinButton*)mw->obstaculesRadius) );
			}
			
			// set initial position to selected player's position
			mw->pathplan->setInitialPos( Point( selected->getCurrentPosition().getX(),	
												selected->getCurrentPosition().getY() )	);
			
			// obstacules
			for(int team=0; team<2; team++)
				for(int i=0; i<mw->game.getNplayers(team); i++)
					if( selected != &(mw->game.players[team][i]) )
							mw->pathplan->obstacles.push_back( ppObstacle(mw->game.players[team][i].getCurrentPosition(),ROBOT) );
			//mw->pathplan->obstacles.push_back( ppObstacle(mw->game.ball.getCurrentPosition(),BALL) );
			
			if(mw->pathplanSettings.isGridBased)
				((DiscretePathplan*)mw->pathplan)->fillEnv(); //fill obstacules in the grid
			
			
			// GUI settings
			gtk_button_set_label((GtkButton*)widget, "Pathplanning Mode ON");
			mw->cursorEvent = CURSOR_EVENT_PATHPLAN;
			mw->pushStatusMessage("Waiting for destination definition.");
		}
	}
	else{
		gtk_button_set_label((GtkButton*)widget, "Enter Pathplanning Mode");
		delete mw->pathplan;
		mw->cursorEvent = CURSOR_EVENT_NOTHING;
		mw->pathplanSettings.toDraw = false;
		mw->pushStatusMessage("");
		gtk_widget_hide( mw->validatePathplanButton );
	}
	
	mw->updateScene();
} 

void validatePathplanButtonCB(GtkWidget *widget, gpointer data)
{
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
	
	mw->pathplan->obstacles.clear();
	GuiPlayer *selected = mw->getSelectedPlayer();
	for(int team=0; team<2; team++)
		for(int i=0; i<mw->game.getNplayers(team); i++)
			if( selected != &(mw->game.players[team][i]) )
					mw->pathplan->obstacles.push_back( ppObstacle(mw->game.players[team][i].getCurrentPosition(),ROBOT) );
	if(mw->pathplanSettings.isGridBased)
				((DiscretePathplan*)mw->pathplan)->fillEnv(); //fill obstacules in the grid
	
	if(mw->pathplan->validatePath(mw->pathplan->getFinalPos(),0))
		mw->pushStatusMessage("Path is VALID!");
	else
		mw->pushStatusMessage("Path is NOT valid!");
}


GuiPlayer* MainWindow::getSelectedPlayer()
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

	if( gtk_toggle_button_get_active((GtkToggleButton*)widget) ) {
		mw->openClient(port);
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

void openJoystick(GtkWidget *widget, gpointer data)
{
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
	
	mw->joystickFd = open("/dev/input/js0", 0 );
	fcntl(mw->joystickFd, F_SETFL, O_RDONLY | O_NONBLOCK );	
	
	if(mw->isVerbose)
		cout << "Opened /dev/input/js0." << endl;
		
	mw->pushStatusMessage("Opened /dev/input/js0.");
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

void deletePlayerButton(GtkWidget *widget, gpointer data)
{
	// parameters
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
	
	int index, team;

	int comboBoxIndex = gtk_combo_box_get_active((GtkComboBox*)mw->game.playersComboBox);
	int nPlayersTeam1 = mw->game.getNplayers(0);

	team = comboBoxIndex < nPlayersTeam1?  0 : 1;
	index = team==0 ? comboBoxIndex : comboBoxIndex - nPlayersTeam1;
	
	if ( index>=0 )
		mw->game.deletePlayer( team, index );

	mw->updateScene();

	//mw->pushStatusMessage("Player Deleted");
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
			
		mw->pushStatusMessage("Saved game state.");
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
		if(mw->isVerbose)
			cout << "Loaded game state." << endl;
		
		mw->pushStatusMessage("Loaded game state.");
		
		fclose(arq);
	}
	
	mw->updateScene();
}

string getParam( gpointer data )
{
	parametersType* parametros = (parametersType*) data;
	return " " + string( (char*)gtk_entry_get_text((GtkEntry*)parametros->widgets[0]) );
}

void launch(string dir, string bin, string param )
{
	string command;
	command = "gnome-terminal --working-directory=\"$(pwd)/" + dir + "\" -e \"" + dir + bin + " " + param + "\"";
	cout << command << endl;
    if(!system(command.c_str()));
}

void launchAi(string param)
{
	launch("../ai/", "ai", param);
}

void launchRadio(string param)
{
	launch("../radio/", "radio", param);
}

void launchTracker(string param)
{
	launch("../tracker/", "tracker", param);
}

void launchSimulator(string param)
{
	launch("../simulation/", "simulator", param);
}

void launchCom(string param)
{
	launch("../communication/", "communication_test", param);
}

void launchRefbox()
{
	system("gnome-terminal -e sslrefbox");
}

void simLoopButton(GtkWidget *widget, gpointer data)
{
	//if(mw->isVerbose) cout << "Launching Simulation Loop..." << endl;
   // mw->pushStatusMessage("Launching Simulation Loop...");
    
	launchSimulator("");
	launchTracker("-sim");
	launchAi("");
	launchRadio("-d");
}

void launchAiButton(GtkWidget *widget, gpointer data)
{
    //if(mw->isVerbose) cout << "Launching AI..." << endl;
    //mw->pushStatusMessage("Launching AI...");
     
	launchAi(getParam(data));
}

void launchRadioButton(GtkWidget *widget, gpointer data)
{
    //if(mw->isVerbose) cout << "Launching Radio..." << endl;
   // mw->pushStatusMessage("Launching Radio...");
    
	launchRadio(getParam(data));
}

void launchTrackerButton(GtkWidget *widget, gpointer data)
{
    //if(mw->isVerbose) cout << "Launching Tracker..." << endl;
    //mw->pushStatusMessage("Launching Tracker...");
    
	launchTracker(getParam(data));
}

void launchSimButton(GtkWidget *widget, gpointer data)
{
    //if(mw->isVerbose) cout << "Launching Simulator..." << endl;
	//mw->pushStatusMessage("Launching Simulator...");
	
	launchSimulator(getParam(data));
}

void launchComButton(GtkWidget *widget, gpointer data)
{
    //cout << "Launching Communication Test..." << endl;
    //mw->pushStatusMessage("Launching Communication Test...");
    
   	launchCom(getParam(data));
}

void MainWindow::pushStatusMessage(string msg)
{
	gtk_statusbar_push((GtkStatusbar*)statusBar, gtk_statusbar_get_context_id((GtkStatusbar*)statusBar,""), msg.c_str());
}

void sizeRequestCB(GtkWidget *widget, GdkEventExpose *event, MainWindow* mw)
{
	int w = mw->soccer_field->allocation.width, 
		h = mw->soccer_field->allocation.height;
	
	if( h > w*FIELD_RATIOXY )
		h = w*FIELD_RATIOXY;
	else if ( w > h/FIELD_RATIOXY )
			w = h/FIELD_RATIOXY;
			
	mw->fieldWidth = w;
	mw->fieldHeight = h;
	
	mw->BORDER_PIX = mw->MM_TO_PIX(BORDER_MM);
	mw->GOAL_LINE = mw->MM_TO_PIX(GOAL_LINE_MM); 
	mw->GOAL_CIRC_RADIUS = mw->MM_TO_PIX(GOAL_CIRC_RADIUS_MM);
	mw->HALF_FIELD_RADIUS = mw->MM_TO_PIX(HALF_FIELD_RADIUS_MM);
	mw->ROBOT_RADIUS =  mw->MM_TO_PIX(ROBOT_RADIUS_MM);
	mw->BALL_RADIUS =  mw->MM_TO_PIX(BALL_RADIUS_MM);

	mw->updateScene();
}

void expanderCB(GtkWidget *widget, MainWindow* mw)
{
	//none of these worked :(
	
	//sizeRequestCB(0,0,mw);
	mw->updateScene();
}


///////////////////////////////////////////////
///////////////               /////////////////
///////////////   structure   /////////////////
///////////////               /////////////////
///////////////////////////////////////////////


void helpAbout()
{
    GtkWidget *dialog = gtk_message_dialog_new
	    (NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
	     ("RoboPET GUI\n\nwww.inf.ufrgs.br/pet\nrobopet-gui.googlecode.com"));
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

void aboutKeyboard()
{
    GtkWidget *dialog = gtk_message_dialog_new
	    (NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
	     ("These are the available keyboard controls:\n\nWASD to move\nQE to rotate\nZ/X do add yellow/blue\n"));
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
	//this->TextOutput = GTK_WIDGET( gtk_builder_get_object(builder,"textview") );
	this->displaySettings.checkPlayerBody = GTK_WIDGET( gtk_builder_get_object(builder,"checkplayerbody") );
	this->displaySettings.checkPlayerData = GTK_WIDGET( gtk_builder_get_object(builder,"checkplayerdata") );
		//gtk_signal_connect_after(GTK_OBJECT(displaySettings.checkPlayerData), "toggled", (GtkSignalFunc) updateSceneCB, this); //not working :(
	this->displaySettings.checkPlayerFuture = GTK_WIDGET( gtk_builder_get_object(builder,"checkfuturepos") );
	this->displaySettings.checkPlayerPath = GTK_WIDGET( gtk_builder_get_object(builder,"checkpath") );
	this->displaySettings.checkBall = GTK_WIDGET( gtk_builder_get_object(builder,"checkball") );
	this->stepsize = GTK_WIDGET( gtk_builder_get_object(builder,"stepsize") );
		gtk_spin_button_set_value((GtkSpinButton*)stepsize,300);
	this->useRrt = GTK_WIDGET( gtk_builder_get_object(builder,"userrt") );
	this->useAstar = GTK_WIDGET( gtk_builder_get_object(builder,"useastar") );
	this->useGstar = GTK_WIDGET( gtk_builder_get_object(builder,"usegstar") );
	this->printFullPathplan = GTK_WIDGET( gtk_builder_get_object(builder,"showfullpath") );
	this->printObstacles = GTK_WIDGET( gtk_builder_get_object(builder,"showobstacles") );
	this->clientHost = GTK_WIDGET( gtk_builder_get_object(builder,"clienthost") );
	this->serverHost = GTK_WIDGET( gtk_builder_get_object(builder,"serverhost") );
	this->clientPort = GTK_WIDGET( gtk_builder_get_object(builder,"clientport") );
		gtk_spin_button_set_value((GtkSpinButton*)clientPort, CLIENT_INITIAL_PORT);
	this->serverPort = GTK_WIDGET( gtk_builder_get_object(builder,"serverport") );
		gtk_spin_button_set_value((GtkSpinButton*)serverPort, SERVER_INITIAL_PORT);
	this->statusBar = GTK_WIDGET( gtk_builder_get_object(builder,"statusbar") );
	this->pathplanGridX = GTK_WIDGET( gtk_builder_get_object(builder,"pathplanGridXSpin") );
		gtk_spin_button_set_value((GtkSpinButton*)pathplanGridX, ENV_MATRIX_SIZE_X);
	this->pathplanGridY = GTK_WIDGET( gtk_builder_get_object(builder,"pathplanGridYSpin") );
		gtk_spin_button_set_value((GtkSpinButton*)pathplanGridY, (int)(ENV_MATRIX_SIZE_X * (FIELD_HEIGHT_MM/(float)FIELD_WIDTH_MM) + 1));
	this->obstaculesRadius = GTK_WIDGET( gtk_builder_get_object(builder,"obstaculesRadiusSpin") );
		gtk_spin_button_set_value((GtkSpinButton*)obstaculesRadius, OBSTACULE_RADIUS);
	this->rrtTimeLimit = GTK_WIDGET( gtk_builder_get_object(builder,"rrtTimeLimitSpin") );
		gtk_spin_button_set_value((GtkSpinButton*)rrtTimeLimit, RRT_DEFAULT_TIMELIMIT);
	this->rrtGoalProb = GTK_WIDGET( gtk_builder_get_object(builder,"rrtGoalProbSpin") );
		gtk_spin_button_set_value((GtkSpinButton*)rrtGoalProb, RRT_DEFAULT_GOALPROB);
	this->rrtStepsize = GTK_WIDGET( gtk_builder_get_object(builder,"rrtStepsizeSpin") );
		gtk_spin_button_set_value((GtkSpinButton*)rrtStepsize, RRT_DEFAULT_STEPSIZE);
	this->gstarPath = GTK_WIDGET( gtk_builder_get_object(builder,"gstarPathSpin") );
		gtk_spin_button_set_value((GtkSpinButton*)gstarPath, -1);
	this->validatePathplanButton = GTK_WIDGET(gtk_builder_get_object(builder,"validatePath"));
		
	g_signal_connect_after( GTK_WIDGET(GTK_WIDGET(gtk_builder_get_object(builder,"expander"))), "activate", G_CALLBACK(expanderCB), this);
		
	// SIGNALS (over the air, over the air)	
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"aboutkeyboard")), "activate", G_CALLBACK(aboutKeyboard), NULL);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"aboutmenu")), "activate", G_CALLBACK(helpAbout), NULL);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"exitmenu")), "activate", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"simLoopButton")), "clicked", G_CALLBACK(simLoopButton), NULL);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"refboxButton")), "clicked", G_CALLBACK(launchRefbox), NULL);

	static parametersType args;
	args.mw = this;
	
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"savemenu")), "activate", G_CALLBACK(saveStateButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"openmenu")), "activate", G_CALLBACK(loadStateButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"checkverbose")), "clicked", G_CALLBACK(isVerboseButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"yellowpp")), "clicked", G_CALLBACK(addYellowPlayerButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"bluepp")), "clicked", G_CALLBACK(addBluePlayerButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"deletepp")), "clicked", G_CALLBACK(deletePlayerButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"setballpos")), "clicked", G_CALLBACK(setBallButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"setdestination")), "clicked", G_CALLBACK(pathplanButton), &args);
	g_signal_connect( validatePathplanButton, "clicked", G_CALLBACK(validatePathplanButtonCB), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"openclient")), "clicked", G_CALLBACK(clientCommunicationButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"openserver")), "clicked", G_CALLBACK(serverCommunicationButton), &args);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"joyButton")), "clicked", G_CALLBACK(openJoystick), &args);
		
	static parametersType args2;
	args2.mw = this;
	args2.widgets.push_back( GTK_WIDGET(gtk_builder_get_object(builder,"argsentry")) );
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"aiButton")), "clicked", G_CALLBACK(launchAiButton), &args2);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"radioButton")), "clicked", G_CALLBACK(launchRadioButton), &args2);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"trackerButton")), "clicked", G_CALLBACK(launchTrackerButton), &args2);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"simButton")), "clicked", G_CALLBACK(launchSimButton), &args2);
	g_signal_connect( GTK_WIDGET(gtk_builder_get_object(builder,"comButton")), "clicked", G_CALLBACK(launchComButton), &args2);
	
	static parametersType args4;
	args4.mw = this;
	args4.widgets.push_back(serverHost);
	g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder,"icanhazip2")), "clicked", G_CALLBACK(getLocalIPButton), &args4);
	
	
	// Main Window Signals
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	//Connect the event of a key pressing with our own signal handling funciont
	//  Must use the "after" signal connection to don't overhide GTK's signal handling (i.e. accelerators handling)
	gtk_signal_connect_after(GTK_OBJECT(window), "key_press_event", (GtkSignalFunc) key_press_event, this);
			
	gtk_signal_connect_after(GTK_OBJECT(window), "size_request", (GtkSignalFunc) sizeRequestCB, this);
}
