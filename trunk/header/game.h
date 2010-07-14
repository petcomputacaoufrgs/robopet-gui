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
        Game(int _nplayersTeam1, int _nplayersTeam2, GtkWidget* _playersComboBox);
        ~Game();

        GtkWidget* playersComboBox;

        void updateNplayersTeam1(int n);
        void updateNplayersTeam2(int n);

        void addPlayerTeam1();
        void addPlayerTeam2();

        int getNplayersTeam1();
        int getNplayersTeam2();


        GuiBall ball;
        vector<guiPlayer> playersTeam1;
        vector<guiPlayer> playersTeam2;



    private:

        int nplayersTeam1, nplayersTeam2;

        void updatePlayersComboBox();
        void resetPlayersComboBox();


};

#endif	/* _GAME_H */

