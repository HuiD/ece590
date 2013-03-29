#ifndef __BOMB_H__
#define __BOMB_H__
#include "Sprite.h"
#include "Explosion.h"
#include <climits>
class Hero;

class Bomb : public Sprite{
private:
    int explosionInterval;
    int awakeInterval;
    int oriTime;
    bool isAwake;
public:
    Bomb(const char * file, int x, int y, int intev, int cur_time){
        Sprite::setVisible(true);
        Sprite::initSprite(file);
        Sprite::setCoords(x, y);
        explosionInterval = intev;
        oriTime = cur_time;
        isAwake = false;
        awakeInterval = 1500;
    }
    ~Bomb() {
//        delete sprite;
    }
    
    void update(vector<Block * > blocks, vector<CollisionPair * > &colList, vector<Hero *> &heroGroup, vector<Explosion *> &explosionGroup) {
        if (!isAwake && checkTimer(awakeInterval)){
            isAwake = true;
            awakeInterval = INT_MAX;
        }
        if (checkTimer(explosionInterval)){
            if (!visible) {
                return;
            }
            setVisible(false);
            isAwake = false;
            Explosion * explosion1 = new Explosion("img/explo1.bmp", getX(), getY(), 4000, SDL_GetTicks());
            explosion1->setCoords(explosion1->getX() - explosion1->getW()/2, explosion1->getY());
            Explosion * explosion2 = new Explosion("img/explo2.bmp", getX(), getY(), 4000, SDL_GetTicks());
            explosion2->setCoords(explosion2->getX(), explosion2->getY() - explosion2->getH()/2);
//            vector<Sprite *> heroGroup = colGroups.at(0);
            explosionGroup.push_back(explosion1);
            explosionGroup.push_back(explosion2);

            for (int i = 0; i < heroGroup.size(); i++){
                CollisionPair * cp1 = new CollisionPair(heroGroup.at(i), explosion1, HeroExplosion);
                colList.push_back(cp1);
                CollisionPair * cp2 = new CollisionPair(heroGroup.at(i), explosion2, HeroExplosion);
                colList.push_back(cp2);
            }

        }
    }
    
    bool checkTimer(int interval) {
        int eclipsedTime = SDL_GetTicks() - oriTime;
        if (eclipsedTime > interval)
            return true;
        return false;
    }
    
    bool isCollided(Sprite * other) {
//        cout<<"im here"<<endl;
        if (isAwake)
            return Sprite::isCollided(other);
        return false;
    }
    
    void inCollision(enum colType t){
        switch (t) {
            case HeroBomb:
                setVisible(false);
                break;
                
            default:
                break;
        }
    }
    
};
#endif
