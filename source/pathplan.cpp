#include<iostream>

#include "pathplan.h"

#include "mainWindow.h"
#include "point.h"

#include "rrt.h"

using namespace std;



Pathplan::Pathplan()
{
	isDrawn = false;
	
}

Pathplan::Pathplan(Point initialpos,int pathplanIndex,int checkPrintFull)
{
	this->initialpos = initialpos;
	this->pathplanIndex = pathplanIndex;
	this->checkPrintFull = checkPrintFull;
}

Pathplan::~Pathplan() {};



void Pathplan::fillEnv_playerBox(int x, int y, int safetyCells)
{	
	const int lado = MM_TO_CELLS( ROBOT_RADIUS_MM ) + safetyCells;


	for(int i=0; i<lado; i++)
        for(int k=0; k<lado; k++)
            env [x+i - lado/2][y+k - lado/2] = OBSTACULO;

}

void Pathplan::fillEnv(vector<GUIPlayer> playersTeam1,vector<GUIPlayer> playersTeam2)
{
	int centerx, centery;

	for(int i=0;i<MAX_X;i++)
		for(int j=0;j<MAX_Y;j++)
			env[i][j] = LIVRE;


        vector<GUIPlayer>::iterator it;
        for(it=playersTeam1.begin(); it<playersTeam1.end(); it++) {

            centerx = (int)MM_TO_CELLS((*it).getCurrentPosition().getX());
            centery = (int)MM_TO_CELLS((*it).getCurrentPosition().getY());
            fillEnv_playerBox(centerx,centery,4);
        }
        
        for(it=playersTeam2.begin(); it<playersTeam2.end(); it++) {

            centerx = (int)MM_TO_CELLS((*it).getCurrentPosition().getX());
            centery = (int)MM_TO_CELLS((*it).getCurrentPosition().getY());
            fillEnv_playerBox(centerx,centery,4);
	}

}






void Pathplan::runRRT()
{
	//pontos iniciais e finais
	state initial = state( MM_TO_CELLS( initialpos.getX() ), MM_TO_CELLS( initialpos.getY() ));
	state goal    = state( MM_TO_CELLS( finalpos.getX() ), MM_TO_CELLS( finalpos.getY() )) ;
	
    RRTTree *solutionTree;
	solutionTree = RRTPlan(env,initial,goal);

    this->pathFull = solutionTree->treeToList();
	this->pathFinal = solutionTree->findSolucao(goal);


	//print(solutionTree,initial,goal,caminhoSolucao,env); //imprime resultado no console
	this->isDrawn=true;
}



void Pathplan::runPathplan()
{
	switch(pathplanIndex){


		case PATHPLAN_RRT:
			runRRT();
			break;


		case PATHPLAN_ASTAR:
			break;
	}
}