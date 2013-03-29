#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include "Sprite.h"

#define NUM_HERO_FILES 8

class Background : public Sprite{
public:
    Background(const char * file){
        Sprite::initSprite(file);
        setVisible(true);
    }
    ~Background() {
//        delete sprite;
    }

    
    void onCollison(){
        cout<<"Background collide!"<<endl;
    }
    
//    void update(vector<SDL_Rect> blockMap, vector<CollisionPair * > &colList, vector<vector<Sprite *> > &colGroups){
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
//extern Background * block;
#endif
