#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include "Sprite.h"
#include "upgrade.h"
class Hero;


#define PERIOD 8000
#define UPGRADES_WIDTH 80

class Background : public Sprite{
    
private:
    int oriTime;
public:
    Background(const char * file){
        Sprite::initSprite(file);
        setVisible(true);
        oriTime = SDL_GetTicks();
    }
    ~Background() {

    }
    
    
    void onCollison(){

    }
    
    void update(vector<Block * > blocks, vector<CollisionPair * > &colList, map<int, Hero*> &heroGroup, vector<Upgrade *> &upgradeGroup){

    }
    
    void inCollision(enum colType t){
        
    }
};
#endif
