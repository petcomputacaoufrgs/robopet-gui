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
        ~Game();

        GtkWidget* playersComboBox;

        void updateNplayers(int team, int n);

        void addPlayer(int team);

        int getNplayers(int team);

        GuiBall ball;
        vector<guiPlayer> players[2];

    private:

        int nplayers[2];

        void updatePlayersComboBox();
        void resetPlayersComboBox();

};

#endif	/* _GAME_H */

