#include "SDL.h"
#include "block.h"
#include "hero.h"
#include <vector>
#include "CollisionPair.h"
#include "background.h"
#include "Bomb.h"
#include "Explosion.h"

Hero * hero;
Bomb * bomb;
vector<Block * > blocks;
bool Connected;
CClientSocket* tcpclient;
CIpAddress* remoteip;



int handle_key(SDLKey k) {
    switch(k) {
        case SDLK_ESCAPE:
            return 1;
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
        case SDLK_SPACE:
            hero->placeBomb();
            break;
    }
}

//vector<SDL_Rect> initBlock(Hero * hero) {
//    int locationX[5] = {100, 200, 300, 400, 500};
//    int locationY[5] = {450, 350, 250, 150, 50};
//    for (int i = 0; i < 5; i++){
//        Block * tmp = new Block();
//        tmp->setCoords(locationX[i], locationY[i]);
//        blocks.push_back(tmp);
//    }
//    vector<SDL_Rect> blockMap;
//    SDL_Rect rectArray[5];
//    for (int i = 0; i < blocks.size(); i++){
//        Block *tmp = blocks.at(i);
//        int x = tmp->getX() - hero->getW();
//        if (x<0)    x = 0;
//        int y = tmp->getY() - hero->getH();
//        if (y<0)    y = 0;
//        int w = tmp->getW() + hero->getW();
//        if (w>WINDOW_WIDTH)    w = WINDOW_WIDTH;
//        int h = tmp->getH() + hero->getH();
//        if (h>WINDOW_HEIGHT)    h = WINDOW_HEIGHT;
//        rectArray[i].x = x;
//        rectArray[i].y = y;
//        rectArray[i].w = w;
//        rectArray[i].h = h;
//        blockMap.push_back(rectArray[i]);
//    }
//    
//    return blockMap;
//}

vector<SDL_Rect> initBlock(Hero * hero) {
    int locationX[5] = {100, 200, 300, 400, 500};
    int locationY[5] = {450, 350, 250, 150, 50};
    for (int i = 0; i < 5; i++){
        Block * tmp = new Block();
        tmp->setCoords(locationX[i], locationY[i]);
        blocks.push_back(tmp);
    }
    vector<SDL_Rect> blockMap;
    SDL_Rect rectArray[5];
    for (int i = 0; i < blocks.size(); i++){
        Block *tmp = blocks.at(i);
        int x = tmp->getX();
//        if (x<0)    x = 0;
        int y = tmp->getY();
//        if (y<0)    y = 0;
        int w = tmp->getW();
//        if (w>WINDOW_WIDTH)    w = WINDOW_WIDTH;
        int h = tmp->getH();
//        if (h>WINDOW_HEIGHT)    h = WINDOW_HEIGHT;
        rectArray[i].x = x;
        rectArray[i].y = y;
        rectArray[i].w = w;
        rectArray[i].h = h;
        blockMap.push_back(rectArray[i]);
    }
    
    return blockMap;
}

void eventLoop(SDL_Surface * screen) {
    SDL_Event event;
//    hero = new Hero();
    bomb = new Bomb("img/blob2.bmp", 200, 450, 4000, SDL_GetTicks());
    Background * background = new Background("img/background.bmp");
    vector<SDL_Rect> blockMap = initBlock(hero);

    background->setCoords(0,0);
    int totalScroll =0;
    
    //preprocess collision
//    vector<vector<Sprite *> > colGroups;
    vector<Hero *> heroGroup;
    vector<Bomb *> bombGroup;
    vector<Explosion *> explosionGroup;
    heroGroup.push_back(hero);
    bombGroup.push_back(bomb);
//    colGroups.push_back(heroGroup);
//    colGroups.push_back(bombGroup);
    vector<CollisionPair * > colList;
    for (int i = 0; i < heroGroup.size(); i++){
        for (int j = 0; j < bombGroup.size(); j++) {
            CollisionPair * cp = new CollisionPair(heroGroup.at(i), bombGroup.at(j), HeroBomb);
            colList.push_back(cp);
        }
    }
        
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


		if(!Connected)
		{
			if(tcpclient->Connect(*remoteip))
			{
				if(tcpclient->Ok()){
					Connected=true;
					cout<<"connected to server"<<endl;
				}
			}
			else{
				if(tcpclient->Ready())
				{
					cout<<"ready"<<endl;
				}
			}
		}
        
        //update sprites
        hero->update(blocks, colList, heroGroup, bombGroup);
//        bomb->update();
//        bombGroup = colGroups.at(1);
        for (int i = 0; i < bombGroup.size(); i++) {
            bombGroup.at(i)->update(blocks, colList, heroGroup, explosionGroup);
        }
        for (int i = 0; i < explosionGroup.size(); i++) {
            explosionGroup.at(i)->update();
        }
//        for (int i = 0; i < colGroups.size(); i++) {
//            vector<Sprite *> curGroup = colGroups.at(i);
//            for (int j = 0; j < curGroup.size(); j++) {
//                curGroup.at(j)->update(blockMap, colList, colGroups);
//            }
//        }
        //check for collision
        for (int i = 0; i < colList.size(); i++){
            CollisionPair * tmp = colList.at(i);
            if (tmp->isCollided()){
                tmp->onCollision();
            }
        }
        
        //draw sprites
        background->blit(screen);
//        for (int i = 0; i < colGroups.size(); i++) {
//            vector<Sprite *> curGroup = colGroups.at(i);
//            for (int j = 0; j < curGroup.size(); j++) {
//                curGroup.at(j)->blit(screen);
//            }
//        }
        for (int j = 0; j < heroGroup.size(); j++) {
            heroGroup.at(j)->blit(screen);
        }
        for (int j = 0; j < bombGroup.size(); j++) {
            bombGroup.at(j)->blit(screen);
        }
        for (int j = 0; j < explosionGroup.size(); j++) {
//            Explosion * cur = explosionGroup.at(j);
//            SDL_Rect tmp;
//            tmp.x = cur->getW()/2;
//            tmp.y = 0;
//            tmp.w = 500;
//            tmp.h = 500;
            explosionGroup.at(j)->blit(screen);
        }
        for (int i = 0; i < blocks.size(); i++) {
            blocks.at(i)->blit(screen);
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
	hero=new Hero();
	tcpclient = new CClientSocket();
	hero->setclient(tcpclient);
	remoteip = new CIpAddress("127.0.1.1", 1234);
    /* Set the screen resolution: 1024x768, 32 bpp
     We also want to do full screen, double-buffered,
     and have the surface in video hardware */
    SDL_Surface * screen = SDL_SetVideoMode(WINDOW_WIDTH,
                                            WINDOW_HEIGHT,
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
    
//    std::pair <int,int> foo;
//    std::pair <int,int> bar;
//    set<pair<int,int> > ss;
//    foo = std::make_pair (10,20);
//    ss.insert(foo);
//    bar = std::make_pair (10,20);
//    if (ss.find(bar)!=ss.end())
//        cout<<"YES";
    
    return EXIT_SUCCESS;
}
