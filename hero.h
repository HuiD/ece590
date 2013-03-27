#ifndef __HERO_H__
#define __HERO_H__
#include "Sprite.h"
#define NUM_HERO_FILES 8
static const char * hero_file_names[NUM_HERO_FILES + 1] = {
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



enum move_state {
    MOVE_LEFT,
    MOVE_RIGHT,
    DONT_MOVE
};


class Hero : public Sprite{
private:
    enum move_state move;
    int frame;
    double speedX;
    double speedY;
    double walk_accel;
    double fall_accel;
    double ground_decel;
    double air_decel;
    bool onground;
public:
    Hero(){
        Sprite::setVisible(true);
        move = DONT_MOVE;
        Sprite::initSprite(NUM_HERO_FILES, hero_file_names);
        Sprite::setCoords(500,450);
        frame = 0;
        speedX = 0.0;
        speedY = 0.0;
        walk_accel = 0.35;
        ground_decel = 1.0;
        air_decel = 0.07;
        fall_accel = 1.7;
        onground = true;
    }
    ~Hero() {
//        delete sprite;
    }
    
#define MAX_SPEED  10.5
    void update() {
        double absSpeed;
        int signumSpeed;
        if (speedX >= 0){
            signumSpeed = 0;
            absSpeed = speedX;
        }
        else {
            signumSpeed = 1;
            absSpeed = -speedX;
        }
        frame = frame++;
        frame = frame & 3;
        if (onground) {
            speedY = 0;
            switch(move) {
                case DONT_MOVE:
                    absSpeed -= ground_decel;
                    if (absSpeed < 0)
                        absSpeed = 0;
                    if (signumSpeed)
                        speedX = -absSpeed;
                    else
                        speedX = absSpeed;
                    break;
                case MOVE_RIGHT:
                    if(speedX < MAX_SPEED) speedX += walk_accel;
                    break;
                case MOVE_LEFT:
                    if(speedX > -MAX_SPEED) speedX -= walk_accel;
                    break;
            }
        }
        else {
            frame +=4;
            absSpeed -= air_decel;
            if (absSpeed < 0)
                absSpeed = 0;
            if(signumSpeed)
                speedX = -absSpeed;
            else
                speedX = absSpeed;
            speedY += fall_accel;
        }
  
        Sprite::setCoords(Sprite::getX()+speedX, Sprite::getY()+speedY);
        if (Sprite::getY() > 450) {
            Sprite::setCoords(Sprite::getX(), 450);
            onground = true;
        }
        Sprite::setAnimFrame(frame);
    }
    void draw(SDL_Surface * screen) {
        Sprite::blit(screen);
    }

    void jump() {
        if (onground) {
            onground = false;
            speedY = -40;
        }
        
    }
    
//    void scrollingBy(int ammount) {
//        x = x - ammount;
//    }
    
    void stopMoving() {
        move = DONT_MOVE;
    }
    void moveLeft() {
        move = MOVE_LEFT;
    }
    void moveRight() {
        move = MOVE_RIGHT;
    }
    
    void onCollison(){
        cout<<"hero collide!"<<endl;
    }
    
};
extern Hero * hero;
#endif
