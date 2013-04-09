#ifndef __HERO_H__
#define __HERO_H__
#include "Sprite.h"
#include <iostream>
#include <utility>
#include <vector>
#include "CollisionPair.h"
#include "Bomb.h"
#include "CNet.h"
#include "heromessage.h"
#include "enemy.h"

#define NUM_HERO_FILES 12
using namespace std;
static const char * hero_file_names[NUM_HERO_FILES + 1] = {
    "img/person/player_1_1.bmp",
    "img/person/player_1_2.bmp",
    "img/person/player_1_3.bmp",
    "img/person/player_1_4.bmp",
    "img/person/player_1_5.bmp",
    "img/person/player_1_6.bmp",
    "img/person/player_1_7.bmp",
    "img/person/player_1_8.bmp",
    "img/person/player_1_9.bmp",
    "img/person/player_1_10.bmp",
    "img/person/player_1_11.bmp",
    "img/person/player_1_12.bmp",
    NULL
};


enum move_state {
    MOVE_LEFT,
    MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_UP,
    DONT_MOVE
};


class Hero : public Sprite{
#define TOTAL_LIFE_NUM 5
#define TOTAL_BOMB_LEVEL 3
private:
    enum move_state move;
    int frame;
    double speedX;
    double speedY;
    double walk_accel;
    double fall_accel;
    bool isBomb;
    bool protection;
    int inExplosionTime;
	heromessage msg;
    int life;
    int bombLevel;
	int bombx;
	int bomby;
	int playerId;
    hero_pos heropos;
    int walkOutTime;
public:
	CClientSocket* tcpclient;
	int* num;
    Hero(){
        Sprite::setVisible(true);
        move = DONT_MOVE;
        Sprite::initSprite(NUM_HERO_FILES, hero_file_names);
        setTransparent();
        //        Sprite::setCoords(500,450);
        frame = 0;
        speedX = 0.0;
        speedY = 0.0;
        walk_accel = 10.00;
        isBomb = false;
        protection = false;
		tcpclient=NULL;
        life = TOTAL_LIFE_NUM;
        bombLevel = 1;
        walkOutTime = 0;
    }
    ~Hero() {
        //        delete sprite;
    }
    int getFrame(){
        return frame;
    }
    void setFrame(int fr){
        frame = fr;
    }
    void setPlayerId(int id)
	{
		playerId=id;
	}
    hero_pos getPos(){
        return heropos;
    }
	int getBombLevel()
	{
		return bombLevel;
	}
	int getBombx()
	{
		return bombx;
	}
	int getBomby()
	{
		return bomby;
	}
	void setclient(CClientSocket* client)
	{
		tcpclient=client;
	}
    
#define MAX_SPEED  25
    void update(vector<Block * > blocks, vector<CollisionPair * > &colList, map<int, Hero* > &heroGroup, vector<Bomb *> &bombGroup, vector<Explosion *> &explosionGroup, vector<Enemy *> &enemyGroup) {
        
        if (!visible) {
            return;
        }
        //        frame = frame++;
        //        frame = frame & 3;
        //speedY = 0;
        switch(move) {
            case DONT_MOVE:
                speedX=0;
                speedY=0;
                break;
            case MOVE_RIGHT:
                if(speedX < MAX_SPEED) speedX = walk_accel;
                speedY=0;
                frame++;
                if (frame>2)
                    frame = 0;
                setAnimFrame(frame+3);
                break;
            case MOVE_LEFT:
                if(speedX > -MAX_SPEED) speedX = -1*walk_accel;
                speedY=0;
                frame++;
                if (frame>2)
                    frame = 0;
                setAnimFrame(frame+9);
                break;
            case MOVE_UP:
                if(speedY > -MAX_SPEED) speedY = -1*walk_accel;
                speedX=0;
                frame++;
                if (frame>2)
                    frame = 0;
                setAnimFrame(frame+6);
                break;
            case MOVE_DOWN:
                if(speedY < MAX_SPEED) 	speedY = walk_accel;
                speedX=0;
                frame++;
                if (frame>2)
                    frame = 0;
                setAnimFrame(frame);
                break;
        }
        int oriX = getX();
        int oriY = getY();
		int posX = getX()+speedX;
		int posY = getY()+speedY;
		if(posX<0) posX=0;
		if(posY<0) posY=0;
        
		heropos.x=posX;
		heropos.y=posY;
		heropos.id=playerId;
        setCoords(posX, posY);
		
        for (int i = 0; i < blocks.size(); i++){
            if (!blocks.at(i)->getVisible())
                continue;
            Block * tmp = blocks.at(i);
            if (getX()>tmp->getX()-getW() && getX()<tmp->getX()+tmp->getW() && getY()>tmp->getY()-getH()+5 && getY()<tmp->getY()+tmp->getH()-5){
                setCoords(oriX, oriY);
                break;
            }
        }
        if (SDL_GetTicks()-walkOutTime > 1000) {
            for (int i = 0; i < bombGroup.size(); i++){
                if (!bombGroup.at(i)->getVisible())
                    continue;
                Bomb * tmp = bombGroup.at(i);
                if (getX()>tmp->getX()-getW() && getX()<tmp->getX()+tmp->getW() && getY()>tmp->getY()-getH()+5 && getY()<tmp->getY()+tmp->getH()-5){
                    setCoords(oriX, oriY);
                    break;
                }
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
        //        Sprite::setAnimFrame(frame);
        
        if (isBomb){
			bombx=getX();
			bomby=getY();
            walkOutTime = SDL_GetTicks();
            /* Bomb * newBomb = new Bomb("img/blob2.bmp", getX(), getY(), 4000, SDL_GetTicks(), bombLevel);
             bombGroup.push_back(newBomb);
             for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
             CollisionPair * cp = new CollisionPair(it->second, newBomb, HeroBomb);
             colList.push_back(cp);
             }
             for (int i = 0; i < explosionGroup.size(); i++){
             CollisionPair * cp = new CollisionPair(newBomb, explosionGroup.at(i), BombExplosion);
             colList.push_back(cp);
             }
             for (int i = 0; i < enemyGroup.size(); i++){
             CollisionPair * cp = new CollisionPair(newBomb, enemyGroup.at(i), BombEnemy);
             colList.push_back(cp);
             }*/
            isBomb = false;
        }
    }
    
    void placeBomb(){
        isBomb = true;
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
    
    void inCollision(enum colType t){
        switch (t) {
            case HeroBomb:
                life--;
                cout<<"hero bombed! life "<<life<<endl;
                break;
            case HeroExplosion:
                if (!protection){
                    life--;
                    cout<<"hero on fire1: life "<<life<<endl;
                    inExplosionTime = SDL_GetTicks();
                    protection = true;
                }
                else if (protection && checkExplosionTime()){
                    life--;
                    cout<<"hero on fire2: life "<<life<<endl;
                    inExplosionTime = SDL_GetTicks();
                    
                }
                break;
            case HeroEnemy:
                if (!protection){
                    life--;
                    cout<<"enemy hero collison1! life: "<<life<<endl;
                    inExplosionTime = SDL_GetTicks();
                    protection = true;
                }
                else if (protection && checkExplosionTime()){
                    //                    protection = false;
                    life--;
                    cout<<"enemy hero collison2! life: "<<life<<endl;
                    inExplosionTime = SDL_GetTicks();
                    
                }
                break;
            case HeroUpgrade:
                if (bombLevel < TOTAL_BOMB_LEVEL)
                    bombLevel++;
                cout<<"hero bomb level: "<<bombLevel<<endl;
                break;
            case HeroLife:
                if (life < TOTAL_LIFE_NUM)
                    life++;
                cout<<"hero life: "<<life<<endl;
                break;
            default:
                break;
        }
        
    }
    
    bool checkExplosionTime(){
        return 1000 < SDL_GetTicks() - inExplosionTime;
    }
    
    int getLife() {
        return life;
    }
};
//extern Hero * hero;
#endif
