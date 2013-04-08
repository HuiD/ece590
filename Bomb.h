#ifndef __BOMB_H__
#define __BOMB_H__
#include "Sprite.h"
#include "Explosion.h"
#include <climits>
#include <algorithm>
#include "enemy.h"
class Hero;

#define NUM_EXP_FILES 9
#define NUM_BOMB_FILES 3
static const char * explo1h_file_names[NUM_EXP_FILES + 1] = {
    "img/fire/explosion_1h_1.bmp",
    "img/fire/explosion_1h_2.bmp",
    "img/fire/explosion_1h_3.bmp",
    "img/fire/explosion_1h_4.bmp",
    "img/fire/explosion_1h_5.bmp",
    "img/fire/explosion_1h_6.bmp",
    "img/fire/explosion_1h_7.bmp",
    "img/fire/explosion_1h_8.bmp",
    "img/fire/explosion_1h_9.bmp",
    NULL
};

static const char * explo2h_file_names[NUM_EXP_FILES + 1] = {
    "img/fire/explosion_2h_1.bmp",
    "img/fire/explosion_2h_2.bmp",
    "img/fire/explosion_2h_3.bmp",
    "img/fire/explosion_2h_4.bmp",
    "img/fire/explosion_2h_5.bmp",
    "img/fire/explosion_2h_6.bmp",
    "img/fire/explosion_2h_7.bmp",
    "img/fire/explosion_2h_8.bmp",
    "img/fire/explosion_2h_9.bmp",
    NULL
};

static const char * explo3h_file_names[NUM_EXP_FILES + 1] = {
    "img/fire/explosion_3h_1.bmp",
    "img/fire/explosion_3h_2.bmp",
    "img/fire/explosion_3h_3.bmp",
    "img/fire/explosion_3h_4.bmp",
    "img/fire/explosion_3h_5.bmp",
    "img/fire/explosion_3h_6.bmp",
    "img/fire/explosion_3h_7.bmp",
    "img/fire/explosion_3h_8.bmp",
    "img/fire/explosion_3h_9.bmp",
    NULL
};

static const char * explo1v_file_names[NUM_EXP_FILES + 1] = {
    "img/fire/explosion_1v_1.bmp",
    "img/fire/explosion_1v_2.bmp",
    "img/fire/explosion_1v_3.bmp",
    "img/fire/explosion_1v_4.bmp",
    "img/fire/explosion_1v_5.bmp",
    "img/fire/explosion_1v_6.bmp",
    "img/fire/explosion_1v_7.bmp",
    "img/fire/explosion_1v_8.bmp",
    "img/fire/explosion_1v_9.bmp",
    NULL
};

static const char * explo2v_file_names[NUM_EXP_FILES + 1] = {
    "img/fire/explosion_2v_1.bmp",
    "img/fire/explosion_2v_2.bmp",
    "img/fire/explosion_2v_3.bmp",
    "img/fire/explosion_2v_4.bmp",
    "img/fire/explosion_2v_5.bmp",
    "img/fire/explosion_2v_6.bmp",
    "img/fire/explosion_2v_7.bmp",
    "img/fire/explosion_2v_8.bmp",
    "img/fire/explosion_2v_9.bmp",
    NULL
};

static const char * explo3v_file_names[NUM_EXP_FILES + 1] = {
    "img/fire/explosion_3v_1.bmp",
    "img/fire/explosion_3v_2.bmp",
    "img/fire/explosion_3v_3.bmp",
    "img/fire/explosion_3v_4.bmp",
    "img/fire/explosion_3v_5.bmp",
    "img/fire/explosion_3v_6.bmp",
    "img/fire/explosion_3v_7.bmp",
    "img/fire/explosion_3v_8.bmp",
    "img/fire/explosion_3v_9.bmp",
    NULL
};

static const char * bomb_file_names[NUM_BOMB_FILES + 1] = {
    "img/items/bomb_3.bmp",
    "img/items/bomb_2.bmp",
    "img/items/bomb_1.bmp",
    NULL
};

class Bomb : public Sprite{
private:
    int explosionInterval;
    int frame;
    int awakeInterval;
    int oriTime;
    bool isAwake;
    int bombLevel;
    int counter;
public:
    Bomb(int x, int y, int intev, int cur_time, int level){
        Sprite::setVisible(true);
        Sprite::initSprite(NUM_BOMB_FILES, bomb_file_names);
        Sprite::setCoords(x, y);
        explosionInterval = intev;
        oriTime = cur_time;
	frame = 0;
	counter = 0;
        isAwake = false;
        awakeInterval = 1500;
        bombLevel = level;
	setTransparent();
    }
    ~Bomb() {
        //        delete sprite;
    }
    
    void update(vector<Block * > blocks, vector<CollisionPair * > &colList, map<int, Hero* > &heroGroup, vector<Bomb *> &bombGroup, vector<Explosion *> &explosionGroup, vector<Enemy *> &enemyGroup) {
        counter++;
	if (counter == 2) {
           frame = frame++;
           if (frame > 2) 
	      frame = 0;
	   counter = 0;
	   setAnimFrame(frame);
        }
	
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
	Explosion * explosion1;
	Explosion * explosion2;
        switch(bombLevel) {
            case 1:
                explosion1 = new Explosion(explo1h_file_names, getX(), getY(), 500, SDL_GetTicks());
        	setHoriExplosionBound(explosion1, blocks);
        	explosion2 = new Explosion(explo1v_file_names, getX(), getY(), 500, SDL_GetTicks());
        	setVertExplosionBound(explosion2, blocks);
		break;
	    case 2:
		explosion1 = new Explosion(explo2h_file_names, getX(), getY(), 500, SDL_GetTicks());
        	setHoriExplosionBound(explosion1, blocks);
        	explosion2 = new Explosion(explo2v_file_names, getX(), getY(), 500, SDL_GetTicks());
        	setVertExplosionBound(explosion2, blocks);
		break;
	    case 3:
		explosion1 = new Explosion(explo3h_file_names, getX(), getY(), 500, SDL_GetTicks());
        	setHoriExplosionBound(explosion1, blocks);
        	explosion2 = new Explosion(explo3v_file_names, getX(), getY(), 500, SDL_GetTicks());
        	setVertExplosionBound(explosion2, blocks);
		break;
	    default:
		break;
	}
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
        int hi = exp->getY()-exp->getH()/2+getH()/2;
        int lo = exp->getY()+exp->getH()/2+getH()/2;
        for (int i = 0; i < blocks.size(); i++){
            Block * bl = blocks.at(i);
            if (!bl->getSolid() || bl->getX()>exp->getX()+exp->getW()-OFFSET || bl->getX()+bl->getW()<exp->getX()+OFFSET)
                continue;
            if (exp->getY()>=bl->getY()+bl->getH()){
                hi = max(bl->getY()+bl->getH(), hi);
            }
            else if (exp->getY()<=bl->getY()){
                lo = min(bl->getY(), lo);
            }
        }
        int ht = lo-hi;
        int tmp = exp->getY()-exp->getH()/2+getH()/2;
        exp->setCoords(exp->getX(), hi);
        exp->setH(ht);
//        exp->rect.x=0;
//        exp->rect.y=hi-tmp;
//        exp->rect.w=exp->getW();
//        exp->rect.h=ht;
        exp->setShowPart(0, hi-tmp, exp->getW(), ht);
//        exp->setSurface(exp->getX(), hi, exp->getW(), ht);
//        exp->setCoords(exp->getX(), hi);
    }
    
    void setHoriExplosionBound(Explosion * exp, vector<Block *> blocks) {
        int le = exp->getX()-exp->getW()/2+getW()/2;
        int ri = exp->getX()+exp->getW()/2+getW()/2;
        for (int i = 0; i < blocks.size(); i++){
            Block * bl = blocks.at(i);
            if (!bl->getSolid() || bl->getY()>exp->getY()+exp->getH()-OFFSET || bl->getY()+bl->getH()<exp->getY()+OFFSET)
                continue;
            if (exp->getX()>=bl->getX()+bl->getW()){
                le = max(bl->getX()+bl->getW(), le);
            }
            else if (exp->getX()<=bl->getX()){
                ri = min(bl->getX(), ri);
            }
        }
        int wi = ri-le;
        int tmp = exp->getX()-exp->getW()/2+getW()/2;
        exp->setCoords(le, exp->getY());
        exp->setW(wi);
        exp->setShowPart(le-tmp, 0, wi, exp->getH());
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
