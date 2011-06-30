#ifndef _GAME_H
#define	_GAME_H

#include <vector>

#include "guiBall.h"
#include "guiPlayer.h"


class Game
{
	public:
        Game();
        ~Game();

        double*		scaleFactorLength;
		double*		scaleFactorWidth;
        GtkWidget*	playersComboBox;


        void updateNplayers(int team, int n);

        void addPlayer(int team, Point pos=Point(0,0));

        int getNplayers(int team);

        GuiBall ball;
        vector<guiPlayer> players[2];
        guiPlayer joyPlayer;

    private:
		int lastAddedTeam;
        int nplayers[2];

        void updatePlayersComboBox();
        void resetPlayersComboBox();

};

#endif	/* _GAME_H */
