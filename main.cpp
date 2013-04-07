#include <stdio.h>
#include "SDL.h"
#include "block.h"
#include "hero.h"
#include <vector>
#include "CollisionPair.h"
#include "background.h"
#include "Bomb.h"
#include "Explosion.h"
#include "enemy.h"
#include <algorithm>
#include "upgrade.h"
#include "SDL_ttf.h"
#include <map>
#include "hellomessage.h"
#include "slotmessage.h"
#include "bombmessage.h"

Hero * hero;
Bomb * bomb; vector<Block * > blocks;
vector<Enemy * > enemyGroup;
//vector<Hero *> heroGroup;
map<int, Hero*> heroGroup;
vector<Bomb *> bombGroup;
vector<Explosion *> explosionGroup;
vector<Upgrade *> upgradeGroup;
bool Connected;
CClientSocket* tcpclient;
CIpAddress* remoteip;
CUdpSocket* udpclient;
vector<CollisionPair * > colList;
int myId;
bool moved=false;
bool bombed=false;
Background * background;
SDL_Surface *text_image;
char textbuf[80];
SDL_Rect textDest;
TTF_Font *text_font;
SDL_Color font_color;

void handleServer();
void handleNetwork();
void handleClients();

int handle_key(SDLKey k) {
    switch(k) {
        case SDLK_ESCAPE:
            return 1;
        case SDLK_LEFT:
			moved=true;
            heroGroup[myId]->moveLeft();
            break;
        case SDLK_RIGHT:
			moved=true;
            heroGroup[myId]->moveRight();
            break;
		case SDLK_UP:
			moved=true;
			heroGroup[myId]->moveUp();
			break;
		case SDLK_DOWN:
			moved=true;
			heroGroup[myId]->moveDown();
			break;
    }
    return 0;
}
void handle_keyup(SDLKey k) {
    switch (k) {
        case SDLK_LEFT:
			heroGroup[myId]->stopMoving();
			break;
        case SDLK_RIGHT:
			heroGroup[myId]->stopMoving();
			break;
		case SDLK_UP:
			heroGroup[myId]->stopMoving();
			break;
		case SDLK_DOWN:
            heroGroup[myId]->stopMoving();
            break;
        case SDLK_SPACE:
			bombed=true;
            heroGroup[myId]->placeBomb();
            break;
    }

}


void initBlock() {
    int locationX[5] = {100, 200, 300, 400, 500};
    int locationY[5] = {450, 350, 250, 150, 50};
    for (int i = 0; i < 5; i++){
        Block * tmp = new Block(locationX[i], locationY[i], false);
        blocks.push_back(tmp);
    }
    Block * tmp = new Block(600, 500, true);
    blocks.push_back(tmp);
}

void initEnemy() {
    int locationX[2] = {20, 700};
    int locationY[2] = {20, 550};
    for (int i = 0; i < 2; i++){
        Enemy * tmp = new Enemy(locationX[i], locationY[i]);
        enemyGroup.push_back(tmp);
    }
}

void sendHello()
{
	hellomessage hello;
	Uint16 port = udpclient->getPort();
	hello.LoadByte(port,0,0);
	tcpclient->Send(hello);
}

void init(string ip)
{
    tcpclient = new CClientSocket();
	remoteip = new CIpAddress(ip, 1234);
	Uint16 port=1234;
	udpclient=new CUdpSocket(port);
	if(udpclient==NULL)
	{
		cout<<"couldn't create udp endpoint\n";
		exit(EXIT_FAILURE);
	}
    while(1)
    {
        if(!Connected)
	    {
		    if(tcpclient->Connect(*remoteip))
		    {
			    if(tcpclient->Ok()){
			    	Connected=true;
			    	cout<<"connected to server"<<endl;
			    }
		    }
		}
		else{
				 break;
			}
	}
	sendHello();
    text_font =  TTF_OpenFont("fonts/FreeSerif.ttf", 20);    
    if (text_font == NULL) {
        printf("Could not load font\n");
        exit(1);
    }

    font_color.r = 0;
    font_color.g = 0;  //very green.  If you want black, make this 0.
    font_color.b = 0;


    
    background = new Background("img/background.bmp");
    initBlock();
    initEnemy();
    background->setCoords(0,0);
    
    int totalScroll =0;
    
    //preprocess collision

//    heroGroup.push_back(hero);
  //  heroGroup[0] = hero;
  
    for (int j = 0; j < bombGroup.size(); j++){
//        for (int i = 0; i < heroGroup.size(); i++) {
        for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
            CollisionPair * cp = new CollisionPair(it->second, bombGroup.at(j), HeroBomb);
            colList.push_back(cp);
        }
       
    }
//    for (int j = 0; j < enemyGroup.size(); j++){
//        for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
//            CollisionPair * cp = new CollisionPair(it->second, enemyGroup.at(j), HeroEnemy);
//            colList.push_back(cp);
//        }
//        
//    }
}

void handleNetwork()
{
	if(tcpclient->Ready())
	{
		handleServer();
	}
	if(udpclient->Ready())
		handleClients();
}

void handleServer()
{
	slotmessage msg;
	if(tcpclient->Receive(msg))
	{


		int id;
		int ch;
		int x;
		msg.UnLoadByte(ch, id, x);
		if(ch=='2')
		{
			delete heroGroup[id];
			heroGroup.erase(id);
			return;
		}
		Hero* newhero = new Hero();
		newhero->setPlayerId(id);
		heroGroup[id]=newhero;
		heroGroup[id]->setCoords(0, 0);
		if(ch=='0')
			myId=id;
        for (int j = 0; j < enemyGroup.size(); j++){
                CollisionPair * cp = new CollisionPair(heroGroup[id], enemyGroup.at(j), HeroEnemy);
                colList.push_back(cp);
            
        }
	}

}

void handleClients()
{
	CNetMessage* msg;
	int channel;
	if(udpclient->Receive(msg, channel))
	{
		if(msg->getType()=='h')
		{
			hero_pos pos;
			msg->UnLoadByte(pos.x, pos.y, pos.id);
			if(pos.id!=myId)
			{
                int oriX = heroGroup[pos.id]->getX();
                int oriY = heroGroup[pos.id]->getY();
				heroGroup[pos.id]->setCoords(pos.x, pos.y);
			}
		}else if(msg->getType()=='b')
		{
			int bx, by, lvl;
			msg->UnLoadByte(bx, by, lvl);
			cout<<"bomb msg received at:"<<bx<<" "<<by<<endl;
			Bomb* newbomb=new Bomb("img/items/bomb_3.bmp", bx, by,4000,SDL_GetTicks(),lvl);
		    bombGroup.push_back(newbomb);
		}
	}	
}

void eventLoop(SDL_Surface * screen) {
    SDL_Event event;
    

        
    while(1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if(handle_key(event.key.keysym.sym)){
                        delete background;
//                        delete heroGroup[myId];
                        for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
                            delete it->second;
                        }
                        for (int i = 0; i < blocks.size(); i++)
                            delete blocks.at(i);
                        for (int i = 0; i < explosionGroup.size(); i++)
                            delete explosionGroup.at(i);
                        for (int i = 0; i < enemyGroup.size(); i++)
                            delete enemyGroup.at(i);
                        for (int i = 0; i < bombGroup.size(); i++)
                            delete bombGroup.at(i);
                        for (int i = 0; i < upgradeGroup.size(); i++)
                            delete upgradeGroup.at(i);
                        for (int i = 0; i < colList.size(); i++) {
                            delete colList.at(i);
                        }
                        TTF_CloseFont(text_font);
//                        SDL_FreeSurface(text_image);
                        return;
                    }
                    break;
                case SDL_KEYUP:
                    handle_keyup(event.key.keysym.sym);
                    break;
            }
            
        }/* input event loop*/

        
        //update sprites
        background->update(blocks, colList, heroGroup, upgradeGroup);
        handleNetwork();

        

		std::map<int, Hero*>::iterator it;
		for(it=heroGroup.begin();it!=heroGroup.end();++it)
		{
			it->second->update(blocks, colList, heroGroup, bombGroup, explosionGroup, enemyGroup);
		}
		if(moved){
			hero_pos newPos=heroGroup[myId]->getPos();
			heromessage msg;
			msg.LoadByte(newPos.x, newPos.y, newPos.id);
			udpclient->Send(msg,tcpclient->getIpAddress() ,-1);
			moved=false;
		}
		if(bombed)
		{
			int bombx, bomby, level;
			bombx=heroGroup[myId]->getBombx();
			bomby=heroGroup[myId]->getBomby();
			level=heroGroup[myId]->getBombLevel();
			bombmessage bmsg;
			bmsg.LoadByte(bombx, bomby, level);
			udpclient->Send(bmsg, tcpclient->getIpAddress(), -1);
			bombed=false;
		}
        for (int i = 0; i < enemyGroup.size(); i++) {
            enemyGroup.at(i)->update(blocks, colList, heroGroup, bombGroup, explosionGroup);
        }

        for (int i = 0; i < bombGroup.size(); i++) {
            bombGroup.at(i)->update(blocks, colList, heroGroup, bombGroup, explosionGroup, enemyGroup);
        }
        for (int i = 0; i < explosionGroup.size(); i++) {
            explosionGroup.at(i)->update();
        }
        for (int i = 0; i < upgradeGroup.size(); i++) {
            upgradeGroup.at(i)->update();
        }

        //check for collision
        for (int i = 0; i < colList.size(); i++){
            CollisionPair * tmp = colList.at(i);
            if (tmp->isCollided()){
                tmp->onCollision();
            }
        }
        
        //draw sprites
        background->blit(screen);
 	for (int j = 0; j < bombGroup.size(); j++) {
            bombGroup.at(j)->blit(screen);
        }
        for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
            it->second->blit(screen);
        }
        for (int j = 0; j < explosionGroup.size(); j++) {
            explosionGroup.at(j)->blit(screen);
        }
        for (int i = 0; i < blocks.size(); i++) {
            blocks.at(i)->blit(screen);
        }
        for (int i = 0; i < enemyGroup.size(); i++) {
            enemyGroup.at(i)->blit(screen);
        }
        for (int i = 0; i < upgradeGroup.size(); i++) {
            upgradeGroup.at(i)->blit(screen);
        }
        
        
        //HUD
        sprintf(textbuf, "Hero Life = %d", heroGroup[myId]->getLife());
        text_image =  TTF_RenderText_Solid(text_font, textbuf, font_color);
        textDest.x = 10;
        textDest.y = 10;
        textDest.w = text_image->w;
        textDest.h = text_image->h;
        SDL_BlitSurface(text_image, NULL, screen, &textDest);
        SDL_FreeSurface(text_image);

        /* since its double buffered, make
         the changes show up*/
        SDL_Flip(screen);
        /* Wait 10 ms between frames*/
        SDL_Delay(50);
    }
    
}



int main(int argc, char* argv[]) {
    /* Initalize SDL - for this demo,
     we only are using the video stuff..
     if you are doing sound, you would do
     SDL_INIT_VIDEO | SDL_INIT_AUDIO
     you can also do timers, cdrom, joystick-
     see man page :)
     */
    SDL_Init(SDL_INIT_VIDEO );
	SDLNet_Init();
    TTF_Init();
    SDL_Surface * screen = SDL_SetVideoMode(WINDOW_WIDTH,
                                            WINDOW_HEIGHT,
                                            0,
                                            SDL_HWSURFACE |
                                            SDL_DOUBLEBUF
                                            );
    /* make it so that holding down a key repeats it*/
    SDL_EnableKeyRepeat(10,10);
    
    if(screen == NULL) {
        cout<<SDL_GetError()<<endl;
        fprintf(stderr, "Can't inialtize video\n");
        SDL_Quit();
        return EXIT_FAILURE;
    }
    /* Set the screen resolution: 1024x768, 32 bpp
     We also want to do full screen, double-buffered,
     and have the surface in video hardware */
    init(argv[1]);
    eventLoop(screen);
    /* cleanup SDL- return to normal screen mode,
     etc */
    SDL_Quit();
    TTF_Quit();

    
    return EXIT_SUCCESS;
}
