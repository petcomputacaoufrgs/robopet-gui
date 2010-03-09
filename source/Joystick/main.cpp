#ifndef WIN32
#include <GL/glut.h>
#define GAMBI_INIT() glutInit(&argc, argv)
#elif
#define GAMBI_INIT() ;;
#include <gl/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "joy.h"


Joystick global_joy;

void SetupRC(void)
{
		if (global_joy.isConfigured())
		{
			global_joy.loadConfig();
		}
		
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void RenderScene(void)
{
     glutSwapBuffers();
}

void JoystickFunc(unsigned int mask, int x, int y, int z)
{
	
		global_joy.receiveInput(mask,x,y,z);
		global_joy.printStatus();
}

int main(int argc, char **argv)
{
    GAMBI_INIT();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("JOY OH JOY");
    glutDisplayFunc(RenderScene);
    glutJoystickFunc(JoystickFunc,100);
    SetupRC();
    glutMainLoop();
		
    return 0;
}

