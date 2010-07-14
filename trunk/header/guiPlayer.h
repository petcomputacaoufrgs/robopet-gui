#ifndef _GUIPLAYER_H
#define	_GUIPLAYER_H

#include "displaySettings.h"

#include "player.h"
using RP::Player;


class guiPlayer: public Player
{
    public:

        guiPlayer() { hasUpdatedInfo = false; }
        
        bool hasUpdatedInfo;
        
        void draw(int index, DisplaySettings settings);


    private:

        void drawBody(float centerX, float centerY);
        void drawAngle(float centerX, float centerY, float angle);
        void drawIndex(float centerX, float centerY, int robotNumber);
        void drawVector(float centerX, float centerY, float vecX, float vecY);

};



#endif	/* _GUIPLAYER_H */

