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
        void deletePlayer( int team, int player );

        int getNplayers(int team);

        GuiBall ball;
        vector<GuiPlayer> players[2];
        GuiPlayer joyPlayer;

    private:
		int lastAddedTeam;
        int nplayers[2];

        void updatePlayersComboBox();
        void resetPlayersComboBox();

};

#endif	/* _GAME_H */

