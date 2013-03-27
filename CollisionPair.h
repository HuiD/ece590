#ifndef __COLLISIONPAIR_H__
#define __COLLISIONPAIR_H__
#include "Sprite.h"

class CollisionPair {
    
protected:
    Sprite * s1;
    Sprite * s2;
public:
    int collisionType;
    CollisionPair(Sprite * s11, Sprite * s22, int t){
        s1 = s11;
        s2 = s22;
        collisionType = t;
    }
    
    bool isCollided(){
        return s1->isCollided(s2);
    }
    
    void onCollision() {
        switch (collisionType) {
            case 0:
                s2->setVisible(false);
                break;
                
            default:
                break;
        }
    }
};

#endif
