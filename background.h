#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include "Sprite.h"

#define NUM_HERO_FILES 8

class Background : public Sprite{
public:
    Background(const char * file){
        Sprite::initSprite(file);
    }
    ~Background() {
//        delete sprite;
    }

    
    void onCollison(){
        cout<<"Background collide!"<<endl;
    }
    
    
};
//extern Background * block;
#endif
