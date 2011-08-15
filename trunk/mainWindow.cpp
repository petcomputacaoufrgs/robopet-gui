#include <mainWindow.h>
#include "interface.h"

using namespace std;

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <linux/joystick.h>



MainWindow::MainWindow()
{
	fieldWidth = 640;
	fieldHeight = MM_TO_PIX(ARENA_HEIGHT_MM);

	isVerbose = true;
	aitoguiClient = NULL;
	guitoaiServer = NULL;
	pathplan = NULL;
	
	joystickFd = -1;
	
	createInterface();
	createDrawingArea();
	gtk_widget_show_all(window);

	game.ball = GuiBall();
	//this->game.scaleFactorLength = &scaleFactorLength;
	//this->game.scaleFactorWidth = &scaleFactorWidth;
	
	//openClient(PORT_AI_TO_GUI, "localhost");
	
	pushStatusMessage("GUI initialized.");
}

void MainWindow::generateTextOutput()
{
    static char text[1024];
    sprintf(text," "); //resets text buffer

    sprintf(text+strlen(text),"Ball: %0.f,%0.f",game.ball.getCurrentPosition().getX(),game.ball.getCurrentPosition().getY());

    sprintf(text+strlen(text),"\n%i Yellow Players:",game.getNplayers(0));
    for( int i=0; i<game.getNplayers(0); i++ ) {
        sprintf(text+strlen(text),"\n- %i: %.0f,%.0f (%.0f°)",game.players[0][i].getId(), game.players[0][i].getCurrentPosition().getX(),game.players[0][i].getCurrentPosition().getY(),game.players[0][i].getCurrentAngle());
    }

    sprintf(text+strlen(text),"\n%i Blue Players:",game.getNplayers(1));
    for( int i=0; i<game.getNplayers(1); i++ ) {
        sprintf(text+strlen(text),"\n- %i: %.0f,%.0f (%.0f°)",game.players[1][i].getId(), game.players[1][i].getCurrentPosition().getX(),game.players[1][i].getCurrentPosition().getY(),game.players[1][i].getCurrentAngle());
    }

    fillTextOutput(text);
}

void dump_event(struct js_event e) {

	printf("----\n");
	printf("time: %u\n", e.time);
	printf("value: %s\n", e.value == 1 ? "DOWN" : "UP" );
	printf("type: %d\n", e.type);
	printf("number: %d\n", e.number);
}

void MainWindow::joystick()
{
	//cout << "iterated joy" << endl;
	
	unsigned int len = 0;
	struct js_event msg;

	if (joystickFd != -1)
	{
		len = read(joystickFd, &msg, sizeof(msg));

		if (len == sizeof(msg)) { //read was succesfull

			if (msg.type == JS_EVENT_BUTTON) { // seems to be a key press
				dump_event(msg);
			}
		}
	}
}

void MainWindow::iterate()
{
	communicate();
	
	joystick();
}	

void MainWindow::drawWorld()
{
	drawField();

	drawPlayers();

	drawPathplan();

	drawBall();
}

gint configure_event(GtkWidget *widget, GdkEventConfigure *event, MainWindow* mw)
{
	mw->pixmap = gdk_pixmap_new(widget->window,
						  widget->allocation.width,
						  widget->allocation.height,
						  -1);
	
	mw->cr = gdk_cairo_create(mw->pixmap);
	
	cairo_set_source_rgb (mw->cr, 1,1,1);
	cairo_rectangle (mw->cr, 0, 0, widget->allocation.width, widget->allocation.height);
	cairo_fill (mw->cr);

  return TRUE;
}


gint expose_event(GtkWidget *widget, GdkEventExpose *event, MainWindow* mw)
{
	mw->drawWorld();
	
	gdk_draw_pixmap(widget->window,
				  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
				  mw->pixmap,
				  event->area.x, event->area.y,
				  event->area.x, event->area.y,
				  event->area.width, event->area.height);

	return FALSE;
}

void MainWindow::updateScene()
{
	gtk_widget_draw(this->window, NULL);
}

void updateSceneCB(MainWindow* mw)
{
	gtk_widget_draw(mw->window, NULL);
}

void iterateCB(MainWindow *mw)
{
	mw->iterate();
}

void MainWindow::createDrawingArea()
{
	gtk_widget_set_size_request(soccer_field, fieldWidth, fieldHeight);
	
	g_signal_connect(G_OBJECT(soccer_field), "configure_event", G_CALLBACK(configure_event), this);    
    g_signal_connect(G_OBJECT(soccer_field), "expose_event", G_CALLBACK(expose_event), this);    
    gtk_signal_connect (GTK_OBJECT (soccer_field), "button_press_event", (GtkSignalFunc) button_press_event, this);
		gtk_widget_set_events (soccer_field, GDK_KEY_PRESS_MASK|GDK_BUTTON_PRESS_MASK);
		cursorEvent = CURSOR_EVENT_NOTHING;
	
	// Calls iteration whenever possible
	g_idle_add((GSourceFunc)iterateCB, this);
	
	// Calls drawing at least each 500 miliseconds
	//g_timeout_add(500, (GSourceFunc)updateSceneCB, this);
}

double MainWindow::PIX_TO_MM(double x) { return x*ARENA_WIDTH_MM/fieldWidth; }
//double MainWindow::PIX_TO_MM_Y(double x) { return x*ARENA_WIDTH_MM/fieldHeight; }

double MainWindow::MM_TO_PIX(double x) { return x*fieldWidth/ARENA_WIDTH_MM; }
