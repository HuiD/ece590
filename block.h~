#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "Sprite.h"
#include "CollisionPair.h"
#include <algorithm>

class Bomb;
class Hero;
class Explosion;
class Enemy;

class Block : public Sprite{
    
private:
    bool solid;
    bool exploded;
public:
    Block(int x, int y, bool ifSolid){
        Sprite::setVisible(true);
        Sprite::initSprite("img/blob3.bmp");
        Sprite::setCoords(x,y);
        solid = ifSolid;
        exploded = false;
    }
    ~Block() {
        //        delete sprite;
    }
    
    void update(vector<Block * > blocks, vector<CollisionPair * > &colList, vector<Hero *> &heroGroup, vector<Bomb *> &bombGroup, vector<Explosion *> &explosionGroup, vector<Enemy *> &enemyGroup) {
//        if (exploded) {
//            vector<Block *>::iterator position = find(blocks.begin(), blocks.end(), this);
//            if (position != blocks.end()) {
//                blocks.erase(position);
//            }
//        }
    }

    void inCollision(enum colType t){
        switch (t) {
            case BlockExplosion:
//                cout<<"block bombed!"<<endl;
                setVisible(false);
                exploded = true;
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
