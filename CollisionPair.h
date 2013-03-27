#ifndef __COLLISIONPAIR_H__
#define __COLLISIONPAIR_H__
#include "Sprite.h"

class CollisionPair {
    
private:
    Sprite * s1;
    Sprite * s2;
public:
    CollisionPair(Sprite * s11, Sprite * s22){
        s1 = s11;
        s2 = s22;
    }
    
    bool isCollided(){
        return s1->isCollided(s2);
    }
    
    void onCollision(){
        s1->onCollison();
        s2->onCollison();
    }
};

#endif
