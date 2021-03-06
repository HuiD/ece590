#ifndef __COLLISIONPAIR_H__
#define __COLLISIONPAIR_H__
#include "Sprite.h"
class Hero;


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
    CollisionPair(Hero * s11, Sprite * s22, enum colType t){
        s1 =(Sprite *) s11;
        s2 = s22;
        collisionType = t;
    }
    ~CollisionPair() {
    }
    
    bool isCollided(){
        if (s1->getVisible() && s2->getVisible())
            return s2->isCollided(s1);
        return false;
    }
    
    void onCollision() {
        s1->inCollision(collisionType);
        s2->inCollision(collisionType);
    }


};

#endif
