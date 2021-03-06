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
#include <algorithm>
#include "upgrade.h"
#include "SDL_ttf.h"
#include <map>
#include "hellomessage.h"
#include "slotmessage.h"
#include "bombmessage.h"
#include "blockmessage.h"
#include <stdlib.h>     /* srand, rand */

#define MULTIPLE 1
#define EXIT 2
#define MENU 3

Hero * hero;
int max_players=-1;
Bomb * bomb; vector<Block * > blocks;
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
SDL_Surface *text_image[5];
char textbuf[5][80];
SDL_Rect textDest[5];
TTF_Font *text_font;
SDL_Color font_color;
bool start=false;
Mix_Music *menuMusic = NULL;
Mix_Chunk *menuSFX = NULL;
Mix_Music *mainMusic = NULL;
Background * winScreen;
Background * loseScreen;

void handleServer();
void handleNetwork();
void handleClients();

int handle_key(SDLKey k) {
	if(!start)
		return 0;
    switch(k) {
            //        case SDLK_ESCAPE:
            //            return 1;
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
            //        case SDLK_d:
            //            return MENU;
    }
    return 0;
}


int handle_keyup(SDLKey k) {
    
	if(!start)
		return 0;
    
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
        case SDLK_ESCAPE:
            Mix_HaltMusic();
            return MENU;
            
    }
    return 0;
}

int handle_menu_key(SDLKey k, int & arrowPos) {
    switch(k) {
        case SDLK_ESCAPE:
            return EXIT;
        case SDLK_RETURN:
            Mix_HaltMusic();
            if (arrowPos==0)
                return MULTIPLE;
            if (arrowPos==1)
                return EXIT;
            break;
            
    }
    return 0;
}
int handle_menu_keyup(SDLKey k, int & arrowPos) {
    switch(k) {
        case SDLK_UP:
            if (Mix_PlayChannel(-1, menuSFX, 0) == -1)
                fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
            arrowPos--;
            if (arrowPos<0)
                arrowPos = 1;
            break;
        case SDLK_DOWN:
            if (Mix_PlayChannel(-1, menuSFX, 0) == -1)
                fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
            arrowPos++;
            if(arrowPos>1)
                arrowPos = 0;
            break;
        case SDLK_SPACE:
            Mix_HaltMusic();
            if (arrowPos==0)
                return MULTIPLE;
            if (arrowPos==1)
                return EXIT;
            break;
    }
    return 0;
}

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

//void initEnemy() {
//    int locationX[2] = {6*UNIT, 7*UNIT};
//    int locationY[2] = {9*UNIT, 4*UNIT};
//    for (int i = 0; i < 2; i++){
//        Enemy * tmp = new Enemy(locationX[i], locationY[i]);
//        enemyGroup.push_back(tmp);
//    }
//}

void sendHello()
{
	hellomessage hello;
	Uint16 port = udpclient->getPort();
	hello.LoadByte(port,0,0);
	tcpclient->Send(hello);
}

void initFont() {
    text_font =  TTF_OpenFont("fonts/FreeSerif.ttf", 20);
    if (text_font == NULL) {
        printf("Could not load font\n");
        exit(1);
    }
    
    font_color.r = 0;
    font_color.g = 0;  //very green.  If you want black, make this 0.
    font_color.b = 0;
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
    
    background = new Background("img/background.bmp");
    
    background->setCoords(0,0);
    
    winScreen = new Background("img/win.bmp");
    winScreen->setCoords(0,0);
    loseScreen = new Background("img/lose.bmp");
    loseScreen->setCoords(0,0);
    
    
    mainMusic = Mix_LoadMUS("sound/mainbgm.wav");
    
    int totalScroll =0;
}

void handleNetwork()
{
	while(!start){
        if(tcpclient->Ready())
        {
            
            handleServer();
        }
        if(heroGroup.size()==max_players)
            start=true;
	}
	if(udpclient->Ready()){
		handleClients();
    }
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
		Hero* newhero = new Hero(id);
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
        
		if(ch=='0')
			myId=id;
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
			Bomb* newbomb=new Bomb(bx, by, 4000, SDL_GetTicks(), lvl);
		    bombGroup.push_back(newbomb);
		}
        
	}
	delete msg;
}

void gameLoopCleanUp() {
    delete background;
    while(!colList.empty()) {
        CollisionPair * tmp = colList.back();
        colList.pop_back();
        delete tmp;
    }
    cout<<"collist"<<colList.size()<<endl;
    for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
        delete it->second;
        heroGroup.erase(it);
    }
    //                        while (!heroGroup.empty()){
    //                            Hero * tmp = heroGroup[0];
    //                            heroGroup.erase(0);
    //                            delete tmp;
    //                        }
    cout<<"hero"<<heroGroup.size()<<endl;
    
    while(!blocks.empty()){
        Block * tmp = blocks.back();
        blocks.pop_back();
        delete tmp;
    }
    cout<<"block"<<blocks.size()<<endl;
    
    while(!explosionGroup.empty()){
        Explosion * tmp = explosionGroup.back();
        explosionGroup.pop_back();
        delete tmp;
    }
    cout<<"exp"<<explosionGroup.size()<<endl;
    
    //                        while(!enemyGroup.empty()){
    //                            Enemy * tmp = enemyGroup.back();
    //                            enemyGroup.pop_back();
    //                            delete tmp;
    //                        }
    //                        cout<<"enemy"<<enemyGroup.size()<<endl;
    
    while(!bombGroup.empty()){
        Bomb * tmp = bombGroup.back();
        bombGroup.pop_back();
        delete tmp;
    }
    cout<<"bomb"<<bombGroup.size()<<endl;
    
    while(!upgradeGroup.empty()){
        Upgrade * tmp = upgradeGroup.back();
        upgradeGroup.pop_back();
        delete tmp;
    }
    cout<<"up"<<upgradeGroup.size()<<endl;
    
    TTF_CloseFont(text_font);
    delete tcpclient;
    delete udpclient;
    delete remoteip;
    Mix_HaltMusic();
    Mix_FreeMusic(mainMusic);
    delete winScreen;
    delete loseScreen;
}

int eventLoop(SDL_Surface * screen) {
    SDL_Event event;
    int animationCounter = 0;
    while(1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYUP:
                    if(handle_keyup(event.key.keysym.sym)==MENU){
                        gameLoopCleanUp();
                        return MENU;
                    }
                    break;
                case SDL_KEYDOWN:
                    handle_key(event.key.keysym.sym);
                    break;
            }
            
            
        }
        
        handleNetwork();
        
        std::map<int, Hero*>::iterator it;
		for(it=heroGroup.begin();it!=heroGroup.end();++it)
		{
			if (it->second->getVisible())
                it->second->update(blocks, colList, heroGroup, bombGroup, explosionGroup);
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
        
        for (int i = 0; i < bombGroup.size(); i++) {
            bombGroup.at(i)->update(blocks, colList, heroGroup, bombGroup, explosionGroup);
        }
        for (int i = 0; i < explosionGroup.size(); i++) {
            explosionGroup.at(i)->update();
        }
        for (int i = 0; i < upgradeGroup.size(); i++) {
            upgradeGroup.at(i)->update();
        }
        
        for (int i = 0; i < colList.size(); i++){
            CollisionPair * tmp = colList.at(i);
            if (tmp->isCollided()){
                tmp->onCollision();
            }
        }
        
        background->blit(screen);
        
        if (Mix_PlayingMusic() == 0) {
            if (Mix_PlayMusic(mainMusic, -1) == -1)
                fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
        }
        
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
        
        for (int i = 0; i < upgradeGroup.size(); i++) {
            upgradeGroup.at(i)->blit(screen);
        }
        for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
            it->second->blit(screen);
        }
        
        for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
            sprintf(textbuf[it->first], "Player %d = %d", it->first+1, it->second->getLife());
            text_image[it->first] =  TTF_RenderText_Solid(text_font, textbuf[it->first], font_color);
            int offsetX = it->first<2 ? 0:1;
            int offsetY = it->first%2==0 ? 0:1;
            textDest[it->first].x = 50+750*offsetX;
            textDest[it->first].y = 10+50*offsetY;
            textDest[it->first].w = text_image[it->first]->w;
            textDest[it->first].h = text_image[it->first]->h;
            SDL_BlitSurface(text_image[it->first], NULL, screen, &(textDest[it->first]));
            SDL_FreeSurface(text_image[it->first]);
        }
        
        
        // game over
        bool iWin = false;
        if (heroGroup[myId]->getLife()!=0){
            iWin = true;
            for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
                if (it->first != myId){
                    if (it->second->getLife() != 0){
                        iWin = false;
                        break;
                    }
                }
            }
        }
        bool iLose=false;
        if (heroGroup[myId]->getLife()==0){
            iLose = true;
            int counter = 0;
            for(map<int, Hero* >::iterator it=heroGroup.begin(); it!=heroGroup.end(); ++it) {
                if (it->first != myId){
                    if (it->second->getLife() != 0){
                        counter++;
                        if (counter>1) {
                            iLose = false;
                            break;
                        }
                    }
                }
            }
        }
        
        if (iWin) {
            animationCounter++;
            if (animationCounter>20){
                winScreen->blit(screen);
                SDL_Flip(screen);
                SDL_Delay(3000);
                gameLoopCleanUp();
                return MENU;
            }
        }
        else if (iLose){
            animationCounter++;
            if (animationCounter>20){
                loseScreen->blit(screen);
                SDL_Flip(screen);
                SDL_Delay(3000);
                gameLoopCleanUp();
                return MENU;
            }
        }
        
        /* since its double buffered, make
         the changes show up*/
        SDL_Flip(screen);
        
        SDL_Delay(50);
    }
    return 0;
}

int menu(SDL_Surface * screen) {
    SDL_Event event;
    Background * menuBackground = new Background("img/menu/menu.bmp");
    menuBackground->setCoords(0,0);
    Background * arrow = new Background("img/menu/arrow.bmp");
    menuMusic = Mix_LoadMUS("sound/menubgm.wav");
    menuSFX = Mix_LoadWAV("sound/arrow.wav");
    int arrowX = 250;
    int arrowY = 515;
    arrow->setCoords(arrowX,arrowY);
    int arrowPos = 0;
    int step = 83;
    while(1) {
        int stage;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    stage = handle_menu_key(event.key.keysym.sym, arrowPos);
                    break;
                case SDL_KEYUP:
                    stage = handle_menu_keyup(event.key.keysym.sym, arrowPos);
                    break;
            }
            if (stage == EXIT){
                delete menuBackground;
                delete arrow;
                Mix_FreeChunk(menuSFX);
                Mix_HaltMusic();
                Mix_FreeMusic(menuMusic);
                menuMusic = NULL;
                return EXIT;
                
            }
            else if (stage == MULTIPLE){
                delete menuBackground;
                delete arrow;
                Mix_FreeChunk(menuSFX);
                Mix_HaltMusic();
                Mix_FreeMusic(menuMusic);
                menuMusic = NULL;
                return MULTIPLE;
                
            }
            
        }
        
        arrow->setCoords(arrowX, arrowY+arrowPos*step);
        menuBackground->setCoords(0,0);
        
        menuBackground->blit(screen);
        arrow->blit(screen);
        
        if (Mix_PlayingMusic() == 0) {
            if (Mix_PlayMusic(menuMusic, -1) == -1)
                fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
        }
        
        SDL_Flip(screen);
        
        SDL_Delay(10);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    
    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;
    
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
        exit(1);
    }
    
    SDL_Surface * screen = SDL_SetVideoMode(WINDOW_WIDTH,
                                            WINDOW_HEIGHT,
                                            32,
                                            SDL_HWSURFACE |
                                            SDL_DOUBLEBUF
                                            );
    
    SDL_EnableKeyRepeat(10,10);
    
    if(screen == NULL) {
        cout<<SDL_GetError()<<endl;
        fprintf(stderr, "Can't inialtize video\n");
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    int stage = menu(screen);
    
    while (stage != EXIT) {
        if (stage == MENU)
            stage = menu(screen);
        if (stage == MULTIPLE){
            SDLNet_Init();
            start=false;
            init(argv[1]);
            TTF_Init();
            
            initFont();
            stage = eventLoop(screen);
            SDLNet_Quit();
            Connected = false;
            TTF_Quit();
            
        }
        
    }
    
    Mix_CloseAudio();
    
    Mix_Quit();
    
    SDL_Quit();
    
    
    
    return EXIT_SUCCESS;
    
    
}
