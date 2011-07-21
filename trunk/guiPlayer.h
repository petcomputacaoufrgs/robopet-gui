#pragma once


#include "displaySettings.h"
#include "guiConstants.h"

#include "player.h"
using RP::Player;

#include "point.h"
using RP::Point;

#include <vector>
using std::vector;


class GuiPlayer: public Player
{
    public:
        GuiPlayer() { hasUpdatedInfo = false; }
        
        bool hasUpdatedInfo;
        vector<Point> path;
        
		void clearPath() { path.clear(); }
		void addPathPoint(Point p) { path.push_back(p); }

};
