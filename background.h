#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include "Sprite.h"
#include "upgrade.h"
#include <stdlib.h>     /* srand, rand */
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
//        delete sprite;
    }

    
    void onCollison(){
//        cout<<"Background collide!"<<endl;
    }
    
    void update(vector<Block * > blocks, vector<CollisionPair * > &colList, vector<Hero *> &heroGroup, vector<Upgrade *> &upgradeGroup){
        if (SDL_GetTicks() - oriTime > PERIOD){
            int x = rand()%(WINDOW_WIDTH-UPGRADES_WIDTH);
            int y = rand()%(WINDOW_HEIGHT-UPGRADES_WIDTH);
            for (int i = 0; i < blocks.size(); i++) {
                Block * bl = blocks.at(i);
                if (!bl->getVisible())
                    continue;
                if (x>bl->getX()-UPGRADES_WIDTH && x<bl->getX()+bl->getW() && y>bl->getY()-UPGRADES_WIDTH && y<bl->getY()+getH())
                    return;
            }
            enum colType t;
            int tmp = rand()%2;
            switch (tmp) {
                case 0:
                    t = HeroUpgrade;
                    break;
                case 1:
                    t = HeroLife;
                    break;
//                case 2:
//                    t = addLife;
//                    break;
                default:
                    break;
            }
            Upgrade * up = new Upgrade(x, y, t);
            upgradeGroup.push_back(up);
            for (int i = 0; i < heroGroup.size(); i++){
                CollisionPair * cp = new CollisionPair(heroGroup.at(i), up, up->getUpType());
                colList.push_back(cp);
                
            }
            oriTime = SDL_GetTicks();
        }
    }
    
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
#endif
