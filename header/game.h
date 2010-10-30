#ifndef _GAME_H
#define	_GAME_H

#include <vector>

#include "guiBall.h"
#include "guiPlayer.h"



class Game
{
    #define MAX_JOGADORES 5 //TEMPORÁRIO

    public:
        Game();
        Game(int _nplayers1, int _nplayers2, GtkWidget* _playersComboBox);
        ~Game();

        GtkWidget* playersComboBox;

        void updateNplayersTeam1(int n);
        void updateNplayersTeam2(int n);

        void addPlayerTeam1();
        void addPlayerTeam2();

        int getNplayersTeam1();
        int getNplayersTeam2();


        GuiBall ball;
        vector<guiPlayer> players[2];



    private:

        int nplayers[2];

        void updatePlayersComboBox();
        void resetPlayersComboBox();


};

#endif	/* _GAME_H */

