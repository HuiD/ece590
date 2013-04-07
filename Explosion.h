#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__
#include "Sprite.h"

#define NUM_EXPLO_FILES 9

class Explosion : public Sprite{
private:
    int interval;
    int oriTime;
    int frame;
public:
    Explosion(const char ** file, int x, int y, int intev, int cur_time){
        Sprite::setVisible(true);
        Sprite::initSprite(NUM_EXPLO_FILES, file);
        Sprite::setCoords(x, y);
        interval = intev;
        oriTime = cur_time;
	frame = 0;
	setTransparent();
    }
    ~Explosion() {
//        delete sprite;
    }
    
    void update() {
	frame ++;
	if (frame > 8)
	    frame = 0;
	setAnimFrame(frame);
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
