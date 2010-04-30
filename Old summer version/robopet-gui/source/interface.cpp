#include<iostream>


#include "mainWindow.h"



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

			if( (event->x < ARENA_WIDTH) && (event->y < ARENA_HEIGHT) ) {

				mw->pathplan.finalpos.setXY( PIX_TO_MM(event->x), PIX_TO_MM(event->y));

				mw->pathplan.runPathplan();

				mw->cursorEvent = CURSOR_EVENT_NOTHING;
			}
			break;
	  }
  }

}




void setBolaPos(GtkWidget *widget, gpointer data)
{
/*
	//decodifica��o dos par�metros
	typeParameters* parametros = (typeParameters*) data;

	MainWindow* mw = parametros->mw;
	int bolax = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[0]);
	int bolay = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[1]);


	mw->bola.setXY(bolax, bolay);
*/
}



void pathplanButton(GtkWidget *widget, gpointer data)
//fun��o de callback do bot�o OK. ela deixar� "um clique pendente", para executar o pathplan apenas depois de o usu�rio clicar na posi��o final desejada
{
	typeParameters* parametros = (typeParameters*) data;
	MainWindow* mw = parametros->mw;



	if( gtk_toggle_button_get_active((GtkToggleButton*)widget)  ){


		//decodifica��o dos par�metros
		int pathplanIndex = gtk_combo_box_get_active((GtkComboBox*)parametros->widgets[0]);
		int jogador = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[1]);
		int isTime1 = gtk_toggle_button_get_active((GtkToggleButton*)parametros->widgets[2]);
		int isTime2 = gtk_toggle_button_get_active((GtkToggleButton*)parametros->widgets[3]);
		int checkPrintFull = gtk_toggle_button_get_active((GtkToggleButton*)parametros->widgets[4]);


		Point initialpos;
		if(isTime1)
			initialpos.setXY( mw->playersTeam1[jogador].getCurrentPosition().getX(),
                                          mw->playersTeam1[jogador].getCurrentPosition().getY());
		else
			initialpos.setXY( mw->playersTeam2[jogador].getCurrentPosition().getX(),
                                          mw->playersTeam2[jogador].getCurrentPosition().getY());

		mw->pathplan = Pathplan(initialpos,pathplanIndex,checkPrintFull); //carrega configura��es na classe Pathplan

		mw->pathplan.fillEnv(mw->playersTeam1, mw->playersTeam2); //configura matriz de posi��es


		gtk_button_set_label((GtkButton*)widget, "Running...");
		mw->cursorEvent = CURSOR_EVENT_PATHPLAN;
	}
	else{
		gtk_button_set_label((GtkButton*)widget, "Set Final Position");
		mw->pathplan.isDrawn = false;
	}
}



void playerManualControl(GtkWidget *widget, gpointer data)
//controle manual de um jogador (joystick ou teclado)
{
	//decodifica��o dos parametros
	typeParameters* parametros = (typeParameters*) data;
	int jogadorIndex = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[0]);
	int isTime1 = gtk_toggle_button_get_active((GtkToggleButton*)parametros->widgets[1]);
	int isTime2 = gtk_toggle_button_get_active((GtkToggleButton*)parametros->widgets[2]);
	//int stepsize = gtk_progress_get_value((GtkProgress*)parametros->widgets[3]);
	int stepsize = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[3]);
	MainWindow* mw = parametros->mw;



	string direction = gtk_button_get_label((GtkButton*)widget);
	if( isTime1 )
		switch(  direction[1]  ) {

				case 's':
					mw->playersTeam1[jogadorIndex].moveY(stepsize);
					break;

				case 'w':
					mw->playersTeam1[jogadorIndex].moveY(-stepsize);
					break;

				case 'a':
					mw->playersTeam1[jogadorIndex].moveX(-stepsize);
					break;

				case 'd':
					mw->playersTeam1[jogadorIndex].moveX(stepsize);
					break;

		}
	else //isTime2
		switch(  direction[1]  ) {

				case 's':
					mw->playersTeam2[jogadorIndex].moveY(stepsize);
					break;

				case 'w':
					mw->playersTeam2[jogadorIndex].moveY(-stepsize);
					break;

				case 'a':
					mw->playersTeam2[jogadorIndex].moveX(-stepsize);
					break;

				case 'd':
					mw->playersTeam2[jogadorIndex].moveX(stepsize);
					break;

		}

}

void openConnectionButton(GtkWidget *widget, gpointer data)
{
    //decodifica��o dos parametros
	typeParameters* parametros = (typeParameters*) data;
	MainWindow* mw = parametros->mw;
	int port = gtk_spin_button_get_value_as_int((GtkSpinButton*)parametros->widgets[0]);
	char *host = (char*) gtk_entry_get_text((GtkEntry*)parametros->widgets[1]);


	if( gtk_toggle_button_get_active((GtkToggleButton*)widget)  ) {

		mw->openCommunication(port, host);

		gtk_button_set_label((GtkButton*)widget, "Disconnect");
	}
	else{

                mw->closeCommunication();

		gtk_button_set_label((GtkButton*)widget, "Open Connection");
	}
}




////////////////////////////////////////////////////
/////////////////                  /////////////////
/////////////////   estrutura��o   /////////////////
/////////////////                  /////////////////
////////////////////////////////////////////////////


void createPathplanningTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 2///
	////////////
	GtkWidget* aba2 = gtk_label_new_with_mnemonic("Pathplanning");

	//widgets
	GtkWidget* label_pathplanners = gtk_label_new("Pathplanner: ");
	GtkWidget* pathplanners = gtk_combo_box_new_text();
		gtk_combo_box_insert_text( GTK_COMBO_BOX(pathplanners), 1, "RRT");
		gtk_combo_box_insert_text( GTK_COMBO_BOX(pathplanners), 2, "A*");
		gtk_combo_box_set_active( GTK_COMBO_BOX(pathplanners), 0);
	GtkWidget* jogador = gtk_spin_button_new_with_range(0, MAX_JOGADORES-1, 1); //spinner � limitado pela constante MAX_JOGADORES (isso n�o deve ficar assim)
	GtkWidget* jogadorLabel = gtk_label_new("Jogador: ");
	GtkWidget* time1 = gtk_radio_button_new_with_label(NULL,"Amarelo");
	GtkWidget* time2 = gtk_radio_button_new_with_label(gtk_radio_button_group (GTK_RADIO_BUTTON (time1)),"Azul");
	GtkWidget* checkPrintFull = gtk_check_button_new_with_label("Print full solution");
            gtk_toggle_button_set_active((GtkToggleButton*)checkPrintFull,TRUE);
	GtkWidget* checkPrintObstacles = gtk_check_button_new_with_label("Print obstacles");
	GtkWidget* ok = gtk_toggle_button_new_with_label("Set Final Position");
	//GtkWidget* xpos = gtk_label_new("x: ");
	//GtkWidget* finalposx = gtk_spin_button_new_with_range(0, ARENA_WIDTH_MM, 10);
	//GtkWidget* ypos = gtk_label_new("y: ");
	//GtkWidget* finalposy = gtk_spin_button_new_with_range(0, ARENA_HEIGHT_MM, 10);

	//hboxes
	GtkWidget* pathplannersBox = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(pathplannersBox), label_pathplanners, false, false, 0);
		gtk_box_pack_start(GTK_BOX(pathplannersBox), pathplanners, false, false, 0);
	GtkWidget* jogadorBox = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(jogadorBox), jogadorLabel, false, false, 0);
		gtk_box_pack_start(GTK_BOX(jogadorBox), jogador, false, false, 0);
		gtk_box_pack_start(GTK_BOX(jogadorBox), time1, false, false, 0);
		gtk_box_pack_start(GTK_BOX(jogadorBox), time2, false, false, 0);
	//GtkWidget* finalPosBox = gtk_hbox_new (FALSE, 0);
	//	gtk_box_pack_start(GTK_BOX(finalPosBox), xpos, false, false, 0);
	//	gtk_box_pack_start(GTK_BOX(finalPosBox), finalposx, false, false, 0);
	//	gtk_box_pack_start(GTK_BOX(finalPosBox), ypos, false, false, 0);
	//	gtk_box_pack_start(GTK_BOX(finalPosBox), finalposy, false, false, 0);

	//vboxes
	GtkWidget* menu2Box = gtk_vbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), pathplannersBox, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), jogadorBox, false, false, 0);
		//gtk_box_pack_start(GTK_BOX(menu2Box), finalPosBox, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), checkPrintFull, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu2Box), ok, false, false, 0);




	////////////////
	//// SINAIS ////

	//  clique bot�o OK
	//armazenamos as widgets que fornecer�o as informa��es para pathplanning
	static typeParameters parametros1;
	parametros1.mw = mw;
	parametros1.widgets.push_back(pathplanners);
	parametros1.widgets.push_back(jogador);
	parametros1.widgets.push_back(time1);
	parametros1.widgets.push_back(time2);
	parametros1.widgets.push_back(checkPrintFull);

	g_signal_connect(G_OBJECT(ok), "clicked", G_CALLBACK(pathplanButton), &parametros1);



	//insere aba no notebook pai
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menu2Box, aba2);

}

void createControlarTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 3///
	////////////
	GtkWidget* aba3 = gtk_label_new_with_mnemonic("Controlar");

	//widgets

	GtkWidget* jogador2 = gtk_spin_button_new_with_range(0, MAX_JOGADORES-1, 1);
	GtkWidget* jogadorLabel2 = gtk_label_new("Jogador: ");
	GtkWidget* time1_aba3 = gtk_radio_button_new_with_label(NULL,"Amarelo");
	GtkWidget* time2_aba3 = gtk_radio_button_new_with_label(gtk_radio_button_group (GTK_RADIO_BUTTON (time1_aba3)),"Azul");
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
	GtkWidget* button_bolapos = gtk_button_new_with_mnemonic("set Bola pos");



	//hboxes
	GtkWidget* timesBox2 = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(timesBox2), time1_aba3, false, false, 0);
		gtk_box_pack_start(GTK_BOX(timesBox2), time2_aba3, false, false, 0);
	GtkWidget* jogadorBox2 = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(jogadorBox2), jogadorLabel2, false, false, 0);
		gtk_box_pack_start(GTK_BOX(jogadorBox2), jogador2, false, false, 0);
		gtk_box_pack_start(GTK_BOX(jogadorBox2), timesBox2, false, false, 0);
	GtkWidget* controleslateraisBox = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(controleslateraisBox), gtk_label_new("stepsize: "), false, false, 0);
		gtk_box_pack_start(GTK_BOX(controleslateraisBox), stepsize, false, false, 0);
		gtk_box_pack_start(GTK_BOX(controleslateraisBox), control_up, false, false, 0);
		gtk_box_pack_start(GTK_BOX(controleslateraisBox), control_left, false, false, 0);
		gtk_box_pack_start(GTK_BOX(controleslateraisBox), control_down, false, false, 0);
		gtk_box_pack_start(GTK_BOX(controleslateraisBox), control_right, false, false, 0);
			//gtk_widget_set_size_request(controleslateraisBox,100,20);
	GtkWidget* controle_bola = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(controle_bola), label_bolax, false, false, 0);
		gtk_box_pack_start(GTK_BOX(controle_bola), bolax, false, false, 0);
		gtk_box_pack_start(GTK_BOX(controle_bola), label_bolay, false, false, 0);
		gtk_box_pack_start(GTK_BOX(controle_bola), bolay, false, false, 0);
		gtk_box_pack_start(GTK_BOX(controle_bola), button_bolapos, false, false, 0);


	//vboxes
	GtkWidget* menu3Box = gtk_vbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(menu3Box), jogadorBox2, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu3Box), controleslateraisBox, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menu3Box), controle_bola, false, false, 0);



	////////////////
	//// SINAIS ////

	//  clique dos bot�es de controle manual
	static typeParameters parametros3;
	parametros3.mw = mw;
	parametros3.widgets.push_back(jogador2);
	parametros3.widgets.push_back(time1_aba3);
	parametros3.widgets.push_back(time2_aba3);
	parametros3.widgets.push_back(stepsize);

	g_signal_connect(G_OBJECT(control_left), "clicked", G_CALLBACK(playerManualControl), &parametros3);
	g_signal_connect(G_OBJECT(control_right), "clicked", G_CALLBACK(playerManualControl), &parametros3);
	g_signal_connect(G_OBJECT(control_up), "clicked", G_CALLBACK(playerManualControl), &parametros3);
	g_signal_connect(G_OBJECT(control_down), "clicked", G_CALLBACK(playerManualControl), &parametros3);


	//  clique bot�o "set Bola pos"
	static typeParameters parametros4;
	parametros4.mw = mw;
	parametros4.widgets.push_back(bolax);
	parametros4.widgets.push_back(bolay);

	g_signal_connect(G_OBJECT(button_bolapos), "clicked", G_CALLBACK(setBolaPos), &parametros4);





	//insere aba no notebook pai
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menu3Box, aba3);
}

void createDisplayTab(MainWindow* mw, GtkWidget* notebook)
{
	////////////
	////ABA 4///
	////////////
	GtkWidget* aba = gtk_label_new_with_mnemonic("Display");

	//widgets
	GtkWidget* playerBody = gtk_check_button_new_with_label("show Body");
		gtk_toggle_button_set_active((GtkToggleButton*)playerBody,TRUE);
			mw->displaySettings.checkPlayerBody = playerBody;
	GtkWidget* playerAngle = gtk_check_button_new_with_label("show Angle");
		gtk_toggle_button_set_active((GtkToggleButton*)playerAngle,TRUE);
			mw->displaySettings.checkPlayerAngle = playerAngle;
	GtkWidget* playerIndex = gtk_check_button_new_with_label("show Index");
		gtk_toggle_button_set_active((GtkToggleButton*)playerIndex,TRUE);
			mw->displaySettings.checkPlayerIndex = playerIndex;
	GtkWidget* playerFuture = gtk_check_button_new_with_label("show Future");
                //gtk_toggle_button_set_active((GtkToggleButton*)playerFuture,TRUE);
			mw->displaySettings.checkPlayerFuture = playerFuture;
	GtkWidget* ballShow = gtk_check_button_new_with_label("show Ball");
		gtk_toggle_button_set_active((GtkToggleButton*)ballShow,TRUE);
			mw->displaySettings.checkBallShow = ballShow;
	GtkWidget* playersFrame = gtk_frame_new("Players");
	GtkWidget* ballFrame = gtk_frame_new("Ball");



	//vboxes
	GtkWidget* playersBox = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(playersBox), playerBody, false, false, 0);
		gtk_box_pack_start(GTK_BOX(playersBox), playerAngle, false, false, 0);
		gtk_box_pack_start(GTK_BOX(playersBox), playerIndex, false, false, 0);
		gtk_box_pack_start(GTK_BOX(playersBox), playerFuture, false, false, 0);
		gtk_container_add(GTK_CONTAINER(playersFrame),playersBox);//insere no frame

	GtkWidget* ballBox = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(ballBox), ballShow, false, false, 0);
		gtk_container_add(GTK_CONTAINER(ballFrame),ballBox);//insere no frame


	//hboxes
	GtkWidget* menuBox = gtk_hbox_new (FALSE, 0);
		gtk_box_pack_start(GTK_BOX(menuBox), playersFrame, false, false, 0);
		gtk_box_pack_start(GTK_BOX(menuBox), ballFrame, false, false, 0);


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
        GtkWidget* button = gtk_toggle_button_new_with_label("Open Communication");
        GtkWidget* portEntry = gtk_spin_button_new_with_range(0, 65536, 1);
            gtk_spin_button_set_value((GtkSpinButton*)portEntry,8100);
        GtkWidget* hostEntry = gtk_entry_new_with_max_length(15);
            gtk_entry_set_text((GtkEntry*)hostEntry,"143.54.12.116");


	//hboxes
	GtkWidget* box1 = gtk_hbox_new (FALSE, 0);
            gtk_box_pack_start(GTK_BOX(box1), gtk_label_new("Port: "), false, false, 0);
            gtk_box_pack_start(GTK_BOX(box1), portEntry, false, false, 0);


        GtkWidget* box2 = gtk_hbox_new (FALSE, 0);
            gtk_box_pack_start(GTK_BOX(box2), gtk_label_new("Host: "), false, false, 0);
            gtk_box_pack_start(GTK_BOX(box2), hostEntry, false, false, 0);

	//vboxes
	GtkWidget* menuBox = gtk_vbox_new (FALSE, 0);
                gtk_box_pack_start(GTK_BOX(menuBox), box1, false, false, 0);
                gtk_box_pack_start(GTK_BOX(menuBox), box2, false, false, 0);
                gtk_box_pack_start(GTK_BOX(menuBox), button, false, false, 0);
                //gtk_widget_set_size_request(menuBox, 200, 50);



	////////////////
	//// SINAIS ////
	//  clique no botão
	static typeParameters parametros;
	parametros.mw = mw;
	parametros.widgets.push_back(portEntry);
	parametros.widgets.push_back(hostEntry);

	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(openConnectionButton), &parametros);




	//insere aba no notebook pai
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), menuBox, aba);
}


GtkWidget* createNotebook(MainWindow* mw)
{
	GtkWidget* notebook = gtk_notebook_new();

	//createTab1(mw,notebook); //n�o tem nada aqui ainda
	createCommunicationTab(mw,notebook);
        createDisplayTab(mw,notebook);
        createControlarTab(mw,notebook);
	createPathplanningTab(mw,notebook);


	return notebook;
}

GtkWidget* createGameControl(MainWindow* mw)
{
	GtkWidget* box = gtk_hbox_new(FALSE, 0);

	GtkWidget* bar = gtk_hscale_new_with_range(0, 500, 1);
		gtk_widget_set_size_request(box, 1000, 20);

	//gtk_box_pack_start(GTK_BOX(box), bar, false, false, 0);

	return bar;
}


GtkWidget* createMenuBar(GtkWidget *window)
{

	GtkWidget* menubar;

  static GtkItemFactoryEntry menu_items[] = {
  { "/_File",         NULL,         NULL, 0, "<Branch>" },
  { "/File/_New",     "<control>N", NULL, 0, NULL },
  { "/File/Save _As", NULL,         NULL, 0, NULL },
  { "/File/sep1",     NULL,         NULL, 0, "<Separator>" },
  { "/File/Quit",     "<control>Q", gtk_main_quit, 0, NULL },
  { "/_Options",      NULL,         NULL, 0, "<Branch>" },
  { "/Options/Test",  NULL,         NULL, 0, NULL },
  { "/_Help",         NULL,         NULL, 0, "<LastBranch>" },
  { "/_Help/About",   NULL,         NULL, 0, NULL },
};


  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;
  gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

  accel_group = gtk_accel_group_new ();

  /* This function initializes the item factory.
     Param 1: The type of menu - can be GTK_TYPE_MENU_BAR, GTK_TYPE_MENU,
              or GTK_TYPE_OPTION_MENU.
     Param 2: The path of the menu.
     Param 3: A pointer to a gtk_accel_group.  The item factory sets up
              the accelerator table while generating menus.
  */

  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", accel_group);

  /* This function generates the menu items. Pass the item factory,
     the number of items in the array, the array itself, and any
     callback data for the the menu items. */
  gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);

  /* Attach the new accelerator group to the window. */
  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);


  /* Finally, return the actual menu bar created by the item factory. */
  return gtk_item_factory_get_widget (item_factory, "<main>");
}



void MainWindow::createInterface()
{

	//widgets gallery: http://library.gnome.org/devel/gtk/unstable/ch02.html


	GtkWidget* menuBar = createMenuBar(this->window);


	GtkWidget* notebook = createNotebook(this);
	GtkWidget* gameControl = createGameControl(this);



	GtkWidget* vbox = gtk_vbox_new (FALSE, 0); //vertical box que corresponde � tela inteira
	gtk_box_pack_start(GTK_BOX(vbox), menuBar, false, false, 0);
	gtk_box_pack_start(GTK_BOX(vbox), soccer_field, false, false, 0);
	gtk_box_pack_start(GTK_BOX(vbox), notebook, false, false, 0);
	gtk_box_pack_start(GTK_BOX(vbox), gtk_hseparator_new(), false, false, 0);
	gtk_box_pack_start(GTK_BOX(vbox), gameControl, false, false, 0);

	gtk_container_add (GTK_CONTAINER (window), vbox); //insere a vbox na janela

}
