#pragma once


#include "displaySettings.h"
#include "guiConstants.h"

#include "player.h"
using RP::Player;

#include "point.h"
using RP::Point;

#include <vector>
using std::vector;


class guiPlayer: public Player
{
    public:
        guiPlayer() { hasUpdatedInfo = false; }
        
        bool hasUpdatedInfo;
        
		void clearPath() { path.clear(); }
		void addPathPoint(Point p) { path.push_back(p); }
        void draw(cairo_t *cr, int index, DisplaySettings settings);

    private:
		vector<Point> path;
		
        void drawBody(cairo_t *cr, float centerX, float centerY);
        void drawAngle(cairo_t *cr, float centerX, float centerY, float angle);
        void drawIndex(cairo_t *cr, float centerX, float centerY, int robotNumber);
        void drawVector(cairo_t *cr, float centerX, float centerY, float vecX, float vecY);

};
