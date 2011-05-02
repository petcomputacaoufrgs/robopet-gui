#ifndef CLASS_DISPLAYCONFIG
#define CLASS_DISPLAYCONFIG

#include<iostream>

#include <gtk/gtk.h>


using namespace std;


class DisplaySettings
{
	public:

		GtkWidget* checkPlayerAngle;
		GtkWidget* checkPlayerIndex;
		GtkWidget* checkPlayerBody;
		GtkWidget* checkBall;
		GtkWidget* checkPlayerVector;
        GtkWidget* checkPlayerFuture;
        GtkWidget* checkPlayerPath;


		int isHidePlayerAngle();
		int isHidePlayerIndex();
		int isHidePlayerBody();
		int isHideBall();
        int isHidePlayerFuture();
        int isHidePlayerPath();
	

		void showAll();
		void hideAll();

};


#endif
