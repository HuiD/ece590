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
#include <queue>

#define NUM_HERO_FILES 16

using namespace std;
static const char * hero1_file_names[NUM_HERO_FILES + 1] = {
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
    "img/person/player_1_die1.bmp",
    "img/person/player_1_die2.bmp",
    "img/person/player_1_die3.bmp",
    "img/person/player_1_die4.bmp",
    NULL
};

static const char * hero2_file_names[NUM_HERO_FILES + 1] = {
    "img/person/player_2_1.bmp",
    "img/person/player_2_2.bmp",
    "img/person/player_2_3.bmp",
    "img/person/player_2_4.bmp",
    "img/person/player_2_5.bmp",
    "img/person/player_2_6.bmp",
    "img/person/player_2_7.bmp",
    "img/person/player_2_8.bmp",
    "img/person/player_2_9.bmp",
    "img/person/player_2_10.bmp",
    "img/person/player_2_11.bmp",
    "img/person/player_2_12.bmp",
    "img/person/player_2_die1.bmp",
    "img/person/player_2_die2.bmp",
    "img/person/player_2_die3.bmp",
    "img/person/player_2_die4.bmp",
    NULL
};

static const char * hero3_file_names[NUM_HERO_FILES + 1] = {
    "img/person/player_3_1.bmp",
    "img/person/player_3_2.bmp",
    "img/person/player_3_3.bmp",
    "img/person/player_3_4.bmp",
    "img/person/player_3_5.bmp",
    "img/person/player_3_6.bmp",
    "img/person/player_3_7.bmp",
    "img/person/player_3_8.bmp",
    "img/person/player_3_9.bmp",
    "img/person/player_3_10.bmp",
    "img/person/player_3_11.bmp",
    "img/person/player_3_12.bmp",
    "img/person/player_3_die1.bmp",
    "img/person/player_3_die2.bmp",
    "img/person/player_3_die3.bmp",
    "img/person/player_3_die4.bmp",
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
#define TOTAL_LIFE_NUM 2
#define TOTAL_BOMB_LEVEL 3
#define TOTAL_BOMB_NUM 9
private:
    enum move_state move;
    int frame;
    int deadframe;
    int counter;
    double speedX;
    double speedY;
    double walk_accel;
    bool isBomb;
    bool isDead;
    bool protection;
    int inExplosionTime;
	heromessage msg;
    int life;
    int maxBombNum;
    int bombLevel;
    int bombx;
    int bomby;
    int playerId;
    int speedUpTime;
    hero_pos heropos;
    int walkOutTime;
    Mix_Chunk *placeBombSFX;
    Mix_Chunk *deadSFX;
    Mix_Chunk *slowdownSFX;
    queue<int> exploTime;
public:
	CClientSocket* tcpclient;
	int* num;
    Hero(int id){
	playerId = id;
        Sprite::setVisible(true);
        move = DONT_MOVE;
	switch(playerId) {
	    case 0:
        	Sprite::initSprite(NUM_HERO_FILES, hero1_file_names);
		break;
	    case 1:
		Sprite::initSprite(NUM_HERO_FILES, hero2_file_names);
		break;
	    case 2:
		Sprite::initSprite(NUM_HERO_FILES, hero3_file_names);
		break;
	    case 4:
		Sprite::initSprite(NUM_HERO_FILES, hero3_file_names);
		break;
	    default:
		break;
	}
        setTransparent();
        frame = 0;
	deadframe = 11;
	counter = 0;
        speedX = 0.0;
        speedY = 0.0;
        walk_accel = 10.00;
        isBomb = false;
	isDead = false;
        protection = false;
	tcpclient=NULL;
        life = TOTAL_LIFE_NUM;
        bombLevel = 1;
	maxBombNum = 1;
        walkOutTime = 0;
	speedUpTime = 0;
	placeBombSFX = Mix_LoadWAV("sound/placebomb.wav");
	deadSFX = Mix_LoadWAV("sound/die.wav");
	slowdownSFX = Mix_LoadWAV("sound/slowdown.wav");
    }
    ~Hero() {
        Mix_FreeChunk(placeBombSFX);
        Mix_FreeChunk(deadSFX);
        Mix_FreeChunk(slowdownSFX);

    }
    int getFrame(){
        return frame;
    }
    void setFrame(int fr){
        frame = fr;
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
    void update(vector<Block * > blocks, vector<CollisionPair * > &colList, map<int, Hero* > &heroGroup, vector<Bomb *> &bombGroup, vector<Explosion *> &explosionGroup) {

        if (!visible) {
            return;
        }

	if (isDead) {
	    move = DONT_MOVE;
	    counter++;
	    if (counter == 4) {
	    	deadframe++;
		counter = 0;
	    	setAnimFrame(deadframe);
	    	if (deadframe > 15) {
		    frame = 0;
		    deadframe = 11;
		    isDead = false;
		    setAnimFrame(frame);
		    if (life <= 0) {
			setVisible(false);
			return;
		    }
		    switch (playerId) {
            		case 0:
                	    setCoords(UNIT, 3*UNIT);
                	    break;
            		case 1:
                	    setCoords((WINDOW_WIDTH/UNIT-2)*UNIT, (WINDOW_HEIGHT/UNIT-2)*UNIT);
                	    break;
            		case 2:
                	    setCoords((WINDOW_WIDTH/UNIT-2)*UNIT, 3*UNIT);
                	    break;
            		case 3:
                	    setCoords(UNIT, (WINDOW_HEIGHT/UNIT-2)*UNIT);
                	    break;
            		default:
                	    break;
        	    }
		    walk_accel = 10;
		    maxBombNum = 1;
		    bombLevel = 1;
		    speedUpTime = 0;
	        }
	    }
	}

	if (walk_accel == 25 && SDL_GetTicks()-speedUpTime > 10000) {
	    walk_accel = 10;
	    if (Mix_PlayChannel(-1, slowdownSFX, 0) == -1)
		fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
	}

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
        
	if (!exploTime.empty()) {
	    if (SDL_GetTicks()-exploTime.front() > 4000) { 
	    	exploTime.pop(); 
	    }   
	}

        if (isBomb){	   
	    bombx=getX();
	    bomby=getY();
            walkOutTime = SDL_GetTicks();
	    isBomb = false;
        }
    }
    
    bool getIsBomb(){
	return isBomb;
    }

    void placeBomb(){
	if (exploTime.size() < maxBombNum) {
	    if (Mix_PlayChannel(-1, placeBombSFX, 0) == -1)
		fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
	    isBomb = true;
	    exploTime.push(SDL_GetTicks());
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
    
    void inCollision(enum colType t){
        switch (t) {
            case HeroExplosion:
		isDead = true;
		
                if (!protection){
		    if (Mix_PlayChannel(-1, deadSFX, 0) == -1)
	    	    	fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
                    life--;
                    inExplosionTime = SDL_GetTicks();
                    protection = true;
                }
                else if (protection && checkExplosionTime()){
		    if (Mix_PlayChannel(-1, deadSFX, 0) == -1)
	    	    	fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
                    life--;
                    inExplosionTime = SDL_GetTicks();
                }
                break;
            case HeroEnemy:
		isDead = true;
		
                if (!protection){
		    if (Mix_PlayChannel(-1, deadSFX, 0) == -1)
	    	    	fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
                    life--;

                    inExplosionTime = SDL_GetTicks();
                    protection = true;
                }
                else if (protection && checkExplosionTime()){
		    if (Mix_PlayChannel(-1, deadSFX, 0) == -1)
	    	    	fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
                    life--;
                    inExplosionTime = SDL_GetTicks();
                    
                }
                break;
            case HeroUpgrade:
                if (bombLevel < TOTAL_BOMB_LEVEL)
                    bombLevel++;
                break;
            case HeroLife:
                if (life < TOTAL_LIFE_NUM)
                    life++;
                break;
	    case HeroSpeed:
		walk_accel = 25;
		speedUpTime = SDL_GetTicks();  
	        break;
	    case HeroBomb:
		if (maxBombNum < TOTAL_BOMB_NUM)
		    maxBombNum++;
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

#endif
