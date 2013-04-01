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
        cout<<"yes"<<endl;
//        delete s1;
//        delete s2;
    }
    
    bool isCollided(){
        if (s1->getVisible() && s2->getVisible())
            return s2->isCollided(s1);
        return false;
    }
    
    void onCollision() {
//        switch (collisionType) {
//            case HeroBomb:
//                s2->setVisible(false);
//                cout<<"collided!"<<endl;
//                break;
//            case HeroBlock:
//                break;
//            case HeroExplosion:
////                s1->inExplosion();
//                cout<<"hero dies!"<<endl;
//                break;
//            default:
//                break;
//        }
        s1->inCollision(collisionType);
        s2->inCollision(collisionType);
    }


};

#endif
