#include <stdlib.h>
#include <math.h>
#include <guiConstants.h>
#include <gtk/gtk.h>

#include "guiPlayer.h"
#include "displaySettings.h"

//void drawBox(float centerX, float centerY, float side);
//void drawQuarterCircle(float centerX, float centerY, float radius, int quadrante);
//void drawCircle(float centerX, float centerY, float radius);

void drawBox(cairo_t *cr, float centerX, float centerY, float side);
void drawLinedPath(cairo_t *cr, vector<Point> path);
