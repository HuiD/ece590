#ifndef __BOMB_H__
#define __BOMB_H__
#include "Sprite.h"


class Bomb : public Sprite{

public:
    Bomb(){
        Sprite::setVisible(true);
        Sprite::initSprite("img/blob2.bmp");
        Sprite::setCoords(200,450);
    }
    ~Bomb() {
//        delete sprite;
    }
    
#define MAX_SPEED  10.5
    void update() {
        
    }
    void draw(SDL_Surface * screen) {        
        Sprite::blit(screen);
    }
    
    
//    void onHBC(){
//        cout<<"bomb collide!"<<endl;
//        setVisible(false);
//    }
    
    
};
//extern Bomb * bomb;
#endif
