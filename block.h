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

class Block : public Sprite{
    
private:
    bool solid;
    int upgradeType;
public:
    Block(int x, int y, bool ifSolid, int rand){
        Sprite::setVisible(true);
        if (ifSolid)
            Sprite::initSprite("img/blocks/tiles_0.bmp");
        else
            initSprite("img/blocks/tiles_1.bmp");
        Sprite::setCoords(x,y);
        solid = ifSolid;
        if(rand==0)
            upgradeType=1;
        else if(rand==1)
            upgradeType=2;
        else
            upgradeType=0;
    }
    ~Block() {
        //        delete sprite;
    }
    
    void update(vector<CollisionPair * > &colList, map<int, Hero*> &heroGroup, vector<Explosion *> &explosionGroup, vector<Upgrade *> &upgradeGroup) {
        
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
    
    void inCollision(enum colType t){
        int tmp;
        switch (t) {
            case BlockExplosion:
                //                cout<<"block bombed!"<<endl;
                setVisible(false);
                // generate upgrades
//                tmp = rand()%4;
//                switch (tmp) {
//                        
//                    case 2:
//                        upgradeType = 1;
//                        break;
//                    case 3:
//                        upgradeType = 2;
//                        break;
//                    default:
//                        break;
//                }
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
