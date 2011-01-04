#include <mainWindow.h>
#include "interface.h"

using namespace std;


MainWindow::MainWindow(string name)
{
	createInterface();
	createDrawingArea();
	gtk_widget_show_all(window);

	isVerbose = false;

	aitoguiClient = NULL;
	guitoaiServer = NULL;
	pathplan = NULL;
	toDrawPathplan = false;

	game.ball = GuiBall();
	
	//pushStatusMessage("GUI initialized.");
}

void MainWindow::generateTextOutput()
{
    static char text[1024];
    sprintf(text,""); //resets text buffer

    sprintf(text+strlen(text),"Ball: %0.f,%0.f",game.ball.getCurrentPosition().getX(),game.ball.getCurrentPosition().getY());

    sprintf(text+strlen(text),"\n%i Yellow Players:",game.getNplayers(0));
    for( int i=0; i<game.getNplayers(0); i++ ) {
        sprintf(text+strlen(text),"\n- %i: %.0f,%.0f (%.0f°)",i, game.players[0][i].getCurrentPosition().getX(),game.players[0][i].getCurrentPosition().getY(),game.players[0][i].getCurrentAngle());
    }

    sprintf(text+strlen(text),"\n%i Blue Players:",game.getNplayers(1));
    for( int i=0; i<game.getNplayers(1); i++ ) {
        sprintf(text+strlen(text),"\n- %i: %.0f,%.0f (%.0f°)",i, game.players[1][i].getCurrentPosition().getX(),game.players[1][i].getCurrentPosition().getY(),game.players[1][i].getCurrentAngle());
    }

    fillTextOutput(text);
}



		
	
	void MainWindow::iterate()
{
	communicate();

	drawWorld();

	generateTextOutput();
}


void MainWindow::drawWorld()
{
	drawField();

	drawPlayers();

	drawPathplan();

	cairo_t *cr = gdk_cairo_create( pixmap );
	cairo_set_source_rgb(cr, ORANGE);
	game.ball.draw(cr, displaySettings);
}

gint configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
	MainWindow * mw = (MainWindow*) data;

	mw->pixmap = gdk_pixmap_new(widget->window,
						  widget->allocation.width,
						  widget->allocation.height,
						  -1);
	
	gdk_draw_rectangle (mw->pixmap,
					  widget->style->white_gc,
					  TRUE,
					  0, 0,
					  widget->allocation.width,
					  widget->allocation.height);

  return TRUE;
}


gint expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	MainWindow * mw = (MainWindow*) data;

	mw->iterate();
	cout << "updated" << endl;
	
	gdk_draw_pixmap(widget->window,
				  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
				  mw->pixmap,
				  event->area.x, event->area.y,
				  event->area.x, event->area.y,
				  event->area.width, event->area.height);

	return FALSE;
}

gboolean renderScene(GtkWidget * widget, GdkEvent *event, gpointer data)
{
	cout << "renderscene" << endl;
        gtk_widget_draw(widget, NULL);

        return TRUE;
}


void timeoutAdd(GtkWidget *widget, MainWindow* mw)
{
        if (mw->timeout_handler_id == 0)
                mw->timeout_handler_id = gtk_timeout_add(TIMEOUT_INTERVAL, (GtkFunction)renderScene, widget);
}


void timeoutRemove(GtkWidget *widget, MainWindow* mw)
{
        if (mw->timeout_handler_id != 0) {
                gtk_timeout_remove(mw->timeout_handler_id);
                mw->timeout_handler_id = 0;
        }
}

void MainWindow::createDrawingArea()
{
	gtk_widget_set_size_request(soccer_field, ARENA_WIDTH, ARENA_HEIGHT);

	g_signal_connect(G_OBJECT(soccer_field), "configure_event", G_CALLBACK(configure_event), this);    
    g_signal_connect(G_OBJECT(soccer_field), "expose_event", G_CALLBACK(expose_event), this);    
	g_signal_connect(G_OBJECT(soccer_field), "map_event",  G_CALLBACK(timeoutAdd), soccer_field);
	g_signal_connect(G_OBJECT(soccer_field), "unmap_event", G_CALLBACK(timeoutRemove), soccer_field);
	g_signal_connect(G_OBJECT(soccer_field), "unrealize", G_CALLBACK(timeoutRemove), soccer_field);
    gtk_signal_connect (GTK_OBJECT (soccer_field), "button_press_event", (GtkSignalFunc) button_press_event, this);
		gtk_widget_set_events (soccer_field, GDK_KEY_PRESS_MASK|GDK_BUTTON_PRESS_MASK);
		cursorEvent = CURSOR_EVENT_NOTHING;
		
	
}
