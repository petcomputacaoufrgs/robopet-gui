
#include <mainWindow.h>
#include "interface.h"

using namespace std;


//MainWindow::MainWindow() {}

MainWindow::MainWindow(string name)
{
        configuraGL();

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	soccer_field = gtk_drawing_area_new();


        createMWindow("RoboPet GUI");

        pushStatusMessage("GUI initialized.");


        isVerbose = false;

        aitoguiClient = NULL;
        guitoaiServer = NULL;

	game.ball = GuiBall();
}


void MainWindow::configuraGL()
{
	//Configure OpenGL-capable visual.
	//Try double-buffered visual
	glconfig = gdk_gl_config_new_by_mode ((GdkGLConfigMode) (GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH  | GDK_GL_MODE_DOUBLE));
	if (glconfig == NULL)
	{
		g_print("In MainWindow::configuraGL(): Cannot find the double-buffered visual.\n");
		g_print("In MainWindow::configuraGL(): Trying single-buffered visual.\n");

		//Try single-buffered visual
		glconfig = gdk_gl_config_new_by_mode ((GdkGLConfigMode) (GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH));
		if (glconfig == NULL)
		{
			g_print("In MainWindow::configuraGL(): No appropriate OpenGL-capable visual found.\n");
		    exit (1);
		}
	}


}

void MainWindow::createMWindow(string title)
{
	gtk_window_set_title(GTK_WINDOW(window), title.c_str());
	gtk_container_set_reallocate_redraws(GTK_CONTAINER(window), TRUE);


	createDrawingArea();
	createInterface();


	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window);
}


void realize(GtkWidget *widget, MainWindow* mw, gpointer   data)
{
  GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
    return;


  glClearColor(DARK_GREEN, 1.0);
  glClearDepth(1.0);

  gdk_gl_drawable_gl_end (gldrawable);
}

gboolean configureEvent(GtkWidget *widget, MainWindow* mw, GdkEventConfigure *event, gpointer data)
{
  GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

  GLfloat w = widget->allocation.width;
  GLfloat h = widget->allocation.height;

  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
    return FALSE;

  glViewport(0, 0, w, h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho(0, ARENA_WIDTH, ARENA_HEIGHT, 0, 0, 1);
  glMatrixMode (GL_MODELVIEW);

  gdk_gl_drawable_gl_end (gldrawable);


  return TRUE;
}



void MainWindow::generateTextOutput()
{
    static char debugText[1024];
    
    sprintf(debugText,"");

    for( int i=0; i<game.getNplayersTeam1(); i++ )
    {
        char buffer[1024];
        sprintf(buffer,"Blue player %i: %.0f,%.0f\n",i, game.playersTeam1[i].getCurrentPosition().getX(),game.playersTeam1[i].getCurrentPosition().getY());
        strcat(debugText,buffer);
    }

    for( int i=0; i<game.getNplayersTeam1(); i++ )
    {
        char buffer[1024];
        sprintf(buffer,"Yellow player %i: %.0f,%.0f\n",i, game.playersTeam2[i].getCurrentPosition().getX(),game.playersTeam2[i].getCurrentPosition().getY());
        strcat(debugText,buffer);
    }

    if( game.getNplayersTeam1()+game.getNplayersTeam2() )
    {
        char buffer[1024];
        sprintf(buffer,"Ball: %0.f,%0.f",game.ball.getCurrentPosition().getX(),game.ball.getCurrentPosition().getY());
        strcat(debugText,buffer);
    }

    if(isVerbose) cout << debugText;
    fillTextView(debugText);

}



void MainWindow::iterate()
{
        comunicate();

	drawWorld();

        generateTextOutput();
}


void MainWindow::drawWorld()
{
        drawField();

	drawPlayers();

	pathplan.draw();

	game.ball.draw(displaySettings);
}



gboolean exposeEvent(GtkWidget * widget, GdkEvent *event, gpointer data)
{
	//interpreta��o dos par�metros da callback
	MainWindow * mw = (MainWindow*) data;


	GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);


	if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
		return FALSE;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


        mw->iterate();


	if (gdk_gl_drawable_is_double_buffered(gldrawable))
		gdk_gl_drawable_swap_buffers(gldrawable);
	else
		glFlush ();

	gdk_gl_drawable_gl_end(gldrawable);

	return TRUE;
}

//Fun��o para updatear a screen
gboolean renderScene(GtkWidget * widget, GdkEvent *event, gpointer data)
{
	//* Invalidate the whole window. * /
	gdk_window_invalidate_rect(widget->window, &widget->allocation, FALSE);

	//* Update synchronously (fast). * /
	gdk_window_process_updates(widget->window, FALSE);

	return TRUE;
}

void timeoutAdd(GtkWidget *widget, MainWindow* mw)
{
    //FIXME troquei renderScene pela glutDisplayFunc
	if (mw->timeout_handler_id == 0)
		mw->timeout_handler_id = gtk_timeout_add(TIMEOUT_INTERVAL, (GtkFunction) renderScene, widget);
}


void timeoutRemove(GtkWidget *widget, MainWindow* mw)
{
	if (mw->timeout_handler_id != 0)
	{
		gtk_timeout_remove(mw->timeout_handler_id);
		mw->timeout_handler_id = 0;
	}
}

/*
gboolean MainWindow::visibilityNotifyEvent(GtkWidget *widget, GdkEventVisibility *event, gpointer data)
{
    if (event->state == GDK_VISIBILITY_FULLY_OBSCURED)
		timeoutRemove(widget);
    else
		timeoutAdd(widget);

	return TRUE;
}
*/


void MainWindow::createDrawingArea()
{
	gtk_widget_set_size_request(soccer_field, ARENA_WIDTH, ARENA_HEIGHT);
	gtk_widget_set_gl_capability (soccer_field, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE);
	gtk_widget_show(soccer_field);


	gtk_signal_connect (GTK_OBJECT (soccer_field), "button_press_event", (GtkSignalFunc) button_press_event, this); //cliques do mouse
		gtk_widget_set_events (soccer_field, GDK_BUTTON_PRESS_MASK);
		cursorEvent = CURSOR_EVENT_NOTHING;
	g_signal_connect_after(G_OBJECT(soccer_field), "realize", G_CALLBACK(realize), this); //instancia��o da Widget
	g_signal_connect(G_OBJECT(soccer_field), "unrealize", G_CALLBACK(timeoutRemove), this);
	g_signal_connect(G_OBJECT(soccer_field), "configure_event", G_CALLBACK(configureEvent), this);
	g_signal_connect(G_OBJECT(soccer_field), "expose_event", G_CALLBACK(exposeEvent), this); //?
	g_signal_connect(G_OBJECT(soccer_field), "map_event",  G_CALLBACK(timeoutAdd), this);
	g_signal_connect(G_OBJECT(soccer_field), "unmap_event", G_CALLBACK(timeoutRemove), this);
	//g_signal_connect(G_OBJECT(soccer_field), "visibility_notify_event", G_CALLBACK(visibilityNotifyEventWrapper), this);


}
