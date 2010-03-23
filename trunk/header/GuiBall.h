#ifndef _GUIBALL_H
#define	_GUIBALL_H

#include "ball.h"

#include "displaySettings.h"



class GuiBall: public Ball
{
    public:

        GuiBall() { hasUpdatedInfo = false; }

        bool hasUpdatedInfo;

        void draw(DisplaySettings settings);
        
  
};


#endif	/* _GUIBALL_H */

