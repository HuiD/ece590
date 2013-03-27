#ifndef __HERO_H__
#define __HERO_H__
#include "Sprite.h"
#include <iostream>
#include <utility>
#include <vector>
#define NUM_HERO_FILES 8
using namespace std;
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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 640


enum move_state {
    MOVE_LEFT,
    MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_UP,
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
        walk_accel = 8.00;
        onground = true;
    }
    ~Hero() {
        //        delete sprite;
    }
    
#define MAX_SPEED  10.5
    void update(vector<SDL_Rect> blockMap) {
        frame = frame++;
        frame = frame & 3;
        //speedY = 0;
        switch(move) {
            case DONT_MOVE:
                speedX=0;
                speedY=0;
                break;
            case MOVE_RIGHT:
                if(speedX < MAX_SPEED) speedX = walk_accel;
                speedY=0;
                break;
            case MOVE_LEFT:
                if(speedX > -MAX_SPEED) speedX = -1*walk_accel;
                speedY=0;
                break;
            case MOVE_UP:
                if(speedY > -MAX_SPEED) speedY = -1*walk_accel;
                speedX=0;
                break;
            case MOVE_DOWN:
                if(speedY < MAX_SPEED) 	speedY = walk_accel;
                speedX=0;
                break;
        }
        int oriX = getX();
        int oriY = getY();
        setCoords(getX()+speedX, getY()+speedY);
        for (int i = 0; i < blockMap.size(); i++){
            SDL_Rect pos = blockMap.at(i);
            if (getX()>=pos.x && getX()<=pos.x+pos.w && getY()>=pos.y && getY()<=pos.y+pos.h){
                setCoords(oriX, oriY);
                break;
            }
        }
        //limit inside boundary
        if (Sprite::getY() > WINDOW_HEIGHT-getH()) {
            Sprite::setCoords(Sprite::getX(), WINDOW_HEIGHT-getH());
        }
        if (Sprite::getY() < 0) {
            Sprite::setCoords(Sprite::getX(), 0);
        }
        if (Sprite::getX() < 0) {
            Sprite::setCoords(0, getY());
        }
        if (Sprite::getX() > WINDOW_WIDTH-getW()) {
            Sprite::setCoords(WINDOW_WIDTH-getW(), getY());
        }
        Sprite::setAnimFrame(frame);
    }

    
    void jump() {
        if (onground) {
            onground = false;
            speedY = -40;
        }
        
    }

    
    void stopMoving() {
        move = DONT_MOVE;
    }
    void moveLeft() {
        move = MOVE_LEFT;
    }
    void moveRight() {
        move = MOVE_RIGHT;
    }
	void moveDown()
	{
		move = MOVE_DOWN;
	}
	void moveUp()
	{
		move = MOVE_UP;
	}
    

    
};
extern Hero * hero;
#endif
