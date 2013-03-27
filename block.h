#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "Sprite.h"


class Block : public Sprite{
    
public:
    Block(){
        Sprite::setVisible(true);
        Sprite::initSprite("img/blob3.bmp");
//        Sprite::setCoords(200,450);
    }
    ~Block() {
        //        delete sprite;
    }
    
    void update() {
        
    }
    void draw(SDL_Surface * screen) {
        if (Sprite::getVisible())
            Sprite::blit(screen);
    }
    
//    void onHBC(){
//
//    }
    
    
};
//extern Bomb * bomb;
#endif
