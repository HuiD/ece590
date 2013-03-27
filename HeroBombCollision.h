#ifndef __HBCOL_H__
#define __HBCOL_H__
#include "Sprite.h"

class HeroBombCollision : public CollisionPair {
        
    void onCollision(){
        s1->onHBC();
        s2->onHBC();
    }
};

#endif
