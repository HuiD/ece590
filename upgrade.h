#ifndef __UPGRADE_H__
#define __UPGRADE_H__
#include "Sprite.h"
#include "CollisionPair.h"
#include <algorithm>
class Block;
class Bomb;
class Hero;
class Explosion;
class Enemy;

#define NUM_LIFEUP_FILES 2
#define NUM_FIREUP_FILES 2
#define NUM_SPEEDUP_FILES 2
#define NUM_BOMBUP_FILES 2

static const char * lifeup_file_names[NUM_LIFEUP_FILES + 1] = {
    "img/items/lifeup_1.bmp",
    "img/items/lifeup_2.bmp",
    NULL
};

static const char * fireup_file_names[NUM_FIREUP_FILES + 1] = {
    "img/items/fireup_1.bmp",
    "img/items/fireup_2.bmp",
    NULL
};

static const char * speedup_file_names[NUM_SPEEDUP_FILES + 1] = {
    "img/items/speedup_1.bmp",
    "img/items/speedup_2.bmp",
    NULL
};

static const char * bombup_file_names[NUM_BOMBUP_FILES + 1] = {
    "img/items/bombup_1.bmp",
    "img/items/bombup_2.bmp",
    NULL
};

class Upgrade : public Sprite{
   
#define LIFETIME  6000
    
private:
    enum colType upType;
    int oriTime;
    int frame;
public:
    Upgrade(int x, int y, enum colType t){
        Sprite::setVisible(true);
        upType = t;
        switch (upType) {
            case HeroUpgrade:
                Sprite::initSprite(NUM_FIREUP_FILES, fireup_file_names);
                frame = 0;
                break;
            case HeroLife:
                Sprite::initSprite(NUM_LIFEUP_FILES, lifeup_file_names);
		frame = 0;
                break;
	    case HeroSpeed:
		Sprite::initSprite(NUM_SPEEDUP_FILES, speedup_file_names);
		frame = 0;
		break;
	    case HeroBomb:
		Sprite::initSprite(NUM_BOMBUP_FILES, bombup_file_names);
		frame = 0;
		break;
            default:
                break;
        }
        Sprite::setCoords(x,y);
        oriTime = SDL_GetTicks();
    }
    ~Upgrade() {
        //        delete sprite;
    }
    
    void update() {
        if (!visible)
            return;
        frame++;
        if (frame > 2)
           frame = 0;
        setAnimFrame(frame);
        if (SDL_GetTicks() - oriTime > LIFETIME)
            setVisible(false);
    }

    void inCollision(enum colType t){
        switch (t) {
            case HeroUpgrade:
//      cout<<"hero upgraded!"<<endl;
                setVisible(false);
                break;
            case HeroLife:
//      cout<<"hero upgraded!"<<endl;
                setVisible(false);
                break;
	    case HeroSpeed:
//	cout<<"hero upgraded!"<<endl;
		setVisible(false);
		break;
	    case HeroBomb:
//	cout<<"hero upgraded!"<<endl;
		setVisible(false);		
		break;
            default:
                break;
        }       
    }
    
    colType getUpType() {
        return upType;
    } 
};
#endif
