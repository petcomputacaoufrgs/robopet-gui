#include<iostream>

#include "pathplan.h"
#include "guiPlayer.h"
#include "mainWindow.h"
#include "point.h"

//#include "rrt.h"

using namespace std;



guiPathplan::guiPathplan()
{
	isDrawn = false;
	
}

guiPathplan::guiPathplan(Point initialpos,int pathplanIndex,int checkPrintFull)
{
	this->initialpos = initialpos;
	this->pathplanIndex = pathplanIndex;
	this->checkPrintFull = checkPrintFull;
}

guiPathplan::~guiPathplan() {};

void guiPathplan::fillEnv(vector<guiPlayer> players1, vector<guiPlayer> players2)
{
        vector<Point> positions;
        
        for(int i=0; i<players1.size(); i++)
            positions.push_back(players1[i].getCurrentPosition());
        for(int i=0; i<players2.size(); i++)
            positions.push_back(players2[i].getCurrentPosition());
    
    
}
