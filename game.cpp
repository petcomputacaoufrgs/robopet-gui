#include <stdlib.h>
#include <vector>

#include "game.h"
#include "guiConstants.h"


Game::Game() { nplayers[0]=0; nplayers[1]=0; lastAddedTeam=0; }

Game::~Game() { }



void Game::updateNplayers(int team, int n)
{
    if( nplayers[team] != n )
    {
        resetPlayersComboBox();
        
        nplayers[team] = n;
        players[team].resize(n);

        updatePlayersComboBox();
    }
}

void Game::addPlayer( int team, Point pos )
{
    resetPlayersComboBox();

    nplayers[team]++;

    players[team].resize(nplayers[team]);
    players[team][nplayers[team]-1].setCurrentPosition(pos);
    players[team][nplayers[team]-1].setId( nplayers[team]-1 );

    lastAddedTeam = team;
    
    updatePlayersComboBox();
}

void Game::deletePlayer( int team, int player )
{
    resetPlayersComboBox();

    nplayers[team]--;

    players[team].erase(players[team].begin()+player);
    players[team].resize(nplayers[team]);
    
    lastAddedTeam = team;
    
    updatePlayersComboBox();
}

void Game::resetPlayersComboBox()
//Remove all entries (to use it before updating numbers of players in the teams)
{
    for(int i=0; i<nplayers[0]+nplayers[1]; i++)
        gtk_combo_box_remove_text(GTK_COMBO_BOX(playersComboBox), 0);

}


void Game::updatePlayersComboBox()
//Fills the PlayersComboBox with the right number of players of each team
{
    	vector<GuiPlayer>::iterator it;
        char buffer[32];

        for(int i=0; i < nplayers[TEAM_BLUE]; i++) {

            sprintf (buffer, "Blue player %i", i);

            gtk_combo_box_insert_text( GTK_COMBO_BOX(playersComboBox), i, buffer);
        }

        for(int i=0; i < nplayers[TEAM_YELLOW]; i++) {

            sprintf (buffer, "Yellow player %i", i);

            gtk_combo_box_insert_text( GTK_COMBO_BOX(playersComboBox), nplayers[0]+i, buffer);
        }

        if(lastAddedTeam==0)
			gtk_combo_box_set_active( GTK_COMBO_BOX(playersComboBox), nplayers[0]-1);
		else
			gtk_combo_box_set_active( GTK_COMBO_BOX(playersComboBox), nplayers[0]+nplayers[1]-1);
}



int Game::getNplayers(int team) { return nplayers[team]; }
