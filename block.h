#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "Sprite.h"


class Block : public Sprite{
    
public:
    Block(){
        Sprite::setVisible(true);
        Sprite::initSprite("img/blob3.bmp");
//        Sprite::setCoords(200,450);
    }
    ~Block() {
        //        delete sprite;
    }
    
//    void update(vector<SDL_Rect> blockMap, vector<CollisionPair * > &colList, vector<vector<Sprite *> > &colGroups) {
//        
//    }

    void inCollision(enum colType t){
//        switch (t) {
//            case HeroBomb:
//                cout<<"hero bombed!"<<endl;
//                break;
//            case HeroExplosion:
//                cout<<"hero on fire"<<endl;
//                break;
//            default:
//                break;
//        }
        
    }
    
};
//extern Bomb * bomb;
#endif
