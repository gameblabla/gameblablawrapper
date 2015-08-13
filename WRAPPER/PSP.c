/*
   COPYRIGHT (C) 2014-2015 GAMEBLABLA
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdlib.h>
#include <SDL/SDL.h>

#define FPS_VIDEO 60
#define MAX_IMAGE 512

#ifdef SOUND_ENABLED
	#include <SDL/SDL_mixer.h>
	#define MAX_SFX 16
	
	int music;
	int gfx_id[32];
#endif

#ifdef IMAGE_CODEC_ENABLED
	#include <SDL/SDL_image.h>
#endif

#include "INPUT.h"
struct input BUTTON;

const float real_FPS = 1000/FPS_VIDEO;

int done = 0;
char* game_name = "";

SDL_Surface *sprites_img[MAX_IMAGE];
SDL_Surface *screen;
unsigned short sprites_img_tocopy[MAX_IMAGE];

int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);

#define Buttons_UP PSP_CTRL_UP
#define Buttons_LEFT PSP_CTRL_LEFT
#define Buttons_RIGHT PSP_CTRL_RIGHT
#define Buttons_DOWN PSP_CTRL_DOWN
#define Buttons_A PSP_CTRL_CROSS
#define Buttons_B PSP_CTRL_SQUARE
#define Buttons_C PSP_CTRL_CIRCLE
#define Buttons_D PSP_CTRL_TRIANGLE
#define Buttons_START PSP_CTRL_START
#define Buttons_SELECT PSP_CTRL_SELECT
#define Buttons_QUIT 0

void msleep(unsigned char milisec)
{
	#ifdef UNIX 
		struct timespec req={0};
		time_t sec=(unsigned short)(milisec/1000);
		
		milisec=milisec-(sec*1000);
		req.tv_sec=sec;
		req.tv_nsec=milisec*1000000L;
		
		while(nanosleep(&req,&req)==-1)
		continue;
	#else
		SDL_Delay(milisec);
	#endif
}

void Init_video()
{
	  SetupCallbacks();
	  sceCtrlSetSamplingCycle(0);
	  sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	  scePowerSetClockFrequency(333, 333, 166);
	  SDL_Init( SDL_INIT_VIDEO );
	  screen = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
}

void Load_Image(unsigned short a, const char* directory)
{
	SDL_Surface *tmp;
	
	#ifdef DEBUG
		fprintf(stderr, "Clearing surface for image %d...\n", a);
	#endif
	
	if (sprites_img[a] != NULL)
	{
		SDL_FreeSurface(sprites_img[a]);
	}
	
	#ifdef DEBUG
		fprintf(stderr, "Trying to load image %d (%s)...\n", a, directory);
	#endif

	#ifdef IMAGE_CODEC_ENABLED
		tmp = IMG_Load(directory);
	#else
		tmp = SDL_LoadBMP(directory);
	#endif
	
	#ifdef DEBUG
		fprintf(stderr, "Loading image %d (%s) was successful\n", a, directory);
	#endif
	
	#ifdef DEBUG
		fprintf(stderr, "Trying to set color for transparency on image %d\n", a);
	#endif
	
	SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(tmp->format, 255, 0, 255));
	
	sprites_img[a] = SDL_DisplayFormat(tmp);
	
	SDL_FreeSurface(tmp);
	
	#ifdef DEBUG
		fprintf(stderr, "It seemed to work.\n");
	#endif
	
	sprites_img_tocopy[a] = 0;
	
}

void Copy_Image(unsigned short a, unsigned short i)
{
	#ifdef DEBUG
		fprintf(stderr, "Transfering the data of id %d to id %d.\n", a, i);
	#endif
	
	sprites_img_tocopy[i] = a;
}

void Put_image(unsigned short a, short x, short y)
{
	SDL_Rect position;
	position.x = x;
	position.y = y;
	
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Put image %d on screen and update its position\n X: %d \n Y: %d\n", a, x ,y);
	#endif

	if (sprites_img_tocopy[a] > 0)
	{
		SDL_BlitSurface(sprites_img[sprites_img_tocopy[a]], NULL, screen, &position);
	}
	else
	{
		SDL_BlitSurface(sprites_img[a], NULL, screen, &position);
	}
}

void Put_sprite(unsigned short a, short x, short y, unsigned short w,unsigned short h, unsigned char f)
{
	SDL_Rect position;
	position.x = x;
	position.y = y;
	
	SDL_Rect frame;
	frame.x = f*w;
	frame.y = 0;
	frame.w = w;
	frame.h = h;
	
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Put sprite %d on screen and update its position\n X: %d \n Y: %d\n Frame: %d\n", a, x ,y, f);
	#endif

	if (sprites_img_tocopy[a] > 0)
	{
		SDL_BlitSurface(sprites_img[sprites_img_tocopy[a]], &frame, screen, &position);
	}
	else
	{
		SDL_BlitSurface(sprites_img[a], &frame, screen, &position);
	}
}

void Clear_screen()
{
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Screen was cleared.\n");
	#endif
	
    SDL_FillRect(screen, 0, 0);
}


void Update_video()
{
	#ifdef UNCAPPED
		SDL_Flip(screen);
	#else
		Uint32 start;
		start = SDL_GetTicks();
		SDL_Flip(screen);
		if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));
	#endif
}

void Faster_update(short x, short y, short w, short h)
{
	#ifdef UNCAPPED
		SDL_UpdateRect(screen, x, y, w, h);
	#else
		Uint32 start;
		start = SDL_GetTicks();
		SDL_UpdateRect(screen, x, y, w, h);
		if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));
	#endif
}

void Faster_clearing(short x, short y, unsigned short w, unsigned short h)
{
	SDL_Rect position;
	position.x = x;
	position.y = y;
	position.w = w;
	position.h = h;
	
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Screen was cleared.\n");
	#endif
	
    SDL_FillRect(screen, &position, 0);
}


void Sync_refresh()
{
	#ifndef UNCAPPED
		Uint32 start;
		start = SDL_GetTicks();
		if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));
	#endif
}

void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B)
{
	SDL_LockSurface(screen);
	
	int color_draw;
	SDL_Rect scr_draw;
	scr_draw.x = x;
	scr_draw.y = y;
	scr_draw.w = 1;
	scr_draw.h = 1;
	
	color_draw = SDL_MapRGB(screen->format, R, G, B);
	
	SDL_FillRect(screen, &scr_draw, color_draw);
	
	SDL_UnlockSurface(screen);
}


void Controls()
{
		SceCtrlData pad;
		sceCtrlPeekBufferPositive(&pad,1);

		if (pad.Buttons & Buttons_UP)	
		{
			BUTTON.UP = 1;	
		}
		else
		{
			BUTTON.UP = 0;	
		}
		
		if (pad.Buttons & Buttons_DOWN)	
		{
			BUTTON.DOWN = 1;	
		}
		else
		{
			BUTTON.DOWN = 0;	
		}
		
		if (pad.Buttons & Buttons_LEFT)	
		{
			BUTTON.LEFT = 1;	
		}
		else
		{
			BUTTON.LEFT = 0;	
		}
		
		if (pad.Buttons & Buttons_RIGHT)	
		{	
			BUTTON.RIGHT = 1;	
		}
		else
		{
			BUTTON.RIGHT = 0;	
		}

		if (pad.Buttons & Buttons_A)	
		{
			BUTTON.A = 1;	
		}
		else
		{
			BUTTON.A = 0;	
		}
		
		if (pad.Buttons & Buttons_B)	
		{
			BUTTON.B = 1;	
		}
		else
		{
			BUTTON.B = 0;	
		}
		
		if (pad.Buttons & Buttons_C)	
		{
			BUTTON.C = 1;	
		}
		else
		{
			BUTTON.C = 0;	
		}
		
		if (pad.Buttons & Buttons_D)	
		{
			BUTTON.D = 1;	
		}
		else
		{
			BUTTON.D = 0;	
		}
		
		if (pad.Buttons & Buttons_START)	
		{
			BUTTON.START = 1;	
		}
		else
		{
			BUTTON.START = 0;	
		}
			
		if (pad.Buttons & Buttons_SELECT)	
		{
			BUTTON.SELECT = 1;	
		}
		else
		{
			BUTTON.SELECT = 0;	
		}
}

void Clear_Image(unsigned short a)
{
	if (sprites_img[a] != NULL)
	{
		SDL_FreeSurface(sprites_img[a]);
	}
	sprites_img_tocopy[a] = 0;
}

void Clear_Images()
{
	short i;

	for (i=0;i<MAX_IMAGE+1;i++)
	{
		if (sprites_img[i] != NULL)
		{
			SDL_FreeSurface(sprites_img[i]);
		}
		sprites_img_tocopy[i] = 0;
	}
}


#ifdef SOUND_ENABLED

		void Init_sound()
		{
			#ifdef DEBUG
				fprintf(stderr, "Init sound system\n");
			#endif
			Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024);
			Mix_AllocateChannels(MAX_SFX);
		}
		
		void Clean_Music()
		{
			#ifdef DEBUG
				fprintf(stderr, "Stop and clear music from memory\n");
			#endif
			
			if (music)
			{
				Mix_HaltMusic();
				Mix_FreeMusic(music);
			}
		}

		void Load_Music(const char* directory)
		{
			#ifdef DEBUG
				fprintf(stderr, "Load music in memory\n");
			#endif
			Clean_Music();
			music = Mix_LoadMUS(directory);
		}

		void Play_Music(char loop)
		{
			if (loop == 1)
			{
				#ifdef DEBUG
					fprintf(stderr, "Playing music in loop\n");
				#endif
				Mix_PlayMusic(music, -1);
			}
			else
			{
				#ifdef DEBUG
					fprintf(stderr, "Playing music\n");
				#endif
				Mix_PlayMusic(music, 0);
			}
		}

		void Load_SFX(unsigned char i, const char* directory)
		{
			#ifdef DEBUG
				fprintf(stderr, "Load sound effect %d (%s) in memory\n", i, directory);
			#endif
			
			if (gfx_id[i])
			{
				Mix_FreeChunk(gfx_id[i]);
				gfx_id[i] = NULL;
			}
			gfx_id[i] = Mix_LoadWAV(directory);
		}

		void Play_SFX(unsigned char i)
		{
			#ifdef DEBUG
				fprintf(stderr, "Play sound effect %d loaded in memory\n", i);
			#endif
			
			Mix_PlayChannel(-1, gfx_id[i], 0) ;
		}

		void Unload_SFX()
		{
			short i;
			
			#ifdef DEBUG
				fprintf(stderr, "Free sound effect from memory\n");
			#endif
			
			for (i=0;i<MAX_SFX;i++) 
			{
				if (gfx_id[i])
				{
					Mix_FreeChunk(gfx_id[i]);
					gfx_id[i] = NULL;
				}
			}
		}

#else

		void Init_sound()
		{
		}
		
		void Clean_Music()
		{
		}

		void Load_Music(const char* directory)
		{
		}

		void Play_Music(char loop)
		{
		}

		void Load_SFX(unsigned char i, const char* directory)
		{
		}

		void Play_SFX(unsigned char i)
		{
		}

		void Unload_SFX()
		{
		}

#endif


void Clearing()
{
	#ifdef SOUND_ENABLED
		Clean_Music();
	#endif

	#ifdef DEBUG
		fprintf(stderr, "Clean surface and free SDL from memory\n");
	#endif
	
	Clear_Images();

	SDL_FreeSurface(screen);
	
	#ifdef SOUND_ENABLED
		Unload_SFX();
		Mix_CloseAudio();
	#endif
	
	sceKernelExitGame();
	return 0;
}


int exit_callback(int arg1, int arg2, void *common) 
{
	done = 1;
	return 0;
}

int CallbackThread(SceSize args, void *argp) 
{
	int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void) 
{
	int thid = sceKernelCreateThread("CallbackThread", CallbackThread, 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);
	if (thid >= 0) sceKernelStartThread(thid, 0, 0);
	return thid;
}
