
#include <mainWindow.h>
#include "interface.h"

using namespace std;



MainWindow::MainWindow() {}

MainWindow::MainWindow(string name)
{
        cout<<"oie"<<endl;
        configuraGL();

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	soccer_field = gtk_drawing_area_new();


        createMWindow("RoboPet GUI");

        communicationClient = NULL;
        listenToAI();

        for(int i = 0; i < MAX_JOGADORES; ++i)
        {
            playersTeam1[i] = GUIPlayer();
            playersTeam2[i] = GUIPlayer();
        }
	ball = GuiBall();
}

void MainWindow::openCommunication(int port, char* host)
{
    communicationClient = new GUIClient(port,host);
    cout<<"Ready to receive AI Packets..."<<endl;
    communicationClient->open(false);
}

void MainWindow::closeCommunication()
{
    delete communicationClient;
    printf("Communication closed.\n");
}


void MainWindow::listenToAI()
{
    if(communicationClient)
    {
        SSL_WrapperPacket packet;
        if(communicationClient->receive(packet))
        {
            cout<<"opa, recebi um pacote"<<endl;

            if(packet.has_aitogui())
            {
                cout<<"opa, esse pacote eh meu!"<<endl;

				cout << "numero de jogadores azuis: " << packet.aitogui().blue_robots_size() <<endl;
				cout << "numero de jogadores amarelos: " << packet.aitogui().yellow_robots_size() <<endl;

                for(int i = 0; i < MAX_JOGADORES && i < packet.aitogui().blue_robots_size(); ++i) {

                    playersTeam1[i].setCurrentPosition( packet.aitogui().blue_robots(i).current_x() ,
                                                        packet.aitogui().blue_robots(i).current_y() );
                    playersTeam1[i].setCurrentAngle( packet.aitogui().blue_robots(i).current_theta() );
                    //------

                    cout << packet.aitogui().blue_robots(i).current_x() << endl;
                    cout << packet.aitogui().blue_robots(i).current_y() << endl;

                    playersTeam1[i].setFuturePosition( packet.aitogui().blue_robots(i).future_x() ,
                                                       packet.aitogui().blue_robots(i).future_y() );
                    playersTeam1[i].setFutureAngle( packet.aitogui().blue_robots(i).future_theta() );
                    //------

                    playersTeam1[i].setPastPosition( packet.aitogui().blue_robots(i).past_x() ,
                                                     packet.aitogui().blue_robots(i).past_y() );

                    playersTeam1[i].hasUpdatedInfo = true;

                }

                for(int i = 0; i < MAX_JOGADORES && i < packet.aitogui().yellow_robots_size(); ++i) {

                    playersTeam2[i].setCurrentPosition( packet.aitogui().yellow_robots(i).current_x() ,
                                                        packet.aitogui().yellow_robots(i).current_y() );
                    playersTeam2[i].setCurrentAngle( packet.aitogui().yellow_robots(i).current_theta() );
                    //------

                    playersTeam2[i].setFuturePosition( packet.aitogui().yellow_robots(i).future_x() ,
                                                       packet.aitogui().yellow_robots(i).future_y() );
                    playersTeam2[i].setFutureAngle( packet.aitogui().yellow_robots(i).future_theta() );
                    //------

                    playersTeam2[i].setPastPosition( packet.aitogui().yellow_robots(i).past_x() ,
                                                     packet.aitogui().yellow_robots(i).past_y() );

                    playersTeam2[i].hasUpdatedInfo = true;
                }


                ball.setCurrentPosition( packet.aitogui().ball().x(),
                                         packet.aitogui().ball().y()  );

                ball.setFuturePosition(  packet.aitogui().ball().past_x(),
                                         packet.aitogui().ball().past_y()  );

                ball.hasUpdatedInfo = true;
            }

        }
    }
    //else printf("Communication Client NULL!\n");
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



void MainWindow::drawWorld()
{
        drawField();

	drawPlayers();

	pathplan.draw();

	ball.draw(displaySettings);
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



        mw->listenToAI();

	mw->drawWorld();




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
