#ifndef _GUIBALL_H
#define	_GUIBALL_H

#include "robopet.h"
#include "movingObject.h"
#include "displaySettings.h"

class GuiBall: public RP::Ball
{
    public:

        GuiBall() { hasUpdatedInfo = false; }

        bool hasUpdatedInfo;

        void draw(DisplaySettings settings);
        
};


#endif	/* _GUIBALL_H */

