#ifndef _GAME_H
#define	_GAME_H

#include <vector>

#include "GuiBall.h"
#include "GUIPlayer.h"



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

        int getNplayersTeam1();
        int getNplayersTeam2();


        GuiBall ball;
        vector<GUIPlayer> playersTeam1;
        vector<GUIPlayer> playersTeam2;



    private:

        int nplayersTeam1, nplayersTeam2;

        void updatePlayersComboBox();
        void resetPlayersComboBox();


};

#endif	/* _GAME_H */

