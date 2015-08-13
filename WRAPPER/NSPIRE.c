/*   COPYRIGHT (C) 2014-2015 GAMEBLABLA   Licensed under the Apache License, Version 2.0 (the "License");   you may not use this file except in compliance with the License.   You may obtain a copy of the License at       http://www.apache.org/licenses/LICENSE-2.0   Unless required by applicable law or agreed to in writing, software   distributed under the License is distributed on an "AS IS" BASIS,   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   See the License for the specific language governing permissions and   limitations under the License.*/#define FPS_VIDEO 60#define MAX_IMAGE 512#define MAX_PATH 128#include <os.h>#include <SDL/SDL.h>#ifdef IMAGE_CODEC_ENABLED	#include <SDL/SDL_image.h>#endif#include "INPUT.h"struct input BUTTON;unsigned short done = 0;const float real_FPS = 1000/FPS_VIDEO;char* game_name = "";SDL_Surface *sprites_img[MAX_IMAGE];unsigned short sprites_img_tocopy[MAX_IMAGE];SDL_Surface *screen;#define Buttons_UP KEY_NSPIRE_UP#define Buttons_LEFT KEY_NSPIRE_LEFT#define Buttons_RIGHT KEY_NSPIRE_RIGHT#define Buttons_DOWN KEY_NSPIRE_DOWN#define Buttons_A KEY_NSPIRE_DOC#define Buttons_B KEY_NSPIRE_DEL#define Buttons_C KEY_NSPIRE_VAR#define Buttons_D KEY_NSPIRE_CTRL#define Buttons_START KEY_NSPIRE_ENTER#define Buttons_SELECT KEY_NSPIRE_MENU#define Buttons_QUIT KEY_NSPIRE_ESCvoid Init_video(){	/*enable_relative_paths(argv);*/	SDL_Init( SDL_INIT_VIDEO );	screen = SDL_SetVideoMode(320, 240, has_colors ? 16 : 8, SDL_SWSURFACE);}void Load_Image(unsigned short a, const char* directory){	char final[MAX_PATH];	SDL_Surface *tmp;		sprintf(final, "./%s.tns", directory);		#ifdef DEBUG		fprintf(stderr, "Clearing surface for image %d...\n", a);	#endif		if (sprites_img[a] != NULL)	{		SDL_FreeSurface(sprites_img[a]);	}		#ifdef DEBUG		fprintf(stderr, "Trying to load image %d (%s)...\n", a, directory);	#endif	#ifdef IMAGE_CODEC_ENABLED		tmp = IMG_Load(final);	#else		tmp = SDL_LoadBMP(final);	#endif		#ifdef DEBUG		fprintf(stderr, "Loading image %d (%s) was successful\n", a, directory);	#endif		#ifdef DEBUG		fprintf(stderr, "Trying to set color for transparency on image %d\n", a);	#endif		SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(tmp->format, 255, 0, 255));		sprites_img[a] = SDL_DisplayFormat(tmp);		SDL_FreeSurface(tmp);		#ifdef DEBUG		fprintf(stderr, "It seemed to work.\n");	#endif		sprites_img_tocopy[a] = 0;	}void Copy_Image(unsigned short a, unsigned short i){	#ifdef DEBUG		fprintf(stderr, "Transfering the data of id %d to id %d.\n", a, i);	#endif		sprites_img_tocopy[i] = a;}void Put_image(unsigned short a, short x, short y){	SDL_Rect position;	position.x = x;	position.y = y;		SDL_Rect size;	size.x = 0;	size.y = 0;		#ifdef DEBUG_CRAZY		fprintf(stderr, "Put image %d on screen and update its position\n X: %d \n Y: %d\n", a, x ,y);	#endif	if (sprites_img_tocopy[a] > 0)	{		size.w = sprites_img[sprites_img_tocopy[a]]->w;		size.h = sprites_img[sprites_img_tocopy[a]]->h;		SDL_BlitSurface(sprites_img[sprites_img_tocopy[a]], &size, screen, &position);	}	else	{		size.w = sprites_img[a]->w;		size.h = sprites_img[a]->h;		SDL_BlitSurface(sprites_img[a], &size, screen, &position);	}}void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f){	SDL_Rect position;	position.x = x;	position.y = y;		SDL_Rect frame;	frame.x = f*w;	frame.y = 0;	frame.w = w;	frame.h = h;		#ifdef DEBUG_CRAZY		fprintf(stderr, "Put sprite %d on screen and update its position\n X: %d \n Y: %d\n Frame: %d\n", a, x ,y, f);	#endif	if (sprites_img_tocopy[a] > 0)	{		SDL_BlitSurface(sprites_img[sprites_img_tocopy[a]], &frame, screen, &position);	}	else	{		SDL_BlitSurface(sprites_img[a], &frame, screen, &position);	}}void Clear_screen(){	#ifdef DEBUG_CRAZY		fprintf(stderr, "Screen was cleared.\n");	#endif	    SDL_FillRect(screen, 0, 0);}void Faster_clearing(short x, short y, unsigned short w, unsigned short h){	SDL_Rect position;	position.x = x;	position.y = y;	position.w = w;	position.h = h;		#ifdef DEBUG_CRAZY		fprintf(stderr, "Screen was cleared.\n");	#endif	    SDL_FillRect(screen, &position, 0);}void Update_video(){		#ifdef UNCAPPED		SDL_Flip(screen);	#else		Uint32 start;		start = SDL_GetTicks();		SDL_Flip(screen);		if(real_FPS > SDL_GetTicks()-start) SDL_Delay(real_FPS-(SDL_GetTicks()-start));	#endif}void Faster_update(short x, short y, short w, short h){	#ifdef UNCAPPED		SDL_UpdateRect(screen, x, y, w, h);	#else		Uint32 start;		start = SDL_GetTicks();		SDL_UpdateRect(screen, x, y, w, h);		if(real_FPS > SDL_GetTicks()-start) SDL_Delay(real_FPS-(SDL_GetTicks()-start));	#endif}void Sync_refresh(){	#ifndef UNCAPPED		Uint32 start;		start = SDL_GetTicks();		if(real_FPS > SDL_GetTicks()-start) SDL_Delay(real_FPS-(SDL_GetTicks()-start));	#endif}void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B){	int color_draw;	color_draw = SDL_MapRGB(screen->format, R, G, B);	nSDL_SetPixel(screen,x,y,color_draw);}void Controls(){		if (isKeyPressed(Buttons_UP))			{			BUTTON.UP = 1;			}		else		{			BUTTON.UP = 0;			}				if (isKeyPressed(Buttons_DOWN))			{			BUTTON.DOWN = 1;			}		else		{			BUTTON.DOWN = 0;			}				if (isKeyPressed(Buttons_LEFT))			{			BUTTON.LEFT = 1;			}		else		{			BUTTON.LEFT = 0;			}				if (isKeyPressed(Buttons_RIGHT))			{			BUTTON.RIGHT = 1;			}		else		{			BUTTON.RIGHT = 0;			}		if (isKeyPressed(Buttons_A))			{			BUTTON.A = 1;			}		else		{			BUTTON.A = 0;			}				if (isKeyPressed(Buttons_B))			{			BUTTON.B = 1;			}		else		{			BUTTON.B = 0;			}				if (isKeyPressed(Buttons_C))			{			BUTTON.C = 1;			}		else		{			BUTTON.C = 0;			}				if (isKeyPressed(Buttons_D))			{			BUTTON.D = 1;			}		else		{			BUTTON.D = 0;			}				if (isKeyPressed(Buttons_START))			{			BUTTON.START = 1;			}		else		{			BUTTON.START = 0;			}					if (isKeyPressed(Buttons_SELECT))			{			BUTTON.SELECT = 1;			}		else		{			BUTTON.SELECT = 0;			}				if (isKeyPressed(Buttons_QUIT))			{			BUTTON.QUIT = 1;			}		else		{			BUTTON.QUIT = 0;			}}void Clear_Image(unsigned short a){	if (sprites_img[a] != NULL)	{		SDL_FreeSurface(sprites_img[a]);	}	sprites_img_tocopy[a] = 0;}void Clear_Images(){	unsigned short i;	for (i=0;i<MAX_IMAGE+1;i++)	{		if (sprites_img[i] != NULL)		{			SDL_FreeSurface(sprites_img[i]);		}		sprites_img_tocopy[i] = 0;	}}void Init_sound(){}void Load_Music(const char* directory){}void Play_Music(char loop){}void Clean_Music(){}void Load_SFX(unsigned char i, const char* directory){}void Play_SFX(unsigned char i){}void Unload_SFX(){}void Clearing(){	#ifdef DEBUG		fprintf(stderr, "Clean surface and free SDL from memory\n");	#endif		Clear_Images();	SDL_FreeSurface(screen);   	SDL_Quit();}