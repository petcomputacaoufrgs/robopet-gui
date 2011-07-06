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
        void draw(cairo_t *cr, DisplaySettings settings);

    private:
		vector<Point> path;
		
        void drawBody(cairo_t *cr, float centerX, float centerY);
        void drawData(cairo_t *cr, float centerX, float centerY);
        void drawVector(cairo_t *cr, float centerX, float centerY, float vecX, float vecY);
        void drawPath(cairo_t *cr, vector<Point> path);

};
