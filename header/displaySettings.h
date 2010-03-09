#ifndef CLASS_DISPLAYCONFIG
#define CLASS_DISPLAYCONFIG

#include<iostream>

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>


using namespace std;


class DisplaySettings
{
	public:
		DisplaySettings() {};
		~DisplaySettings() {};

		GtkWidget* checkPlayerAngle;
		GtkWidget* checkPlayerIndex;
		GtkWidget* checkPlayerBody;
		GtkWidget* checkBallShow;
		GtkWidget* checkPlayerVector;
                GtkWidget* checkPlayerFuture;


		int isCheckPlayerAngle();
		int isCheckPlayerIndex();
		int isCheckPlayerBody();
		int isCheckBallShow();
		int isCheckPlayerVector();
                int isCheckPlayerFuture();
	

		void showAll();
		void hideAll();


};



#endif