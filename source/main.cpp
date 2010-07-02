
#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include "gtk/gtk.h"
#include "gtk/gtkgl.h"


#include <stdlib.h>
#include "mainWindow.h"


int main (int argc, char *argv[])
{
        gtk_init (&argc, &argv);
        glutInit(&argc, argv);
	static MainWindow window = MainWindow("GUI");

        printf("GUI Rodando!\n");
        
	gtk_main();

	return 0;
}