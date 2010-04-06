#ifndef CLASS_DISPLAYCONFIG
#define CLASS_DISPLAYCONFIG

#include<iostream>

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>


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


		int isHidePlayerAngle();
		int isHidePlayerIndex();
		int isHidePlayerBody();
		int isHideBall();
		int isHidePlayerVector();
                int isHidePlayerFuture();
	

		void showAll();
		void hideAll();


};



#endif