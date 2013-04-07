#ifndef __BOMB_H__
#define __BOMB_H__
#include "Sprite.h"
#include "Explosion.h"
#include <climits>
#include <algorithm>
#include "enemy.h"
class Hero;

#define NUM_EXP_FILES 3
static const char * explo_file_names[2*NUM_EXP_FILES + 1] = {
    "img/fire/explosion_1.bmp",
    "img/fire/explosion_2.bmp",
    "img/fire/explosion_3.bmp",
    "img/fire/explosion_4.bmp",
    "img/fire/explosion_5.bmp",
    "img/fire/explosion_6.bmp",
    NULL
};

class Bomb : public Sprite{
private:
    int explosionInterval;
    int awakeInterval;
    int oriTime;
    bool isAwake;
    int bombLevel;
public:
    Bomb(const char * file, int x, int y, int intev, int cur_time, int level){
        Sprite::setVisible(true);
        Sprite::initSprite(file);
        Sprite::setCoords(x, y);
        explosionInterval = intev;
        oriTime = cur_time;
        isAwake = false;
        awakeInterval = 1500;
        bombLevel = level;
	setTransparent();
    }
    ~Bomb() {
        //        delete sprite;
    }
    
    void update(vector<Block * > blocks, vector<CollisionPair * > &colList, map<int, Hero* > &heroGroup, vector<Bomb *> &bombGroup, vector<Explosion *> &explosionGroup, vector<Enemy *> &enemyGroup) {
        

        if (checkTimer(explosionInterval)){
            generateExplosions(blocks, colList, heroGroup, bombGroup, explosionGroup, enemyGroup);
            
        }
        if (!isAwake && checkTimer(awakeInterval)){
            isAwake = true;
            awakeInterval = INT_MAX;
        }
    }
    
    void generateExplosions(vector<Block * > blocks, vector<CollisionPair * > &colList, map<int, Hero* > &heroGroup, vector<Bomb *> &bombGroup, vector<Explosion *> &explosionGroup, vector<Enemy *> &enemyGroup) {
        if (!visible) {
            return;
        }
        setVisible(false);
//        isAwake = false;
        Explosion * explosion1 = new Explosion(explo_file_names[2*(bombLevel-1)], getX(), getY(), 4000, SDL_GetTicks());
        //            explosion1->setCoords(explosion1->getX() - explosion1->getW()/2, explosion1->getY());
        setHoriExplosionBound(explosion1, blocks);
        Explosion * explosion2 = new Explosion(explo_file_names[2*(bombLevel-1)+1], getX(), getY(), 4000, SDL_GetTicks());
        //            explosion2->setCoords(explosion2->getX(), explosion2->getY() - explosion2->getH()/2);
        setVertExplosionBound(explosion2, blocks);
        explosionGroup.push_back(explosion1);
        explosionGroup.push_back(explosion2);
        
        // add HeroExplosion collison pairs
        for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
            CollisionPair * cp1 = new CollisionPair(it->second, explosion1, HeroExplosion);
            colList.push_back(cp1);
            CollisionPair * cp2 = new CollisionPair(it->second, explosion2, HeroExplosion);
            colList.push_back(cp2);
        }
        // add BombExplosion collison pairs
        for (int i = 0; i < bombGroup.size(); i++) {
            CollisionPair * cp1 = new CollisionPair(bombGroup.at(i), explosion1, BombExplosion);
            colList.push_back(cp1);
            CollisionPair * cp2 = new CollisionPair(bombGroup.at(i), explosion2, BombExplosion);
            colList.push_back(cp2);
        }
        // add EnemyExplosion collison pairs
        for (int i = 0; i < enemyGroup.size(); i++) {
            CollisionPair * cp1 = new CollisionPair(enemyGroup.at(i), explosion1, EnemyExplosion);
            colList.push_back(cp1);
            CollisionPair * cp2 = new CollisionPair(enemyGroup.at(i), explosion2, EnemyExplosion);
            colList.push_back(cp2);
        }
        // add BlockExplosion collison pairs
        for (int i = 0; i < blocks.size(); i++) {
            if (blocks.at(i)->getSolid())
                continue;
            if (!blocks.at(i)->getVisible())
                continue;
            CollisionPair * cp1 = new CollisionPair(blocks.at(i), explosion1, BlockExplosion);
            colList.push_back(cp1);
            CollisionPair * cp2 = new CollisionPair(blocks.at(i), explosion2, BlockExplosion);
            colList.push_back(cp2);
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
        int hi = exp->getY()-exp->getH()/2+getH()/2;
        int lo = exp->getY()+exp->getH()/2+getH()/2;
        for (int i = 0; i < blocks.size(); i++){
            Block * bl = blocks.at(i);
            if (!bl->getSolid() || bl->getX()>exp->getX()+exp->getW() || bl->getX()+bl->getW()<exp->getX())
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
    
    void setHoriExplosionBound(Explosion * exp, vector<Block *> blocks) {
        int le = exp->getX()-exp->getW()/2+getW()/2;
        int ri = exp->getX()+exp->getW()/2+getW()/2;
        for (int i = 0; i < blocks.size(); i++){
            Block * bl = blocks.at(i);
            if (!bl->getSolid() || bl->getY()>exp->getY()+exp->getH() || bl->getY()+bl->getH()<exp->getY())
                continue;
            if (exp->getX()>=bl->getX()+bl->getW()){
                le = max(bl->getX()+bl->getW(), le);
            }
            else if (exp->getX()<=bl->getX()){
                ri = min(bl->getX(), ri);
            }
        }
        int wi = ri-le+1;
        exp->setSurface(wi, exp->getH());
        exp->setCoords(le, exp->getY());
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
            case BombEnemy:
//                setVisible(false);
                explosionInterval = 0;
                break;
            case BombExplosion:
                cout<<"i here"<<endl;
                explosionInterval = 0;
//                awakeInterval = 0;
                break;
            default:
                break;
        }
    }
    
};
#endif
