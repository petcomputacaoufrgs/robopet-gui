#include <mainWindow.h>
#include "interface.h"

using namespace std;


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
    static char text[1024];
    sprintf(text,""); //resets text buffer

    sprintf(text+strlen(text),"Ball: %0.f,%0.f",game.ball.getCurrentPosition().getX(),game.ball.getCurrentPosition().getY());

    sprintf(text+strlen(text),"\n%i Blue Players:",game.getNplayersTeam1());
    for( int i=0; i<game.getNplayersTeam1(); i++ ) {
        sprintf(text+strlen(text),"\n- %i: %.0f,%.0f",i, game.playersTeam1[i].getCurrentPosition().getX(),game.playersTeam1[i].getCurrentPosition().getY());
    }

    sprintf(text+strlen(text),"\n%i Yellow Players:",game.getNplayersTeam2());
    for( int i=0; i<game.getNplayersTeam2(); i++ ) {
        sprintf(text+strlen(text),"\n- %i: %.0f,%.0f",i, game.playersTeam2[i].getCurrentPosition().getX(),game.playersTeam2[i].getCurrentPosition().getY());
    }

    
    fillTextView(text);

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


void MainWindow::createDrawingArea()
{
	gtk_widget_set_size_request(soccer_field, ARENA_WIDTH, ARENA_HEIGHT);
	gtk_widget_set_gl_capability (soccer_field, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE);
	gtk_widget_show(soccer_field);

	gtk_signal_connect (GTK_OBJECT (soccer_field), "button_press_event", (GtkSignalFunc) button_press_event, this); //cliques do mouse
		gtk_widget_set_events (soccer_field, GDK_BUTTON_PRESS_MASK);
		cursorEvent = CURSOR_EVENT_NOTHING;
	g_signal_connect_after(G_OBJECT(soccer_field), "realize", G_CALLBACK(realize), this); //instancia��o da Widget
	g_signal_connect(G_OBJECT(soccer_field), "configure_event", G_CALLBACK(configureEvent), this);
	g_signal_connect(G_OBJECT(soccer_field), "expose_event", G_CALLBACK(exposeEvent), this); //?


}
