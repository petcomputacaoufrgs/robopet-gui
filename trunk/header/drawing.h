#include <stdlib.h>
#include <math.h>
#include <guiConstants.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#include "guiPlayer.h"
#include "displaySettings.h"


void    drawQuarterCircle(float centerX, float centerY, float radius, int quadrante);
void    drawCircle(float centerX, float centerY, float radius);

void    drawRobot(float centerX, float centerY, float angle, int robotNumber);
void    drawBall(float centerX, float centerY);
void    drawField();

void    drawPositionMarker(int x, int y);