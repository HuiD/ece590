#ifndef __UPGRADE_H__
#define __UPGRADE_H__
#include "Sprite.h"
#include "CollisionPair.h"
#include <algorithm>
class Block;
class Bomb;
class Hero;
class Explosion;
class Enemy;

//enum upgradeType {
//    level1, level2, addLife
//};

class Upgrade : public Sprite{
   
#define LIFETIME  6000
    
private:
    enum colType upType;
    int oriTime;
public:
    Upgrade(int x, int y, enum colType t){
        Sprite::setVisible(true);
        upType = t;
        switch (upType) {
            case HeroUpgrade:
                Sprite::initSprite("img/items/FireUpgrade.bmp");
                break;
            case HeroLife:
                Sprite::initSprite("img/items/LifeUpgrade.bmp");
                break;
//            case addLife:
//                Sprite::initSprite("img/blob3.bmp");
//                break;
            default:
                break;
        }
        Sprite::setCoords(x,y);
        oriTime = SDL_GetTicks();
    }
    ~Upgrade() {
        //        delete sprite;
    }
    
    void update() {
        if (!visible)
            return;
        if (SDL_GetTicks() - oriTime > LIFETIME)
            setVisible(false);
    }

    void inCollision(enum colType t){
        switch (t) {
            case HeroUpgrade:
//                cout<<"hero upgraded!"<<endl;
                setVisible(false);
                break;
            case HeroLife:
                //                cout<<"hero upgraded!"<<endl;
                setVisible(false);
                break;
            default:
                break;
        }
        
    }
    
    colType getUpType() {
        return upType;
    }
    
    
};
#endif
