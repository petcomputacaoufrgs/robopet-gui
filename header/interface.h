#ifndef _INTERFACE_H
#define	_INTERFACE_H

#include<iostream>


#include "mainWindow.h"



//////////////////////////////////////////////////
///////////////               ////////////////////
///////////////   callbacks   ////////////////////
///////////////               ////////////////////
//////////////////////////////////////////////////


void button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer data);

void key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer data);

void setBolaPos(GtkWidget *widget, gpointer data);

void pathplanButton(GtkWidget *widget, gpointer data);

void openConnectionButton(GtkWidget *widget, gpointer data);



/////////////////////////////////////////////////
/////////////////               /////////////////
/////////////////   estrutura   /////////////////
/////////////////               /////////////////
/////////////////////////////////////////////////


void createPathplanningTab(MainWindow* mw, GtkWidget* notebook);

void createControlarTab(MainWindow* mw, GtkWidget* notebook);

void createDisplayTab(MainWindow* mw, GtkWidget* notebook);

void createCommunicationTab(MainWindow* mw, GtkWidget* notebook);

void createLauncherTab(MainWindow* mw, GtkWidget* notebook);

GtkWidget* createNotebook(MainWindow* mw);

GtkWidget* createGameControl(MainWindow* mw);

GtkWidget* createMenuBar(GtkWidget *window);

void processSpecialKeys(int key, int x, int y);

#endif	/* _INTERFACE_H */

