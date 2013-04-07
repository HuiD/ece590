#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__
#include "Sprite.h"


class Explosion : public Sprite{
private:
    int interval;
    int oriTime;
public:
    Explosion(const char * file, int x, int y, int intev, int cur_time){
        Sprite::setVisible(true);
        Sprite::initSprite(file);
        Sprite::setCoords(x, y);
        interval = intev;
        oriTime = cur_time;
    }
    ~Explosion() {
//        delete sprite;
    }
    
    void update() {
        if (visible && checkTimer()){
//            cout<<visible<<endl;
            setVisible(false);
        }
    }
    
    bool checkTimer() {
        int eclipsedTime = SDL_GetTicks() - oriTime;
        if (eclipsedTime > interval)
            return true;
        return false;
    }
    
    void inCollision(enum colType t){
        switch (t) {
            case HeroExplosion:
//                cout<<"hero on fire"<<endl;
                cout<<"test for level 3"<<endl;
                break;
            case BombExplosion:
                cout<<"bomb on bomb"<<endl;
                break;
            case EnemyExplosion:
                cout<<"enemy explode!"<<endl;
                break;
            case BlockExplosion:
                cout<<"block bombed!"<<endl;
                break;
            default:
                break;
        }
        
    }

    
};
#endif
