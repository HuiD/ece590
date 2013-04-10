#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "Sprite.h"
#include "CollisionPair.h"
#include <algorithm>
#include <map>
//class Upgrade;
#include "upgrade.h"
#include <stdlib.h>     /* srand, rand */

class Bomb;
class Hero;
class Explosion;
class Enemy;

#define NUM_BLOCK_FILES 7
static const char * block_file_names[NUM_BLOCK_FILES + 1] = {
    "img/blocks/tiles_1.bmp",
    "img/blocks/tiles_2.bmp",
    "img/blocks/tiles_3.bmp",
    "img/blocks/tiles_4.bmp",
    "img/blocks/tiles_5.bmp",
    "img/blocks/tiles_6.bmp",
    "img/blocks/tiles_7.bmp",
    NULL
};

class Block : public Sprite{
    
private:
    bool solid;
    bool isDestroyed;
    int upgradeType;
    int frame;
    bool isExploded;
public:
    Block(int x, int y, bool ifSolid, int rand){
        Sprite::setVisible(true);
        if (ifSolid)
            Sprite::initSprite("img/blocks/tiles_0.bmp");
        else 
            initSprite(NUM_BLOCK_FILES, block_file_names);    
	frame = 0;
	isDestroyed = false;
        Sprite::setCoords(x,y);
        solid = ifSolid;
        if(rand==0)
            upgradeType=1;
        else if(rand==1)
            upgradeType=2;
        else
            upgradeType=0;
        isExploded = false;
    }
    ~Block() {
        //        delete sprite;
    }
    
    void update(vector<CollisionPair * > &colList, map<int, Hero*> &heroGroup, vector<Explosion *> &explosionGroup, vector<Upgrade *> &upgradeGroup) {
        if (isDestroyed) {
	    frame ++;
	    setAnimFrame(frame);
	    if (frame > 6) {
		setVisible(false);
                if (upgradeType>0) {
            	    enum colType t;
                    switch (upgradeType) {
                	case 1:
                    	    t = HeroUpgrade;
                    	    break;
               		case 2:
                            t = HeroLife;
                    	    break;
                	default:
                    	    break;
                    }
            	    Upgrade * up = new Upgrade(getX(), getY(), t);
            	    upgradeGroup.push_back(up);
            	    for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
                	CollisionPair * cp = new CollisionPair(it->second, up, up->getUpType());
                	colList.push_back(cp);
            	    }
            	    upgradeType = 0;
                }
	    }
	}

	if (isExploded){
            if (upgradeType>0) {
                enum colType t;
                switch (upgradeType) {
                    case 1:
                        t = HeroUpgrade;
                        break;
                    case 2:
                        t = HeroLife;
                        break;
                    default:
                        break;
                }
                Upgrade * up = new Upgrade(getX(), getY(), t);
                upgradeGroup.push_back(up);
                for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
                    CollisionPair * cp = new CollisionPair(it->second, up, up->getUpType());
                    colList.push_back(cp);
                }
                upgradeType = 0;
            }
            isExploded = false;
        }   
}

    
    void inCollision(enum colType t){
        int tmp;
        switch (t) {
            case BlockExplosion:
                //                cout<<"block bombed!"<<endl;

                // setVisible(false);
                // generate upgrades
		isDestroyed = true;
                
                isExploded = true;

                break;
            default:
                break;
        }
        
    }
    
    bool getSolid(){
        return solid;
    }
    
};
#endif
