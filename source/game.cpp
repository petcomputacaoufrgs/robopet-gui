#include <stdlib.h>
#include <vector>

#include "game.h"


Game::Game() { nplayersTeam1=0; nplayersTeam2=0; }

Game::Game(int _nplayersTeam1, int _nplayersTeam2, GtkWidget* _playersComboBox)
{ 
    nplayersTeam1 = _nplayersTeam1;
    nplayersTeam2 = _nplayersTeam1;
    playersComboBox = _playersComboBox;
}

Game::~Game() { }



void Game::updateNplayersTeam1(int n)
{
    if( nplayersTeam1 != n )
    {
        resetPlayersComboBox();
        
        nplayersTeam1 = n;
        playersTeam1.resize(n);

        updatePlayersComboBox();
    }
}

void Game::updateNplayersTeam2(int n)
{
    if( nplayersTeam2 != n )
    {
        resetPlayersComboBox();

        nplayersTeam2 = n;
        playersTeam2.resize(n);

        updatePlayersComboBox();
    }
}


void Game::resetPlayersComboBox()
//Remove all entries (to use it before updating numbers of players in the teams)
{
    for(int i=0; i<nplayersTeam1+nplayersTeam2; i++)
        gtk_combo_box_remove_text(GTK_COMBO_BOX(playersComboBox), 0);

}


void Game::updatePlayersComboBox()
//Fills the PlayersComboBox with the right number of players of each team
{
    	vector<GUIPlayer>::iterator it;
        char buffer[32];

        for(int i=0; i < nplayersTeam1; i++) {

            sprintf (buffer, "Yellow player %i", i);

            gtk_combo_box_insert_text( GTK_COMBO_BOX(playersComboBox), i, buffer);
        }

        for(int i=0; i < nplayersTeam2; i++) {

            sprintf (buffer, "Blue player %i", i);

            gtk_combo_box_insert_text( GTK_COMBO_BOX(playersComboBox), nplayersTeam1+i, buffer);
        }

        gtk_combo_box_set_active( GTK_COMBO_BOX(playersComboBox), 0);
}



int Game::getNplayersTeam1() { return nplayersTeam1; }

int Game::getNplayersTeam2() { return nplayersTeam2; }
