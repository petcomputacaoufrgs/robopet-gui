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
