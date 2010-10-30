#include <stdlib.h>
#include <vector>

#include "game.h"


Game::Game() { nplayers[0]=0; nplayers[1]=0; }

Game::Game(int _nplayers1, int _nplayers2, GtkWidget* _playersComboBox)
{ 
    nplayers[0] = _nplayers1;
    nplayers[1] = _nplayers2;
    playersComboBox = _playersComboBox;
}

Game::~Game() { }



void Game::updateNplayersTeam1(int n)
{
    if( nplayers[0] != n )
    {
        resetPlayersComboBox();
        
        nplayers[0] = n;
        players[0].resize(n);

        updatePlayersComboBox();
    }
}

void Game::updateNplayersTeam2(int n)
{
    if( nplayers[1] != n )
    {
        resetPlayersComboBox();

        nplayers[1] = n;
        players[1].resize(n);

        updatePlayersComboBox();
    }
}

void Game::addPlayerTeam1()
{
    resetPlayersComboBox();

    nplayers[0]++;

    players[0].resize(nplayers[0]);

    updatePlayersComboBox();
}


void Game::addPlayerTeam2()
{
    resetPlayersComboBox();

    nplayers[1]++;

    players[1].resize(nplayers[1]);

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
    	vector<guiPlayer>::iterator it;
        char buffer[32];

        for(int i=0; i < nplayers[0]; i++) {

            sprintf (buffer, "Yellow player %i", i);

            gtk_combo_box_insert_text( GTK_COMBO_BOX(playersComboBox), i, buffer);
        }

        for(int i=0; i < nplayers[1]; i++) {

            sprintf (buffer, "Blue player %i", i);

            gtk_combo_box_insert_text( GTK_COMBO_BOX(playersComboBox), nplayers[0]+i, buffer);
        }

        gtk_combo_box_set_active( GTK_COMBO_BOX(playersComboBox), 0);
}



int Game::getNplayersTeam1() { return nplayers[0]; }

int Game::getNplayersTeam2() { return nplayers[1]; }
