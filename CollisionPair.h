#ifndef __COLLISIONPAIR_H__
#define __COLLISIONPAIR_H__
#include "Sprite.h"

enum colType{
    HeroBomb, HeroBlock
};

class CollisionPair {
    
protected:
    Sprite * s1;
    Sprite * s2;
public:
    enum colType collisionType;
    CollisionPair(Sprite * s11, Sprite * s22, enum colType t){
        s1 = s11;
        s2 = s22;
        collisionType = t;
    }
    
    bool isCollided(){
        return s1->isCollided(s2);
    }
    
    void onCollision() {
        switch (collisionType) {
            case HeroBomb:
                s2->setVisible(false);
                break;
            case HeroBlock:
                
                break;
            default:
                break;
        }
    }


};

#endif
