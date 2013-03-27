#include "SDL.h"
#include "block.h"
#include "hero.h"
#include <vector>
#include "CollisionPair.h"
#include "background.h"
#include "Bomb.h"

Hero * hero;
Bomb * bomb;
vector<Block * > blocks;

int handle_key(SDLKey k) {
    switch(k) {
        case SDLK_ESCAPE:
            return 1;
        case SDLK_SPACE:
            hero->jump();
            break;
        case SDLK_LEFT:
            hero->moveLeft();
            break;
        case SDLK_RIGHT:
            hero->moveRight();
            break;
		case SDLK_UP:
			hero->moveUp();
			break;
		case SDLK_DOWN:
			hero->moveDown();
			break;
    }
    return 0;
}
void handle_keyup(SDLKey k) {
    switch (k) {
        case SDLK_LEFT:
        case SDLK_RIGHT:
		case SDLK_UP:
		case SDLK_DOWN:
            hero->stopMoving();
            break;
    }
}

void initBlock() {
    int locationX[5] = {100, 200, 300, 400, 500};
    int locationY[5] = {450, 350, 250, 150, 50};
    for (int i = 0; i < 5; i++){
        Block * tmp = new Block();
        tmp->setCoords(locationX[i], locationY[i]);
        blocks.push_back(tmp);
    }
}

void eventLoop(SDL_Surface * screen) {
    SDL_Event event;
    hero = new Hero();
    bomb = new Bomb();
    Background * background = new Background("img/background.bmp");
    initBlock();
    background->setCoords(0,0);
    int totalScroll =0;
    
    //preprocess collision
    vector<CollisionPair * > colList;
    CollisionPair * cp = new CollisionPair(hero, bomb, 0);
    colList.push_back(cp);
        
    while(1) {
        /* This function returns 0 if no
         events are pending, 1 (and fills in event)
         if one is*/
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if(handle_key(event.key.keysym.sym)){
                        return;
                    }
                    break;
                case SDL_KEYUP:
                    handle_keyup(event.key.keysym.sym);
                    break;
            }
            
        }/* input event loop*/
        
        hero->update();
        bomb->update();
        int x = hero->getX();
        int scroll = (x - 500) / 25;
        
        
        
        //        hero->scrollingBy(scroll);
//        totalScroll += scroll;
//        totalScroll = totalScroll & 2047;
        
        //        background->setCoords(-totalScroll, 0);
        
        
        
        background->blit(screen);
        hero->draw(screen);
        if (bomb->getVisible())
            bomb->draw(screen);
        for (int i = 0; i < blocks.size(); i++) {
            blocks.at(i)->draw(screen);
        }
        
        //check for collision
        for (int i = 0; i < colList.size(); i++){
            CollisionPair * tmp = colList.at(i);
            if (tmp->isCollided()){
                tmp->onCollision();
            }
        }
        

        /* since its double buffered, make
         the changes show up*/
        SDL_Flip(screen);
        /* Wait 10 ms between frames*/
        SDL_Delay(10);
    }
}



int main(void) {
    /* Initalize SDL - for this demo,
     we only are using the video stuff..
     if you are doing sound, you would do
     SDL_INIT_VIDEO | SDL_INIT_AUDIO
     you can also do timers, cdrom, joystick-
     see man page :)
     */
    SDL_Init(SDL_INIT_VIDEO );
    /* Set the screen resolution: 1024x768, 32 bpp
     We also want to do full screen, double-buffered,
     and have the surface in video hardware */
    SDL_Surface * screen = SDL_SetVideoMode(800,
                                            640,
                                            0,
                                            SDL_HWSURFACE |
                                            SDL_DOUBLEBUF 
                                            );
    /* make it so that holding down a key repeats it*/
    SDL_EnableKeyRepeat(10,10);
    
    if(screen == NULL) {
        fprintf(stderr, "Can't inialtize video\n");
        SDL_Quit();
        return EXIT_FAILURE;
    }
    eventLoop(screen);
    /* cleanup SDL- return to normal screen mode,
     etc */
    SDL_Quit();
    return EXIT_SUCCESS;
}
