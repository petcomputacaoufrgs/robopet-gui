#include<iostream>
#include <fstream>


#include "interface.h"



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
//CALLBACK que capture todos cliques de mouse
{
  MainWindow* mw = (MainWindow*) data;


  if (event->button == 1 ){

	  switch(mw->cursorEvent) {


		case CURSOR_EVENT_NOTHING:
			break;


		case CURSOR_EVENT_PATHPLAN:
			if( (event->x < ARENA_WIDTH) && (event->y < ARENA_HEIGHT) )
                        {
				mw->pathplan.finalpos.setXY( PIX_TO_MM(event->x), PIX_TO_MM(event->y));
				mw->pathplan.runPathplan( mw->pathplan.pathplanIndex );

				mw->cursorEvent = CURSOR_EVENT_NOTHING;
			}
			break;
	  }
  }

}


void MainWindow::fillTextView(char text[])
{
    if( this->textView ) {

        GtkTextBuffer *text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(this->textView));
        gtk_text_buffer_set_text (text_buffer, text, -1);
    }
}


void setBallPos(GtkWidget *widget, gpointer data)
{
        // parameters
	typeParameters* parametros = (typeParameters*) data;

	MainWindow* mw = parametros->mw;
	int ballx = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[0]);
	int bally = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[1]);


	mw->game.ball.setCurrentPosition(Point(ballx, bally));
}


void pathplanButton(GtkWidget *widget, gpointer data)
//fun��o de callback do bot�o OK. ela deixar� "um clique pendente", para executar o pathplan apenas depois de o usu�rio clicar na posi��o final desejada
{
	typeParameters* parametros = (typeParameters*) data;
	MainWindow* mw = parametros->mw;



	if( gtk_toggle_button_get_active((GtkToggleButton*)widget)  ){


		//decodifica��o dos par�metros
		int pathplanIndex = gtk_combo_box_get_active((GtkComboBox*)parametros->widgets[0]);
		//int jogador = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[1]);
		//int isTime1 = gtk_toggle_button_get_active((GtkToggleButton*)parametros->widgets[2]);
		//int isTime2 = gtk_toggle_button_get_active((GtkToggleButton*)parametros->widgets[3]);
		int checkPrintFull = gtk_toggle_button_get_active((GtkToggleButton*)parametros->widgets[1]);

                int playerTeam, playerIndex;
                playerIndex = mw->getSelectedPlayer(playerTeam);


		Point initialpos;
		if(playerTeam==1)
			initialpos.setXY( mw->game.playersTeam1[playerIndex].getCurrentPosition().getX(),
                                          mw->game.playersTeam1[playerIndex].getCurrentPosition().getY());
		else
			initialpos.setXY( mw->game.playersTeam2[playerIndex].getCurrentPosition().getX(),
                                          mw->game.playersTeam2[playerIndex].getCurrentPosition().getY());

		mw->pathplan = guiPathplan(initialpos,pathplanIndex,checkPrintFull); //carrega configura��es na classe Pathplan

                vector<RP::Point> positions;
                for(int i=0; i<(int)mw->game.getNplayersTeam1(); i++)
                    positions.push_back(mw->game.playersTeam1[i].getCurrentPosition());
                for(int i=0; i<(int)mw->game.getNplayersTeam2(); i++)
                    positions.push_back(mw->game.playersTeam2[i].getCurrentPosition());
                mw->pathplan.fillEnv(positions); //configura matriz de posi��es


		gtk_button_set_label((GtkButton*)widget, "Running...");
                //mw->pushStatusMessage("Pathplanning is running.");
		mw->cursorEvent = CURSOR_EVENT_PATHPLAN;
                mw->pathplan.isDrawn = true;
	}
	else{
		gtk_button_set_label((GtkButton*)widget, "Set Destination");
                //mw->pushStatusMessage("Waiting for destination definition.");
		mw->pathplan.isDrawn = false;
	}

}


int MainWindow::getSelectedPlayer(int &playerTeam)
{
        int comboBoxIndex = gtk_combo_box_get_active((GtkComboBox*)this->game.playersComboBox);
        int nPlayersTeam1 = this->game.getNplayersTeam1();
        int nPlayersTeam2 = this->game.getNplayersTeam2();

        int playerIndex;
        
        comboBoxIndex < nPlayersTeam1?  playerTeam = 1 : playerTeam = 2;

        playerTeam == 1?  playerIndex = comboBoxIndex : playerIndex = comboBoxIndex - nPlayersTeam1;

        return playerIndex;

}


void isVerboseButton(GtkWidget *widget, gpointer data)
{
    	// parameters
	typeParameters* parametros = (typeParameters*) data;
	MainWindow* mw = parametros->mw;

        if ( gtk_toggle_button_get_active((GtkToggleButton*)widget) )
            mw->isVerbose = true;
        else
            mw->isVerbose = false;

}


void playerManualControl(GtkWidget *widget, gpointer data)
//controle manual de um jogador (joystick ou teclado)
{
	// parameters
	typeParameters* parametros = (typeParameters*) data;
	int stepsize = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[0]);
	MainWindow* mw = parametros->mw;

        int playerTeam, playerIndex;
        playerIndex = mw->getSelectedPlayer(playerTeam);

        if( playerIndex!=-1 ) {
            string direction = gtk_button_get_label((GtkButton*)widget);
            if( playerTeam == 1 )
                    switch(  direction[1]  ) {

                                    case 's':
                                            mw->game.playersTeam1[playerIndex].setCurrentPositionY( mw->game.playersTeam1[playerIndex].getCurrentPosition().getY() + stepsize );
                                            break;

                                    case 'w':
                                            mw->game.playersTeam1[playerIndex].setCurrentPositionY( mw->game.playersTeam1[playerIndex].getCurrentPosition().getY() - stepsize );
                                            break;

                                    case 'a':
                                            mw->game.playersTeam1[playerIndex].setCurrentPositionX( mw->game.playersTeam1[playerIndex].getCurrentPosition().getX() - stepsize );
                                            break;

                                    case 'd':
                                            mw->game.playersTeam1[playerIndex].setCurrentPositionX( mw->game.playersTeam1[playerIndex].getCurrentPosition().getX() + stepsize );
                                            break;

                                    case 'q':
                                            mw->game.playersTeam1[playerIndex].setCurrentAngle( mw->game.playersTeam1[playerIndex].getCurrentAngle() - 10 );
                                            break;

                                    case 'e':
                                            mw->game.playersTeam1[playerIndex].setCurrentAngle( mw->game.playersTeam1[playerIndex].getCurrentAngle() + 10 );
                                            break;

                    }
            else if( playerTeam == 2)
                    switch(  direction[1]  ) {

                                    case 's':
                                            mw->game.playersTeam2[playerIndex].setCurrentPositionY( mw->game.playersTeam2[playerIndex].getCurrentPosition().getY() + stepsize );
                                            break;

                                    case 'w':
                                            mw->game.playersTeam2[playerIndex].setCurrentPositionY( mw->game.playersTeam2[playerIndex].getCurrentPosition().getY() - stepsize );
                                            break;

                                    case 'a':
                                            mw->game.playersTeam2[playerIndex].setCurrentPositionX( mw->game.playersTeam2[playerIndex].getCurrentPosition().getX() - stepsize );
                                            break;

                                    case 'd':
                                            mw->game.playersTeam2[playerIndex].setCurrentPositionX( mw->game.playersTeam2[playerIndex].getCurrentPosition().getX() + stepsize );
                                            break;

                                    case 'q':
                                            mw->game.playersTeam2[playerIndex].setCurrentAngle( mw->game.playersTeam2[playerIndex].getCurrentAngle() - 10 );
                                            break;

                                    case 'e':
                                            mw->game.playersTeam2[playerIndex].setCurrentAngle( mw->game.playersTeam2[playerIndex].getCurrentAngle() + 10 );
                                            break;

                    }
        }

}


void clientCommunicationButton(GtkWidget *widget, gpointer data)
{
        // parameters
	typeParameters* parametros = (typeParameters*) data;
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
	typeParameters* parametros = (typeParameters*) data;
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
	typeParameters* parametros = (typeParameters*) data;
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
	typeParameters* parametros = (typeParameters*) data;
	MainWindow* mw = parametros->mw;

        mw->game.addPlayerTeam1();

        //mw->pushStatusMessage("Added 1 Yellow Player.");
}


void addBluePlayerButton(GtkWidget *widget, gpointer data)
{
        // parameters
	typeParameters* parametros = (typeParameters*) data;
	MainWindow* mw = parametros->mw;

        mw->game.addPlayerTeam2();

        //mw->pushStatusMessage("Added 1 Blue Player.");
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



void createPathplanningTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 2///
	////////////
	GtkWidget* aba2 = gtk_label_new_with_mnemonic("Pathplanning");

	//widgets
	GtkWidget* label_pathplanners = gtk_label_new("Pathplanner: ");
	GtkWidget* pathplanners = gtk_combo_box_new_text();
		gtk_combo_box_insert_text( GTK_COMBO_BOX(pathplanners), PATHPLAN_RRT, "RRT");
		gtk_combo_box_insert_text( GTK_COMBO_BOX(pathplanners), PATHPLAN_ASTAR, "A*");
		gtk_combo_box_set_active( GTK_COMBO_BOX(pathplanners), 0);
	
	GtkWidget* checkPrintFull = gtk_check_button_new_with_label("Print full solution");
            gtk_toggle_button_set_active((GtkToggleButton*)checkPrintFull,TRUE);
	GtkWidget* checkPrintObstacles = gtk_check_button_new_with_label("Print obstacles");
	GtkWidget* ok = gtk_toggle_button_new_with_label("Set Destination");


	//hboxes
	GtkWidget* pathplannersBox = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(pathplannersBox), label_pathplanners, false, false, 0);
		gtk_box_pack_start(GTK_BOX(pathplannersBox), pathplanners, false, false, 0);
	GtkWidget* jogadorBox = gtk_hbox_new (FALSE, 0);


	//vboxes
	GtkWidget* menu2Box = gtk_vbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), pathplannersBox, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), jogadorBox, false, false, 0);
		//gtk_box_pack_start(GTK_BOX(menu2Box), finalPosBox, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), checkPrintFull, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), ok, false, false, 0);




	/////////////////
	//// SIGNALS ////

	//  OK button
	static typeParameters parametros1;
	parametros1.mw = mw;
	parametros1.widgets.push_back(pathplanners);
	parametros1.widgets.push_back(checkPrintFull);

	g_signal_connect(G_OBJECT(ok), "clicked", G_CALLBACK(pathplanButton), &parametros1);



	//insere aba no notebook pai
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menu2Box, aba2);

}

void createControlTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 3///
	////////////
	GtkWidget* aba3 = gtk_label_new_with_mnemonic("Manual control");

	//widgets

	//GtkWidget* jogador2 = gtk_spin_button_new_with_range(0, MAX_JOGADORES-1, 1);
	//GtkWidget* jogadorLabel2 = gtk_label_new("Jogador: ");
	//GtkWidget* time1_aba3 = gtk_radio_button_new_with_label(NULL,"Amarelo");
	//GtkWidget* time2_aba3 = gtk_radio_button_new_with_label(gtk_radio_button_group (GTK_RADIO_BUTTON (time1_aba3)),"Azul");
	//GtkWidget* stepsize = gtk_hscale_new_with_range(0, 500, 1);
	//	gtk_widget_set_size_request(stepsize, 100, 15);
	GtkWidget* stepsize = gtk_spin_button_new_with_range(0, 500, 1);
            gtk_spin_button_set_value((GtkSpinButton*)stepsize,100);

	GtkWidget* control_up = gtk_button_new_with_mnemonic("_w");
	GtkWidget* control_down = gtk_button_new_with_mnemonic("_s");
	GtkWidget* control_left = gtk_button_new_with_mnemonic("_a");
	GtkWidget* control_right = gtk_button_new_with_mnemonic("_d");
	GtkWidget* label_bolax = gtk_label_new("x: ");
	GtkWidget* bolax = gtk_spin_button_new_with_range(0, ARENA_WIDTH_MM, 10);
	GtkWidget* label_bolay = gtk_label_new("y: ");
	GtkWidget* bolay = gtk_spin_button_new_with_range(0, ARENA_HEIGHT_MM, 10);
	GtkWidget* button_ballpos = gtk_button_new_with_mnemonic("set ball pos");
        GtkWidget* rotate_cw = gtk_button_new_with_mnemonic("_q");
        GtkWidget* rotate_ccw = gtk_button_new_with_mnemonic("_e");


        GtkWidget* movementControlBox1 = gtk_hbox_new (FALSE, 0);
                gtk_box_pack_start(GTK_BOX(movementControlBox1), gtk_label_new("stepsize: "), false, false, 0);
		gtk_box_pack_start(GTK_BOX(movementControlBox1), stepsize, false, false, 0);

        GtkWidget* movementControlBox2 = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(movementControlBox2), control_up, false, false, 0);
		gtk_box_pack_start(GTK_BOX(movementControlBox2), control_left, false, false, 0);
		gtk_box_pack_start(GTK_BOX(movementControlBox2), control_down, false, false, 0);
		gtk_box_pack_start(GTK_BOX(movementControlBox2), control_right, false, false, 0);
                
        GtkWidget* movementControlBox3 = gtk_hbox_new (FALSE, 0);
                gtk_box_pack_start(GTK_BOX(movementControlBox3), gtk_label_new("(rotate CW/CCW)"), false, false, 0);
                gtk_box_pack_start(GTK_BOX(movementControlBox3), rotate_cw, false, false, 0);
		gtk_box_pack_start(GTK_BOX(movementControlBox3), rotate_ccw, false, false, 0);


        GtkWidget* ballControlBox2 = gtk_hbox_new (FALSE, 0);
                gtk_box_pack_start(GTK_BOX(ballControlBox2), label_bolax, false, false, 0);
		gtk_box_pack_start(GTK_BOX(ballControlBox2), bolax, false, false, 0);
		gtk_box_pack_start(GTK_BOX(ballControlBox2), label_bolay, false, false, 0);
		gtk_box_pack_start(GTK_BOX(ballControlBox2), bolay, false, false, 0);

        GtkWidget* ballControlBox = gtk_vbox_new (FALSE, 0);
                gtk_box_pack_start(GTK_BOX(ballControlBox), ballControlBox2, false, false, 0);
                gtk_box_pack_start(GTK_BOX(ballControlBox), button_ballpos, false, false, 0);

        GtkWidget* playersControl = gtk_vbox_new (FALSE, 0);
                gtk_box_pack_start(GTK_BOX(playersControl), movementControlBox1, false, false, 0);
                gtk_box_pack_start(GTK_BOX(playersControl), movementControlBox2, false, false, 0);
                gtk_box_pack_start(GTK_BOX(playersControl), movementControlBox3, false, false, 0);

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

	//  manual control buttons
	static typeParameters parametros3;
	parametros3.mw = mw;
        parametros3.widgets.push_back(stepsize);

	g_signal_connect(G_OBJECT(control_left), "clicked", G_CALLBACK(playerManualControl), &parametros3);
	g_signal_connect(G_OBJECT(control_right), "clicked", G_CALLBACK(playerManualControl), &parametros3);
	g_signal_connect(G_OBJECT(control_up), "clicked", G_CALLBACK(playerManualControl), &parametros3);
	g_signal_connect(G_OBJECT(control_down), "clicked", G_CALLBACK(playerManualControl), &parametros3);
	g_signal_connect(G_OBJECT(rotate_cw), "clicked", G_CALLBACK(playerManualControl), &parametros3);
	g_signal_connect(G_OBJECT(rotate_ccw), "clicked", G_CALLBACK(playerManualControl), &parametros3);

	//  set Bola pos
	static typeParameters parametros5;
	parametros5.mw = mw;
	parametros5.widgets.push_back(bolax);
	parametros5.widgets.push_back(bolay);

	g_signal_connect(G_OBJECT(button_ballpos), "clicked", G_CALLBACK(setBallPos), &parametros5);





	//insere aba no notebook pai
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menu3Box, aba3);
}

void createSettingsTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 4///
	////////////
	GtkWidget* aba = gtk_label_new_with_mnemonic("Settings");

	//widgets
        //Players
        GtkWidget* playersFrame = gtk_frame_new("Players");
	GtkWidget* playerBody = gtk_check_button_new_with_label("hide Body");
		//gtk_toggle_button_set_active((GtkToggleButton*)playerBody,TRUE);
		mw->displaySettings.checkPlayerBody = playerBody;
	GtkWidget* playerAngle = gtk_check_button_new_with_label("hide Angle");
		//gtk_toggle_button_set_active((GtkToggleButton*)playerAngle,TRUE);
		mw->displaySettings.checkPlayerAngle = playerAngle;
	GtkWidget* playerIndex = gtk_check_button_new_with_label("hide Index");
		//gtk_toggle_button_set_active((GtkToggleButton*)playerIndex,TRUE);
		mw->displaySettings.checkPlayerIndex = playerIndex;
	GtkWidget* playerFuture = gtk_check_button_new_with_label("hide Future");
                //gtk_toggle_button_set_active((GtkToggleButton*)playerFuture,TRUE);
		mw->displaySettings.checkPlayerFuture = playerFuture;
        //Ball
        GtkWidget* ballFrame = gtk_frame_new("Ball");
        GtkWidget* ballHide = gtk_check_button_new_with_label("hide Ball");
		//gtk_toggle_button_set_active((GtkToggleButton*)ballHide,TRUE);
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
		gtk_container_add(GTK_CONTAINER(playersFrame),playersBox);//insere no frame

	GtkWidget* ballBox = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(ballBox), ballHide, false, false, 0);
		gtk_container_add(GTK_CONTAINER(ballFrame),ballBox);//insere no frame

        GtkWidget* programBox = gtk_vbox_new(FALSE, 0);
                gtk_box_pack_start(GTK_BOX(programBox), isVerbose, false, false, 0);
                gtk_container_add(GTK_CONTAINER(programFrame),programBox);//insere no frame


	//hboxes
	GtkWidget* menuBox = gtk_hbox_new (FALSE, 0);
                gtk_box_pack_start(GTK_BOX(menuBox), programFrame, false, false, 0);
                gtk_box_pack_start(GTK_BOX(menuBox), playersFrame, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menuBox), ballFrame, false, false, 0);





        /////////////////
	//// SIGNALS ////
	//  CHECK BOX: "be Verbose"
	static typeParameters parametros;
	parametros.mw = mw;
	g_signal_connect(G_OBJECT(isVerbose), "clicked", G_CALLBACK(isVerboseButton), &parametros);





        //insere aba no notebook pai
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menuBox, aba);
}

void createCommunicationTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 5///
	////////////
	GtkWidget* aba = gtk_label_new_with_mnemonic("Communication");

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
	static typeParameters parametros;
	parametros.mw = mw;
	parametros.widgets.push_back(client_portEntry);
	parametros.widgets.push_back(client_hostEntry);

	g_signal_connect(G_OBJECT(client_button), "clicked", G_CALLBACK(clientCommunicationButton), &parametros);

        static typeParameters parametros2;
	parametros2.mw = mw;
	parametros2.widgets.push_back(server_portEntry);
	parametros2.widgets.push_back(server_hostEntry);

	g_signal_connect(G_OBJECT(server_button), "clicked", G_CALLBACK(serverCommunicationButton), &parametros2);


        static typeParameters parametros3;
	parametros3.mw = mw;
	parametros3.widgets.push_back(client_hostEntry);
        parametros3.widgets.push_back(server_hostEntry);
        parametros3.widgets.push_back(client_button_getlocalip);
        parametros3.widgets.push_back(server_button_getlocalip);

	g_signal_connect(G_OBJECT(client_button_getlocalip), "clicked", G_CALLBACK(getLocalIPButton), &parametros3);
        g_signal_connect(G_OBJECT(server_button_getlocalip), "clicked", G_CALLBACK(getLocalIPButton), &parametros3);




	//insere aba no notebook pai
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menuBox, aba);
}


GtkWidget* createNotebook(MainWindow* mw)
{
	GtkWidget* notebook = gtk_notebook_new();

	//createTab1(mw,notebook); //n�o tem nada aqui ainda
	createCommunicationTab(mw,notebook);
        createSettingsTab(mw,notebook);
        createControlTab(mw,notebook);
	createPathplanningTab(mw,notebook);


	return notebook;
}

GtkWidget* createLateralMenu(MainWindow* mw)
{
    
    mw->textView = gtk_text_view_new();

    
    GtkWidget* jogadores = gtk_combo_box_new_text();
                mw->game.playersComboBox = jogadores; //seta um ponteiro que lembrará desta widget, para poder modificá-la sempre que o número de jogadores for modificado

    GtkWidget* addBluePlayer = gtk_button_new_with_mnemonic("Blue++");
    GtkWidget* addYellowPlayer = gtk_button_new_with_mnemonic("Yellow++");

    GtkWidget* addPlayersBox = gtk_hbox_new (FALSE, 0);
            gtk_box_pack_start(GTK_BOX(addPlayersBox), addBluePlayer, false, false, 0);
            gtk_box_pack_start(GTK_BOX(addPlayersBox), addYellowPlayer, false, false, 0);

    
    GtkWidget* playerFrameVBOX = gtk_vbox_new (FALSE, 0);
        gtk_container_add(GTK_CONTAINER(playerFrameVBOX),jogadores);
        gtk_container_add(GTK_CONTAINER(playerFrameVBOX),addPlayersBox);

    GtkWidget* playersFrame = gtk_frame_new("Players");
        gtk_container_add(GTK_CONTAINER(playersFrame),playerFrameVBOX);


    GtkWidget* lateralMenu = gtk_vbox_new (FALSE, 0);
            gtk_box_pack_start(GTK_BOX(lateralMenu), playersFrame, false, false, 0);
            gtk_box_pack_start(GTK_BOX(lateralMenu), mw->textView, false, false, 0);

   

	////////////////
	//// SINAIS ////
	//  BUTTON: addBluePlayer
        //  BUTTON: yellowBluePlayer
	static typeParameters parametros;
	parametros.mw = mw;
	g_signal_connect(G_OBJECT(addBluePlayer), "clicked", G_CALLBACK(addBluePlayerButton), &parametros);
	g_signal_connect(G_OBJECT(addYellowPlayer), "clicked", G_CALLBACK(addYellowPlayerButton), &parametros);



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
	    (NULL,
	     GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
	     GTK_BUTTONS_CLOSE,
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

	gtk_container_add (GTK_CONTAINER (window), vbox); //insere a vbox na janela

}
