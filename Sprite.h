#ifndef __SPRITE_H__
#define __SPRITE_H__
#include "SDL.h"
#include "SDL/SDL_mixer.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;

enum colType{
    HeroBomb, HeroExplosion, BombExplosion, BlockExplosion, HeroEnemy, EnemyExplosion, BombEnemy, HeroUpgrade, HeroLife, HeroSpeed
};


#define WINDOW_WIDTH 950
#define WINDOW_HEIGHT 750
#define UNIT 50
#define OFFSET 15

class Sprite {
protected:
    int maxAnimFrames;
    SDL_Surface ** anim_frames;
    int currentFrame;
    SDL_Rect pos;
    bool visible;

public:
    ~Sprite() {
        int i;
        for (i = 0; i < maxAnimFrames; i++) {
            SDL_FreeSurface(anim_frames[i]);
        }
        delete[] anim_frames;
    }
    void setTransparent(){
	SDL_SetColorKey(anim_frames[0],
			SDL_SRCCOLORKEY | SDL_RLEACCEL,
			SDL_MapRGB(anim_frames[0]->format, 0, 255, 0));
    }
    void initSprite(const char * file){
        currentFrame = 0;
        maxAnimFrames = 1;
        anim_frames = new SDL_Surface*[1];
        anim_frames[0] = SDL_LoadBMP(file);
        pos.w = anim_frames[0]->w;
        pos.h = anim_frames[0]->h;
    }
    void initSprite(int frames, const char ** files){
        maxAnimFrames = frames;
        currentFrame = 0;
        int i;        
        anim_frames = new SDL_Surface*[frames];
        for (i = 0; i < frames; i++) {
            anim_frames[i] = SDL_LoadBMP(files[i]);
            SDL_SetColorKey(anim_frames[i],
                            SDL_SRCCOLORKEY | SDL_RLEACCEL,
                            SDL_MapRGB(anim_frames[i]->format , 0, 255, 0));
            
        }
        pos.w = anim_frames[0]->w;
        pos.h = anim_frames[0]->h;
    }
    void setAnimFrame(int x) {
        if ( x>=0 && x< maxAnimFrames) {
            currentFrame = x;
            pos.w = anim_frames[currentFrame]->w;
            pos.h = anim_frames[currentFrame]->h;
        }
    }
    int getAnimFrame() const{
        return currentFrame;
    }
    void setCoords(int x, int y) {
        pos.x = x;
        pos.y = y;
    }
    void getCoords(int & x, int &y) {
        x = pos.x;
        y = pos.y;
    }
    
    void blit (SDL_Surface * screen) {
        if (visible) {
            int x = pos.x;
            int y = pos.y;
            SDL_BlitSurface(anim_frames[currentFrame], NULL, screen, &pos);
            pos.x = x;
            pos.y = y;
        }
    }
    
    void blit (SDL_Surface * screen, SDL_Rect *srcRect) {
        if (visible) {
            int x = pos.x;
            int y = pos.y;
            SDL_BlitSurface(anim_frames[currentFrame], srcRect, screen, &pos);
            pos.x = x;
            pos.y = y;
        }
    }
    
    virtual bool isCollided(Sprite * other) {
        int top1 = pos.y;
        int left1 = pos.x;
        int bottom1 = top1 + pos.h;
        int right1 = left1 + pos.w;
        int top2 = other->getY();
        int left2 = other->getX();
        int bottom2 = top2 + other->getH();
        int right2 = left2 + other->getW();
        if (bottom1 <= top2+OFFSET) return false;
        if (top1 >= bottom2-OFFSET) return false;
        if (right1 <= left2+OFFSET) return false;
        if (left1 >= right2-OFFSET) return false;
        return true;
    }
    
    int getX(){
        return pos.x;
    }
    
    int getY(){
        return pos.y;
    }
    
    int getH(){
        return pos.h;
    }
    
    int getW(){
        return pos.w;
    }
    
    void setW(int wid) {
        pos.w = wid;
    }
    
    void setH(int hei) {
        pos.h = hei;
    }
    
    void setVisible(bool flag){
        visible = flag;
    }
    
    bool getVisible(){
        return visible;
    }
    
    virtual void inCollision(enum colType t) = 0;
};
#endif
