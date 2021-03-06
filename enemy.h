#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "Sprite.h"
#include <iostream>
#include <utility>
#include <vector>
#include "CollisionPair.h"
class Bomb;
#include <stdlib.h>    


#define NUM_ENEMY_FILES 6
using namespace std;
static const char * enemy_file_names[NUM_ENEMY_FILES + 1] = {
    "img/enemy/enemy_1.bmp",
    "img/enemy/enemy_2.bmp",
    "img/enemy/enemy_3.bmp",
    "img/enemy/enemy_4.bmp",
    "img/enemy/enemy_5.bmp",
    "img/enemy/enemy_6.bmp",
    NULL
};



class Enemy : public Sprite{
private:
    int frame;
    double speedX;
    double speedY;
    double walk_accel;
    double fall_accel;
    int direction;
    int ori_time;
public:
    Enemy(int x, int y){
        Sprite::setVisible(true);
        Sprite::initSprite(NUM_ENEMY_FILES, enemy_file_names);
        Sprite::setCoords(x, y);
        frame = 0;
        speedX = 0.0;
        speedY = 0.0;
        walk_accel = 8.00;
        ori_time = SDL_GetTicks();
        direction = rand()%5;
    }
    ~Enemy() {
        
    }
    
    
#define SPEED  5
    void update(vector<Block * > blocks, vector<CollisionPair * > &colList, map<int, Hero* > &heroGroup, vector<Bomb *> &bombGroup, vector<Explosion *> &explosionGroup) {
        if (!visible) {
            return;
        }
        frame = frame++;

        if (frame > 5) {
            frame = 0;
        }

        if (SDL_GetTicks() - ori_time > 500) {
            ori_time = SDL_GetTicks();
            direction = rand()%5;
        }

        switch(direction) {
            case 0:
                speedX=0;
                speedY=0;
                break;
            case 1:
                if(speedY > -SPEED) speedY = -1*walk_accel;
                speedX=0;
                break;
            case 2:
                if(speedY < SPEED) 	speedY = walk_accel;
                speedX=0;
                break;
            case 3:
                if(speedX > -SPEED) speedX = -1*walk_accel;
                speedY=0;
                break;
            case 4:
                if(speedX < SPEED) speedX = walk_accel;
                speedY=0;
                break;
            default:
                break;
                
        }
        int oriX = getX();
        int oriY = getY();
		int posX = getX()+speedX;
		int posY = getY()+speedY;
		if(posX<0) posX=0;
		if(posY<0) posY=0;
        setCoords(posX, posY);
		
        for (int i = 0; i < blocks.size(); i++){
            Block * tmp = blocks.at(i);
            if (getX()>tmp->getX()-getW()+5 && getX()<tmp->getX()+tmp->getW()-5 && getY()>tmp->getY()-getH()+5 && getY()<tmp->getY()+tmp->getH()-5){
                setCoords(oriX, oriY);
                break;
            }
        }

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
    
    void inCollision(enum colType t){
        switch (t) {
            case HeroEnemy:
                break;
            case EnemyExplosion:
                setVisible(false);
                break;
            case BombEnemy:
                setVisible(false);
                break;
            default:
                break;
        }
        
    }
    
};
#endif
