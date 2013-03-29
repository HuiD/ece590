#ifndef __BOMB_H__
#define __BOMB_H__
#include "Sprite.h"
#include "Explosion.h"
#include <climits>
#include <algorithm>
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
//            explosion1->setSurface(explosion1->getW()*2/3, explosion1->getH());
            explosion1->setCoords(explosion1->getX() - explosion1->getW()/2, explosion1->getY());
            Explosion * explosion2 = new Explosion("img/explo2.bmp", getX(), getY(), 4000, SDL_GetTicks());
//            explosion2->setCoords(explosion2->getX(), explosion2->getY() - explosion2->getH()/2);
            setVertExplosionBound(explosion2, blocks);
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
    
    void setVertExplosionBound(Explosion * exp, vector<Block *> blocks) {
//        exp->setCoords(exp->getX(), exp->getY() - exp->getH()/2);
//        for (int i = 0; i < blocks.size(); i++){
//            Block * bl = blocks.at(i);            
//            if (bl->getY()>exp->getY()+exp->getH())
//                continue;
//            else if (bl->getY()>exp->getY()){
////                exp->setH(bl->getY()-exp->getY());
//                exp->setSurface(exp->getW(), bl->getY()-exp->getY());
//            }
//            else if (bl->getY()+bl->getH()<exp->getY())
//                continue;
//            else {
//                exp->setSurface(exp->getW(), exp->getH() - (bl->getY()+bl->getH()-exp->getY()));
//
////                exp->setH(exp->getH() - (bl->getY()+bl->getH()-exp->getY()));
//                exp->setCoords(exp->getX(), bl->getY()+bl->getH());
//            }
//                
//        }
        int hi = exp->getY()-exp->getH()/2;
        int lo = exp->getY()+exp->getH()/2;
        for (int i = 0; i < blocks.size(); i++){
            Block * bl = blocks.at(i);
            if (bl->getX()>exp->getX()+exp->getW() || bl->getX()+bl->getW()<exp->getX())
                continue;
            if (exp->getY()>=bl->getY()+bl->getH()){
                hi = max(bl->getY()+bl->getH(), hi);
            }
            else if (exp->getY()<=bl->getY()){
                lo = min(bl->getY(), lo);
            }
        }
        int ht = lo-hi+1;
        exp->setSurface(exp->getW(), ht);
        exp->setCoords(exp->getX(), hi);
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
