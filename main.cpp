#include "gtk/gtk.h"

#include <stdlib.h>
#include "mainWindow.h"

int main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);

	static MainWindow window = MainWindow("GUI");

	printf("GUI Rodando!\n");
        
	gtk_main();

	return 0;
}
