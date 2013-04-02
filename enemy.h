#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "Sprite.h"
#include <iostream>
#include <utility>
#include <vector>
#include "CollisionPair.h"
class Bomb;
#include <stdlib.h>     /* srand, rand */



#define NUM_ENEMY_FILES 8
using namespace std;
static const char * enemy_file_names[NUM_ENEMY_FILES + 1] = {
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



class Enemy : public Sprite{
private:
//    enum move_state move;
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
//        move = DONT_MOVE;
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

    
#define SPEED  7
    void update(vector<Block * > blocks, vector<CollisionPair * > &colList, map<int, Hero* > &heroGroup, vector<Bomb *> &bombGroup, vector<Explosion *> &explosionGroup) {
//        cout<<SDL_GetTicks()<<endl;
        if (!visible) {
            return;
        }
        frame = frame++;
        frame = frame & 3;
        //speedY = 0;
        if (SDL_GetTicks() - ori_time > 400) {
            ori_time = SDL_GetTicks();
            direction = rand()%5;
        }
//        int n = rand()%5;
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
            if (getX()>=tmp->getX()-getW() && getX()<=tmp->getX()+tmp->getW() && getY()>=tmp->getY()-getH() && getY()<=tmp->getY()+tmp->getH()){
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
    
    void inCollision(enum colType t){
        switch (t) {
            case HeroEnemy:
//                cout<<"hero enemy collison!"<<endl;
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