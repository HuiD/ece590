#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "Sprite.h"

#define NUM_HERO_FILES 8
static const char * block_file_names[NUM_HERO_FILES + 1] = {
    "img/blob1.bmp",
    "img/blob2.bmp",
    "img/blob3.bmp",
    "img/blob4.bmp",
    "img/blob5.bmp",
    "img/blob6.bmp",
    "img/blob7.bmp",
    "img/blob8.bmp",
    NULL
};

class Block : public Sprite{
private:
//    Sprite *sprite;
    int x;
    int y;
    int frame;
    double speedX;
    double speedY;
    double walk_accel;
    double fall_accel;
    double ground_decel;
    double air_decel;
    bool onground;
public:
    Block(){
        
//        sprite = new Sprite(NUM_HERO_FILES,
//                            block_file_names);
        Sprite::initSprite("img/blob2.bmp");
        x= 200;
        y = 450;
        frame = 0;
        speedX = 0.0;
        speedY = 0.0;
        walk_accel = 0.35;
        ground_decel = 1.0;
        air_decel = 0.07;
        fall_accel = 1.7;
        onground = true;
    }
    ~Block() {
//        delete sprite;
    }
    
#define MAX_SPEED  10.5
    void update() {
        
    }
    void draw(SDL_Surface * screen) {
        
        Sprite::setCoords(x,y);
        Sprite::blit(screen);
    }
    
    int getX() const {
        return x;
    }
    
    void onCollison(){
        cout<<"block collide!"<<endl;
    }
    
    
};
extern Block * block;
#endif
