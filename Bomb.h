#ifndef __BOMB_H__
#define __BOMB_H__
#include "Sprite.h"


class Bomb : public Sprite{

public:
    Bomb(const char * file, int x, int y){
        Sprite::setVisible(true);
        Sprite::initSprite(file);
        Sprite::setCoords(x, y);
    }
    ~Bomb() {
//        delete sprite;
    }
    
#define MAX_SPEED  10.5
    void update() {
        
    }
    
    
};
//extern Bomb * bomb;
#endif
