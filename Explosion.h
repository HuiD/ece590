#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__
#include "Sprite.h"

#define NUM_EXPLO_FILES 9

class Explosion : public Sprite{
private:
    int interval;
    int oriTime;
    int frame;
    SDL_Rect showPart;

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

    }
    SDL_Rect getShowPart(){
        return showPart;
    }
    void setShowPart(int x, int y, int w, int h) {
        showPart.x=x;
        showPart.y=y;
        showPart.w=w;
        showPart.h=h;
    }
    void update() {
        frame++;
        if (frame > 8)
            frame = 0;
        setAnimFrame(frame);
        setW(showPart.w);
        setH(showPart.h);
        if (visible && checkTimer()){

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
                break;
            case BombExplosion:
                break;
            case EnemyExplosion:
                break;
            case BlockExplosion:
                break;
            default:
                break;
        } 
    }  
};
#endif
