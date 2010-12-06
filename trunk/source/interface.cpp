#include<iostream>
#include <fstream>

#include "gdk/gdkkeysyms.h"

#include "interface.h"
#include "rrt.h"
#include "astar.h"

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
  
  if (event->button == 1 ){

	  switch(mw->cursorEvent) {

		case CURSOR_EVENT_NOTHING:
			break;

		case CURSOR_EVENT_PATHPLAN:
			if( (event->x < ARENA_WIDTH) && (event->y < ARENA_HEIGHT) )
			{					
				mw->pathplan->setFinalPos( Point(PIX_TO_MM(event->x)-BORDER_MM, PIX_TO_MM(event->y)-BORDER_MM) ); //convert screen coordinates into mm, which is the what setFinalPos receives
				mw->pathplan->run();
				mw->cursorEvent = CURSOR_EVENT_NOTHING;
				mw->toDrawPathplan = true;
			}
			break;
	  }
  }

}

void key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer data)
//CALLBACK which captures keyboard keys
{
    MainWindow* mw = (MainWindow*) data;

    // parameters
    int stepsize = mw->getStepsize();  

    pair<int,int> ret = mw->getSelectedPlayer();
    int playerIndex, playerTeam;
    playerIndex = ret.first;
    playerTeam = ret.second;

    if( playerIndex!=-1 ) {
        switch(  event->keyval  ) {
            case GDK_s: mw->game.players[playerTeam][playerIndex].setCurrentPositionY( mw->game.players[playerTeam][playerIndex].getCurrentPosition().getY() + stepsize ); break;

            case GDK_w: mw->game.players[playerTeam][playerIndex].setCurrentPositionY( mw->game.players[playerTeam][playerIndex].getCurrentPosition().getY() - stepsize ); break;

            case GDK_a: mw->game.players[playerTeam][playerIndex].setCurrentPositionX( mw->game.players[playerTeam][playerIndex].getCurrentPosition().getX() - stepsize ); break;

            case GDK_d: mw->game.players[playerTeam][playerIndex].setCurrentPositionX( mw->game.players[playerTeam][playerIndex].getCurrentPosition().getX() + stepsize ); break;

            case GDK_q: mw->game.players[playerTeam][playerIndex].setCurrentAngle( mw->game.players[playerTeam][playerIndex].getCurrentAngle() - 10 ); break;

            case GDK_e: mw->game.players[playerTeam][playerIndex].setCurrentAngle( mw->game.players[playerTeam][playerIndex].getCurrentAngle() + 10 ); break;

        }
    }

    
}

int MainWindow::getStepsize()
{
	return gtk_spin_button_get_value_as_int((GtkSpinButton*)stepsize);
}

pathplanType MainWindow::getPathplanIndex()
{
	return (pathplanType) gtk_combo_box_get_active((GtkComboBox*)pathplanBox);
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
    if( this->TextOutput ) {

        GtkTextBuffer *text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(this->TextOutput));
        gtk_text_buffer_set_text (text_buffer, text, -1);
    }
}


void setBallPos(GtkWidget *widget, gpointer data)
{
        // parameters
	parametersType* parametros = (parametersType*) data;

	MainWindow* mw = parametros->mw;
	int ballx = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[0]);
	int bally = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[1]);


	mw->game.ball.setCurrentPosition(Point(ballx, bally));
}


void pathplanButton(GtkWidget *widget, gpointer data)
//creates a "hanging click" to wait until the user clicks in the drawing area
{
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;

	if( gtk_toggle_button_get_active((GtkToggleButton*)widget)  ){

		// parameters decoding
		pair<int,int> ret = mw->getSelectedPlayer();
		int playerIndex, playerTeam;
		playerIndex = ret.first;
		playerTeam = ret.second;

		switch(mw->getPathplanIndex()) {
			case RRT: 
				mw->pathplan = new Rrt();
				break;
			case ASTAR:
				mw->pathplan = new AStar();
				break;
		}
		
		mw->pathplan->setInitialPos( Point( mw->game.players[playerTeam][playerIndex].getCurrentPosition().getX(),	
											mw->game.players[playerTeam][playerIndex].getCurrentPosition().getY() )	);
		
		// environment setting
		vector<RP::Point> positions;
		for(int team=0; team<2; team++)
			for(int i=0; i<mw->game.getNplayers(team); i++)
				if( !(playerTeam==team && i==playerIndex) )
						positions.push_back(mw->game.players[team][i].getCurrentPosition());
		mw->pathplan->fillEnv(positions);
		
		// GUI settings
		gtk_button_set_label((GtkButton*)widget, "Running...");
		//mw->pushStatusMessage("Pathplanning is running.");
		mw->cursorEvent = CURSOR_EVENT_PATHPLAN;
	}
	else{
		gtk_button_set_label((GtkButton*)widget, "Set Destination");
		//mw->pushStatusMessage("Waiting for destination definition.");
		delete mw->pathplan;
		mw->cursorEvent = CURSOR_EVENT_NOTHING;
		mw->toDrawPathplan = false;
	}

}


pair<int,int> MainWindow::getSelectedPlayer()
{
		int index, team;

		int comboBoxIndex = gtk_combo_box_get_active((GtkComboBox*)this->game.playersComboBox);
		int nPlayersTeam1 = this->game.getNplayers(0);
		int nPlayersTeam2 = this->game.getNplayers(1);

		team = comboBoxIndex < nPlayersTeam1?  0 : 1;
		index = team==0 ? comboBoxIndex : comboBoxIndex - nPlayersTeam1;

		return pair<int,int>(index,team);
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
	int port = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[0]);
	char *host = (char*) gtk_entry_get_text((GtkEntry*)parametros->widgets[1]);


	if( gtk_toggle_button_get_active((GtkToggleButton*)widget)  ) {

		mw->openClient(port, host);

		gtk_button_set_label((GtkButton*)widget, "Disconnect");
	}
	else{

                mw->closeClient();

		gtk_button_set_label((GtkButton*)widget, "Open Connection");
	}
}


void serverCommunicationButton(GtkWidget *widget, gpointer data)
{
    // parameters
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
	int port = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[0]);
	char *host = (char*) gtk_entry_get_text((GtkEntry*)parametros->widgets[1]);


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
	MainWindow* mw = parametros->mw;
    GtkWidget* clientHostEntry = (GtkWidget*)parametros->widgets[0];
    GtkWidget* serverHostEntry = (GtkWidget*)parametros->widgets[1];
    GtkWidget* client_getIpButton = (GtkWidget*)parametros->widgets[2];
    GtkWidget* server_getIpButton = (GtkWidget*)parametros->widgets[3];

	//use linux command to get the local IP
	//system( "ifconfig -a | grep 'inet end.:' | cut -f13-13 -d' ' > ip.txt");
	system( "wget -q http://icanhazip.com/ -O ip.txt");
		ifstream file;
		file.open ("ip.txt", fstream::out);
		char ip[20]; file.getline(ip,20);
		file.close();
	//system( "rm ip.txt");

	if( widget == client_getIpButton )
		gtk_entry_set_text((GtkEntry*)clientHostEntry,ip);
	else
		gtk_entry_set_text((GtkEntry*)serverHostEntry,ip);
}


void addYellowPlayerButton(GtkWidget *widget, gpointer data)
{
	// parameters
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;

	mw->game.addPlayer(0);

	//mw->pushStatusMessage("Added 1 Yellow Player.");
}


void addBluePlayerButton(GtkWidget *widget, gpointer data)
{
	// parameters
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;

	mw->game.addPlayer(1);

	//mw->pushStatusMessage("Added 1 Blue Player.");
}

void saveStateButton(void)
{
	printf("cliclou em save\n");
	
	/*FILE *arq = fopen("status","w");
	if (arq != NULL)
	{
		fprintf(arq,"blue\n")
		for (int i=0; i<2; i++)
		{
			if (i)
			{
				fprintf(arq,"yellow\n");
			}
			for (int j=0; j<getNplayers(i); j++)
			{
				fprintf(arq,"%d,%d\n",get*/
}

void loadStateButton(void)
{
	printf("cliclou em load\n");
	/*FILE *arq = fopen("status","r");
	if (arq != NULL){
		while (!foef(arq)){

		//adicionar os players aqui:
		// game.addPlayer(1,Point(1500,1500));
		}
	}*/
}

string getParam( gpointer data )
{
	parametersType* parametros = (parametersType*) data;
	MainWindow* mw = parametros->mw;
	return " " + string( (char*)gtk_entry_get_text((GtkEntry*)parametros->widgets[0]) );
}

void launch(string command )
{
	command = "gnome-terminal -e \"" + command + "\"&";
    system(command.c_str());
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








/////////////////////////////////////////////////////
/////////////////                   /////////////////
/////////////////   structuration   /////////////////
/////////////////                   /////////////////
/////////////////////////////////////////////////////


void createControlTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 3///
	////////////
	GtkWidget* tab = gtk_label_new_with_mnemonic("Manual control");

	//widgets
	mw->stepsize = gtk_spin_button_new_with_range(0, 500, 1);
    	gtk_spin_button_set_value((GtkSpinButton*)mw->stepsize,300);

	GtkWidget* label_bolax = gtk_label_new("x: ");
	GtkWidget* bolax = gtk_spin_button_new_with_range(0, ARENA_WIDTH_MM, 10);
	GtkWidget* label_bolay = gtk_label_new("y: ");
	GtkWidget* bolay = gtk_spin_button_new_with_range(0, ARENA_HEIGHT_MM, 10);
	GtkWidget* button_ballpos = gtk_button_new_with_mnemonic("set ball pos");


	GtkWidget* movementControlBox1 = gtk_hbox_new (FALSE, 0);
			gtk_box_pack_start(GTK_BOX(movementControlBox1), gtk_label_new("stepsize: "), false, false, 0);
	gtk_box_pack_start(GTK_BOX(movementControlBox1), mw->stepsize, false, false, 0);

	GtkWidget* playersControl = gtk_vbox_new (FALSE, 0);
			gtk_box_pack_start(GTK_BOX(playersControl), movementControlBox1, false, false, 0);
			gtk_box_pack_start(GTK_BOX(playersControl), gtk_label_new("use WASD to move"), false, false, 0);
	gtk_box_pack_start(GTK_BOX(playersControl), gtk_label_new("and QE to rotate"), false, false, 0);

	GtkWidget* ballControlBox2 = gtk_hbox_new (FALSE, 0);
			gtk_box_pack_start(GTK_BOX(ballControlBox2), label_bolax, false, false, 0);
	gtk_box_pack_start(GTK_BOX(ballControlBox2), bolax, false, false, 0);
	gtk_box_pack_start(GTK_BOX(ballControlBox2), label_bolay, false, false, 0);
	gtk_box_pack_start(GTK_BOX(ballControlBox2), bolay, false, false, 0);

	GtkWidget* ballControlBox = gtk_vbox_new (FALSE, 0);
			gtk_box_pack_start(GTK_BOX(ballControlBox), ballControlBox2, false, false, 0);
			gtk_box_pack_start(GTK_BOX(ballControlBox), button_ballpos, false, false, 0);

	GtkWidget* playersFrame = gtk_frame_new("Players");
			gtk_container_add(GTK_CONTAINER(playersFrame),playersControl);

	GtkWidget* ballFrame = gtk_frame_new("Ball");
			gtk_container_add(GTK_CONTAINER(ballFrame),ballControlBox);


	//vboxes
	GtkWidget* menu3Box = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(menu3Box), playersFrame, false, false, 0);
                gtk_box_pack_start(GTK_BOX(menu3Box), ballFrame, false, false, 0);



    /////////////////
	//// SIGNALS ////

	//  set Bola pos
	static parametersType parametros5;
	parametros5.mw = mw;
	parametros5.widgets.push_back(bolax);
	parametros5.widgets.push_back(bolay);

	g_signal_connect(G_OBJECT(button_ballpos), "clicked", G_CALLBACK(setBallPos), &parametros5);


	//insert this tab in the notebook
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menu3Box, tab);
}

void createSettingsTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 4///
	////////////
	GtkWidget* tab = gtk_label_new_with_mnemonic("Settings");

	//widgets
	//Players
	GtkWidget* playersFrame = gtk_frame_new("Players");
	GtkWidget* playerBody = gtk_check_button_new_with_label("hide Body");
		mw->displaySettings.checkPlayerBody = playerBody;
	GtkWidget* playerAngle = gtk_check_button_new_with_label("hide Angle");
		mw->displaySettings.checkPlayerAngle = playerAngle;
	GtkWidget* playerIndex = gtk_check_button_new_with_label("hide Index");
		mw->displaySettings.checkPlayerIndex = playerIndex;
	GtkWidget* playerFuture = gtk_check_button_new_with_label("hide Future");
	mw->displaySettings.checkPlayerFuture = playerFuture;
	//Ball
	GtkWidget* ballFrame = gtk_frame_new("Ball");
	GtkWidget* ballHide = gtk_check_button_new_with_label("hide Ball");
	mw->displaySettings.checkBall = ballHide;
	//Program
	GtkWidget* programFrame = gtk_frame_new("Program");
	GtkWidget* isVerbose = gtk_check_button_new_with_label("be Verbose");


	//vboxes
	GtkWidget* playersBox = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(playersBox), playerBody, false, false, 0);
		gtk_box_pack_start(GTK_BOX(playersBox), playerAngle, false, false, 0);
		gtk_box_pack_start(GTK_BOX(playersBox), playerIndex, false, false, 0);
		gtk_box_pack_start(GTK_BOX(playersBox), playerFuture, false, false, 0);
		gtk_container_add(GTK_CONTAINER(playersFrame),playersBox);

	GtkWidget* ballBox = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(ballBox), ballHide, false, false, 0);
		gtk_container_add(GTK_CONTAINER(ballFrame),ballBox);

        GtkWidget* programBox = gtk_vbox_new(FALSE, 0);
                gtk_box_pack_start(GTK_BOX(programBox), isVerbose, false, false, 0);
                gtk_container_add(GTK_CONTAINER(programFrame),programBox);

	//hboxes
	GtkWidget* menuBox = gtk_hbox_new (FALSE, 0);
                gtk_box_pack_start(GTK_BOX(menuBox), programFrame, false, false, 0);
                gtk_box_pack_start(GTK_BOX(menuBox), playersFrame, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menuBox), ballFrame, false, false, 0);


    /////////////////
	//// SIGNALS ////
	//  CHECK BOX: "be Verbose"
	static parametersType parametros;
	parametros.mw = mw;
	g_signal_connect(G_OBJECT(isVerbose), "clicked", G_CALLBACK(isVerboseButton), &parametros);


	//insert this tab in the notebook
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menuBox, tab);
}

void createCommunicationTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 5///
	////////////
	GtkWidget* tab = gtk_label_new_with_mnemonic("Communication");

	//widgets
	GtkWidget* client_button = gtk_toggle_button_new_with_label("Open Communication");
	GtkWidget* client_portEntry = gtk_spin_button_new_with_range(0, 65536, 1);
		gtk_spin_button_set_value((GtkSpinButton*)client_portEntry,CLIENT_INITIAL_PORT);
	GtkWidget* client_hostEntry = gtk_entry_new_with_max_length(15);
		gtk_entry_set_text((GtkEntry*)client_hostEntry,CLIENT_INITIAL_HOST);
	GtkWidget* client_button_getlocalip = gtk_button_new_with_mnemonic("ICanHazIP");

	GtkWidget* server_button = gtk_toggle_button_new_with_label("Open Communication");
	GtkWidget* server_portEntry = gtk_spin_button_new_with_range(0, 65536, 1);
		gtk_spin_button_set_value((GtkSpinButton*)server_portEntry,SERVER_INITIAL_PORT);
	GtkWidget* server_hostEntry = gtk_entry_new_with_max_length(15);
		gtk_entry_set_text((GtkEntry*)server_hostEntry,SERVER_INITIAL_HOST);
	GtkWidget* server_button_getlocalip = gtk_button_new_with_mnemonic("ICanHazIP");

	GtkWidget* clientFrame = gtk_frame_new("CLIENT");
	GtkWidget* serverFrame = gtk_frame_new("SERVER");


	//CLIENT BOX
	GtkWidget* portbox1 = gtk_hbox_new (FALSE, 0);
            gtk_box_pack_start(GTK_BOX(portbox1), gtk_label_new("Port: "), false, false, 0);
            gtk_box_pack_start(GTK_BOX(portbox1), client_portEntry, false, false, 0);

	GtkWidget* hostbox1 = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(hostbox1), gtk_label_new("Host: "), false, false, 0);
		gtk_box_pack_start(GTK_BOX(hostbox1), client_hostEntry, false, false, 0);
		gtk_box_pack_start(GTK_BOX(hostbox1), client_button_getlocalip, false, false, 0);

	GtkWidget* clientBox = gtk_vbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(clientBox), portbox1, false, false, 0);
		gtk_box_pack_start(GTK_BOX(clientBox), hostbox1, false, false, 0);
		gtk_box_pack_start(GTK_BOX(clientBox), client_button, false, false, 0);
		gtk_container_add(GTK_CONTAINER(clientFrame),clientBox);//insere no frame
        

	//SERVER BOX
	GtkWidget* portbox2 = gtk_hbox_new (FALSE, 0);
            gtk_box_pack_start(GTK_BOX(portbox2), gtk_label_new("Port: "), false, false, 0);
            gtk_box_pack_start(GTK_BOX(portbox2), server_portEntry, false, false, 0);


	GtkWidget* hostbox2 = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(hostbox2), gtk_label_new("Host: "), false, false, 0);
		gtk_box_pack_start(GTK_BOX(hostbox2), server_hostEntry, false, false, 0);
		gtk_box_pack_start(GTK_BOX(hostbox2), server_button_getlocalip, false, false, 0);
		
	GtkWidget* serverBox = gtk_vbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(serverBox), portbox2, false, false, 0);
		gtk_box_pack_start(GTK_BOX(serverBox), hostbox2, false, false, 0);
		gtk_box_pack_start(GTK_BOX(serverBox), server_button, false, false, 0);
		gtk_container_add(GTK_CONTAINER(serverFrame),serverBox);//insere no frame


	GtkWidget* menuBox = gtk_hbox_new (FALSE, 0);
			gtk_box_pack_start(GTK_BOX(menuBox), clientFrame, false, false, 0);
			gtk_box_pack_start(GTK_BOX(menuBox), serverFrame, false, false, 0);

	/*GtkWidget* panedBox = gtk_hpaned_new();
			gtk_paned_pack1((GtkPaned*)panedBox, clientBox, TRUE, FALSE);
			gtk_paned_pack2((GtkPaned*)panedBox, serverBox, FALSE,FALSE);*/



	////////////////
	//// SINAIS ////
	//  BUTTON: Client Open Connection
	static parametersType parametros;
	parametros.mw = mw;
	parametros.widgets.push_back(client_portEntry);
	parametros.widgets.push_back(client_hostEntry);

	g_signal_connect(G_OBJECT(client_button), "clicked", G_CALLBACK(clientCommunicationButton), &parametros);

	static parametersType parametros2;
	parametros2.mw = mw;
	parametros2.widgets.push_back(server_portEntry);
	parametros2.widgets.push_back(server_hostEntry);

	g_signal_connect(G_OBJECT(server_button), "clicked", G_CALLBACK(serverCommunicationButton), &parametros2);


	static parametersType parametros3;
	parametros3.mw = mw;
	parametros3.widgets.push_back(client_hostEntry);
        parametros3.widgets.push_back(server_hostEntry);
        parametros3.widgets.push_back(client_button_getlocalip);
        parametros3.widgets.push_back(server_button_getlocalip);

	g_signal_connect(G_OBJECT(client_button_getlocalip), "clicked", G_CALLBACK(getLocalIPButton), &parametros3);
	g_signal_connect(G_OBJECT(server_button_getlocalip), "clicked", G_CALLBACK(getLocalIPButton), &parametros3);


	//insert this tab in the notebook
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menuBox, tab);
}


void createPathplanningTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 2///
	////////////
	GtkWidget* tab = gtk_label_new_with_mnemonic("Pathplanning");

	//widgets
	GtkWidget* label_pathplanners = gtk_label_new("Pathplanner: ");
	mw->pathplanBox = gtk_combo_box_new_text();
		gtk_combo_box_insert_text( GTK_COMBO_BOX(mw->pathplanBox), RRT, "RRT");
		gtk_combo_box_insert_text( GTK_COMBO_BOX(mw->pathplanBox), ASTAR, "A*");
		gtk_combo_box_set_active( GTK_COMBO_BOX(mw->pathplanBox), 0);
	
	mw->printFullPathplan = gtk_check_button_new_with_label("Print full solution");
		gtk_toggle_button_set_active((GtkToggleButton*)mw->printFullPathplan,TRUE);
	mw->printObstacles = gtk_check_button_new_with_label("Print obstacules");
		gtk_toggle_button_set_active((GtkToggleButton*)mw->printObstacles,TRUE);
	GtkWidget* ok = gtk_toggle_button_new_with_label("Set Destination");


	//hboxes
	GtkWidget* pathplannersBox = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(pathplannersBox), label_pathplanners, false, false, 0);
		gtk_box_pack_start(GTK_BOX(pathplannersBox), mw->pathplanBox, false, false, 0);
	GtkWidget* jogadorBox = gtk_hbox_new (FALSE, 0);


	//vboxes
	GtkWidget* menu2Box = gtk_vbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), pathplannersBox, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), jogadorBox, false, false, 0);
		//gtk_box_pack_start(GTK_BOX(menu2Box), finalPosBox, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), mw->printFullPathplan, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), mw->printObstacles, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), ok, false, false, 0);


	/////////////////
	//// SIGNALS ////

	//  OK button
	static parametersType args;
	args.mw = mw;
	args.widgets.push_back(mw->pathplanBox);

	g_signal_connect(G_OBJECT(ok), "clicked", G_CALLBACK(pathplanButton), &args);



	//insert this tab in the notebook
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menu2Box, tab);

}


void createLauncherTab(MainWindow* mw, GtkWidget* notebook)
{
	GtkWidget* tab = gtk_label_new_with_mnemonic("Launcher");

	//widgets
	GtkWidget* buttonBox = gtk_hbutton_box_new();
		gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonBox), GTK_BUTTONBOX_CENTER);
	GtkWidget* aiButton = gtk_button_new_with_label("AI");
	GtkWidget* radioButton = gtk_button_new_with_label("Radio");	
	GtkWidget* trackerButton = gtk_button_new_with_label("Tracker");	
	GtkWidget* simButton = gtk_button_new_with_label("Simulation");	
	GtkWidget* comButton = gtk_button_new_with_label("Communication test");
	GtkWidget* param = gtk_entry_new_with_max_length(15);

	gtk_container_add(GTK_CONTAINER(buttonBox),aiButton);
	gtk_container_add(GTK_CONTAINER(buttonBox),radioButton);
	gtk_container_add(GTK_CONTAINER(buttonBox),trackerButton);
	gtk_container_add(GTK_CONTAINER(buttonBox),simButton);
	gtk_container_add(GTK_CONTAINER(buttonBox),comButton);

	//boxes
	GtkWidget* hbox = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new("Additional command line arguments:"), false, false, 0);
		gtk_box_pack_start(GTK_BOX(hbox), param, false, false, 0);
	GtkWidget* vbox = gtk_vbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(vbox), hbox, false, false, 0);
		gtk_box_pack_start(GTK_BOX(vbox), buttonBox, false, false, 0);

    /////////////////
	//// SIGNALS ////

	//  Launchers buttons
	static parametersType args ;
	args.mw = mw;
	args.widgets.push_back(param);

	g_signal_connect(G_OBJECT(aiButton), "clicked", G_CALLBACK(launchAiButton), &args);
	g_signal_connect(G_OBJECT(radioButton), "clicked", G_CALLBACK(launchRadioButton), &args);
	g_signal_connect(G_OBJECT(trackerButton), "clicked", G_CALLBACK(launchTrackerButton), &args);
	g_signal_connect(G_OBJECT(simButton), "clicked", G_CALLBACK(launchSimButton), &args);
	g_signal_connect(G_OBJECT(comButton), "clicked", G_CALLBACK(launchComButton), &args);


	//insert this tab in the notebook
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),vbox, tab);

}

GtkWidget* createNotebook(MainWindow* mw)
{
	GtkWidget* notebook = gtk_notebook_new();

	//createTab1(mw,notebook); //n�o tem nada aqui ainda
	createCommunicationTab(mw,notebook);
        createSettingsTab(mw,notebook);
        createControlTab(mw,notebook);
	createPathplanningTab(mw,notebook);
	createLauncherTab(mw,notebook);

	return notebook;
}


GtkWidget* createLateralMenu(MainWindow* mw)
{
    
    mw->TextOutput = gtk_text_view_new();

    
    GtkWidget* jogadores = gtk_combo_box_new_text();
                mw->game.playersComboBox = jogadores;

    GtkWidget* addBluePlayer = gtk_button_new_with_mnemonic("Blue++");
    GtkWidget* addYellowPlayer = gtk_button_new_with_mnemonic("Yellow++");
    GtkWidget* saveState = gtk_button_new_with_mnemonic("Save");
    GtkWidget* loadState = gtk_button_new_with_mnemonic("Load");

    GtkWidget* addPlayersBox = gtk_hbox_new (FALSE, 0);
            gtk_box_pack_start(GTK_BOX(addPlayersBox), addBluePlayer, false, false, 0);
            gtk_box_pack_start(GTK_BOX(addPlayersBox), addYellowPlayer, false, false, 0);
            
    GtkWidget* stateBox = gtk_hbox_new (FALSE, 0);
            gtk_box_pack_start(GTK_BOX(stateBox), saveState, false, false, 0);
            gtk_box_pack_start(GTK_BOX(stateBox), loadState, false, false, 0);

    
    GtkWidget* playerFrameVBOX = gtk_vbox_new (FALSE, 0);
            gtk_container_add(GTK_CONTAINER(playerFrameVBOX),jogadores);
            gtk_container_add(GTK_CONTAINER(playerFrameVBOX),addPlayersBox);
            gtk_container_add(GTK_CONTAINER(playerFrameVBOX),stateBox);

    GtkWidget* playersFrame = gtk_frame_new("Players");
            gtk_container_add(GTK_CONTAINER(playersFrame),playerFrameVBOX);


    GtkWidget* lateralMenu = gtk_vbox_new (FALSE, 0);
            gtk_box_pack_start(GTK_BOX(lateralMenu), playersFrame, false, false, 0);
            gtk_box_pack_start(GTK_BOX(lateralMenu), mw->TextOutput, false, false, 0);

   

	////////////////
	//// SINAIS ////
	//  BUTTON: addBluePlayer
        //  BUTTON: yellowBluePlayer
	static parametersType parametros;
	parametros.mw = mw;
	g_signal_connect(G_OBJECT(addBluePlayer), "clicked", G_CALLBACK(addBluePlayerButton), &parametros);
	g_signal_connect(G_OBJECT(addYellowPlayer), "clicked", G_CALLBACK(addYellowPlayerButton), &parametros);
	g_signal_connect(G_OBJECT(saveState), "clicked", G_CALLBACK(saveStateButton), &parametros);
	g_signal_connect(G_OBJECT(loadState), "clicked", G_CALLBACK(loadStateButton), &parametros);



    return lateralMenu;
}


GtkWidget* createGameControl(MainWindow* mw)
{
	GtkWidget* box = gtk_hbox_new(FALSE, 0);

	GtkWidget* bar = gtk_hscale_new_with_range(0, 500, 1);
		gtk_widget_set_size_request(box, 1000, 20);

	//gtk_box_pack_start(GTK_BOX(box), bar, false, false, 0);

	return bar;
}

void about()
{
    GtkWidget *dialog = gtk_message_dialog_new
	    (NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
	     ("RoboPET GUI\n\nwww.inf.ufrgs.br/pet"));
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

GtkWidget* createMenuBar(GtkWidget *window)
{

  GtkWidget* menubar;

  static GtkItemFactoryEntry menu_items[] = {
  { (gchar*)"/_File",         NULL,         NULL, 0, (gchar*)"<Branch>" },
  { (gchar*)"/File/sep1",     NULL,         NULL, 0, (gchar*)"<Separator>" },
  { (gchar*)"/File/Quit",     (gchar*)"<alt>Q", gtk_main_quit, 0, NULL },
  { (gchar*)"/_Help",         NULL,         NULL, 0, (gchar*)"<LastBranch>" },
  { (gchar*)"/_Help/About",   NULL,         about, 0, NULL },
};


  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;
  gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

  accel_group = gtk_accel_group_new ();

  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", accel_group);

  gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);

  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);


  return gtk_item_factory_get_widget (item_factory, "<main>");
}


void MainWindow::createInterface()
{
	//widgets gallery: http://library.gnome.org/devel/gtk/unstable/ch02.html

	GtkWidget* menuBar = createMenuBar(this->window);

	GtkWidget* lateralMenu = createLateralMenu(this);

	statusBar = gtk_statusbar_new();

	GtkWidget* notebook = createNotebook(this);
	//GtkWidget* gameControl = createGameControl(this);

	GtkWidget* hbox = gtk_hbox_new (FALSE, 0); //horiztal box that is the whole program window
            gtk_box_pack_start(GTK_BOX(hbox), soccer_field, false, false, 0);
            gtk_box_pack_start(GTK_BOX(hbox), lateralMenu, false, false, 0);

	GtkWidget* vbox = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), menuBar, false, false, 0);
        gtk_box_pack_start(GTK_BOX(vbox), hbox, false, false, 0);
	gtk_box_pack_start(GTK_BOX(vbox), notebook, false, false, 0);
	//gtk_box_pack_start(GTK_BOX(vbox), gtk_hseparator_new(), false, false, 0);
        gtk_box_pack_start(GTK_BOX(vbox), statusBar, false, false, 0);
	//gtk_box_pack_start(GTK_BOX(vbox), gameControl, false, false, 0);

	gtk_container_add (GTK_CONTAINER (window), vbox); //insert vbox in the window
}
