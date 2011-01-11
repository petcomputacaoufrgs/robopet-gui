#ifndef _GUIPLAYER_H
#define	_GUIPLAYER_H

#include "displaySettings.h"
#include "guiConstants.h"

#include "player.h"
using RP::Player;


class guiPlayer: public Player
{
    public:

        guiPlayer() { hasUpdatedInfo = false; }
        
        bool hasUpdatedInfo;
        
        void draw(cairo_t *cr, int index, DisplaySettings settings);

    private:

        void drawBody(cairo_t *cr, float centerX, float centerY);
        void drawAngle(cairo_t *cr, float centerX, float centerY, float angle);
        void drawIndex(cairo_t *cr, float centerX, float centerY, int robotNumber);
        void drawVector(cairo_t *cr, float centerX, float centerY, float vecX, float vecY);

};



#endif	/* _GUIPLAYER_H */

