#include <stdio.h>
#include "SDL.h"
#include "SDL/SDL_mixer.h"
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
#include "blockmessage.h"
#include <stdlib.h>     /* srand, rand */


Hero * hero;
int max_players=-1;
Bomb * bomb; vector<Block * > blocks;
vector<Enemy * > enemyGroup;
//vector<Hero *> heroGroup;
map<int, Hero*> heroGroup;
vector<Bomb *> bombGroup;
vector<Explosion *> explosionGroup;
vector<Upgrade *> upgradeGroup; bool Connected;
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
bool start=false;

void handleServer();
void handleNetwork();
void handleClients();

int handle_key(SDLKey k) {
	if(!start)
		return 0;
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
    if(!start)
	return ;

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
			//bombed=true;
            heroGroup[myId]->placeBomb();
	    bombed = heroGroup[myId]->getIsBomb();
            break;
    }

}

//#define UNIT 50

void initBlock() {
    bool initialized =false;

    for (int j=0; j<WINDOW_WIDTH/UNIT; j++) {
        int x = j*UNIT;
        int y = 2*UNIT;
        blocks.push_back(new Block(x, y, true,3));
        y = (WINDOW_HEIGHT/UNIT-1)*UNIT;
        blocks.push_back(new Block(x, y, true,3));
    }
    for (int i=3; i<WINDOW_HEIGHT/UNIT-1; i++) {
        int x = 0;
        int y = i*UNIT;
        blocks.push_back(new Block(x, y, true,3));
        x = (WINDOW_WIDTH/UNIT-1)*UNIT;
        blocks.push_back(new Block(x, y, true,3));
    }
    for (int i=2; i<WINDOW_WIDTH/UNIT-1; i+=2){
        for (int j=4; j<WINDOW_HEIGHT/UNIT-1; j+=2){
            blocks.push_back(new Block(i*50, j*50, true,3));

        }
    }
    blockmessage bmsg;
    while(1)
    {
        if(tcpclient->Ready())
        {
            if(tcpclient->Receive(bmsg))
            {
                int x, y, r;
                bmsg.UnLoadByte(x, y, r);
                if(r==9)
                    break;
                blocks.push_back(new Block(x*50, y*50, false, r));
            }
        }
        
    }
}

void initEnemy() {
    int locationX[2] = {6*UNIT, 7*UNIT};
    int locationY[2] = {9*UNIT, 4*UNIT};
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
	initBlock();
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
    initEnemy();
    background->setCoords(0,0);
    
    /*Mix_Chunk * placeBomb = Mix_LoadWAV("sound/placebomb.bmp");
    if (placeBomb == NULL) {
	printf("Could not load sound effect\n");
	exit(1);
    }*/

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
		max_players=x;
		cout<<"max players: "<<max_players<<endl;
		if(ch=='2')
		{
			delete heroGroup[id];
			heroGroup.erase(id);
			return;
		}
		Hero* newhero = new Hero();
		newhero->setPlayerId(id);
		heroGroup[id]=newhero;
        switch (id) {
            case 0:
                heroGroup[id]->setCoords(UNIT, 3*UNIT);
                break;
            case 1:
                heroGroup[id]->setCoords((WINDOW_WIDTH/UNIT-2)*UNIT, (WINDOW_HEIGHT/UNIT-2)*UNIT);
                break;
            case 2:
                heroGroup[id]->setCoords((WINDOW_WIDTH/UNIT-2)*UNIT, 3*UNIT);
                break;
            case 3:
                heroGroup[id]->setCoords(UNIT, (WINDOW_HEIGHT/UNIT-2)*UNIT);
                break;
            default:
                break;
        }
//		heroGroup[id]->setCoords(UNIT, 3*UNIT);
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
                int frame = heroGroup[pos.id]->getFrame();
                if (pos.y<oriY){
                    frame++;
                    if (frame>2)
                        frame = 0;
                    heroGroup[pos.id]->setAnimFrame(frame+6);
                }
                else if (pos.x>oriX){
                    frame++;
                    if (frame>2)
                        frame = 0;
                    heroGroup[pos.id]->setAnimFrame(frame+3);
                }
                else if (pos.y>oriY){
                    frame++;
                    if (frame>2)
                        frame = 0;
                    heroGroup[pos.id]->setAnimFrame(frame);
                }
                else if (pos.x<oriX){
                    frame++;
                    if (frame>2)
                        frame = 0;
                    heroGroup[pos.id]->setAnimFrame(frame+9);
                }
                heroGroup[pos.id]->setFrame(frame);
			}
		}else if(msg->getType()=='b')
		{
			int bx, by, lvl;
			msg->UnLoadByte(bx, by, lvl);
//			cout<<"bomb msg received at:"<<bx<<" "<<by<<endl;
			Bomb* newbomb=new Bomb(bx, by, 4000, SDL_GetTicks(), lvl);
		    bombGroup.push_back(newbomb);
		}
	}	
	delete msg;
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
		if(heroGroup.size()==max_players)
			start=true;
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
        for (int i = 0; i < blocks.size(); i++) {
            blocks[i]->update(colList, heroGroup, explosionGroup, upgradeGroup);
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
        for (int j = 0; j < explosionGroup.size(); j++) {
            SDL_Rect tmp = explosionGroup.at(j)->getShowPart();
            explosionGroup.at(j)->blit(screen, &tmp);
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
 	for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
            it->second->blit(screen);
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
        /* Wait 50 ms between frames*/
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
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
	fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
	exit(1);
    }
    
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
