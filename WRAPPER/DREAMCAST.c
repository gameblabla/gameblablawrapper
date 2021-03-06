/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#define FPS_VIDEO 60
#define MAX_IMAGE 512
#define MAX_PATH 128

#include <kos.h>
#include <string.h>
#include <mp3/sndserver.h>
#include <oggvorbis/sndoggvorbis.h>

#if defined(DEBUG) || defined(DEBUG_CRAZY)
	#include <stdio.h>
#endif

#include <SDL/SDL.h>

#ifdef SOUND_ENABLED
	#define MAX_SFX 32
	char* music_path;
	short music_type;
	sfxhnd_t sfx_id[MAX_SFX];
#endif

#ifdef IMAGE_CODEC_ENABLED
	#include <SDL/SDL_image.h>
#endif

#include "INPUT.h"
struct input BUTTON;

unsigned short done = 0;

const float real_FPS = float(1000/FPS_VIDEO);

char* game_name = 0;

SDL_Surface *sprites_img[MAX_IMAGE];
SDL_Surface *screen;

#ifdef JOYSTICK
	SDL_Joystick *joy;
#endif

maple_device_t *cont, *kbd;
cont_state_t *state;

#define Buttons_UP CONT_DPAD_UP
#define Buttons_LEFT CONT_DPAD_LEFT
#define Buttons_RIGHT CONT_DPAD_RIGHT
#define Buttons_DOWN CONT_DPAD_DOWN
#define Buttons_A CONT_A
#define Buttons_B CONT_B
#define Buttons_C CONT_X
#define Buttons_D CONT_Y
#define Buttons_START CONT_START
#define Buttons_SELECT 0
#define Buttons_QUIT 0

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);

void msleep(unsigned char milisec)
{
	struct timespec req={0};
	time_t sec=(unsigned short)(milisec/1000);

	milisec=milisec-(sec*1000);
	req.tv_sec=sec;
	req.tv_nsec=milisec*1000000L;

	while(nanosleep(&req,&req)==-1)
		continue;
}

void Init_video(char* argv[])
{
	#ifdef JOYSTICK
		SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
		SDL_JoystickEventState(SDL_ENABLE);
	#else
		SDL_Init( SDL_INIT_VIDEO );
	#endif
	
	screen = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE);
	SDL_ShowCursor(0);
}

void Load_Image(unsigned short a, const char* directory)
{
	char final[MAX_PATH];
	SDL_Surface *tmp;
	
	sprintf(final, "/cd/%s", directory);

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
		tmp = IMG_Load(final);
	#else
		tmp = SDL_LoadBMP(final);
	#endif

	#ifdef DEBUG
		fprintf(stderr, "Loading image %d (%s) was successful\n", a, directory);
	#endif

	#ifdef DEBUG
		fprintf(stderr, "Trying to set a colorkey on image %d\n", a);
	#endif

	SDL_SetColorKey(tmp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(tmp->format, 255, 0, 255));

	#ifdef DEBUG
		fprintf(stderr, "Good, Display format it...\n");
	#endif

	sprites_img[a] = SDL_DisplayFormat(tmp);
	
	#ifdef DEBUG
		fprintf(stderr, "Ok, Deleting temporary surface...\n");
	#endif

	SDL_FreeSurface(tmp);

	#ifdef DEBUG
		fprintf(stderr, "Operation sucessful !\n");
	#endif

}

void Put_image(unsigned short a, short x, short y)
{
	SDL_Rect position;
	position.x = x;
	position.y = y;

	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Put image %d on screen and update its position\n X: %d \n Y: %d\n", a, x ,y);
	#endif
	
	SDL_BlitSurface(sprites_img[a], NULL, screen, &position);
}

void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f)
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

	SDL_BlitSurface(sprites_img[a], &frame, screen, &position);
}


void Clear_screen()
{
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Screen was cleared.\n");
	#endif

    SDL_FillRect(screen, 0, 0);
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
    /*kbd = maple_enum_type(0, MAPLE_FUNC_KEYBOARD);*/
	cont  = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
	state = (cont_state_t *) maple_dev_status(cont);
	
	BUTTON.UP 	= (state->buttons & Buttons_UP)	   ? 1 : 0;
	BUTTON.DOWN 	= (state->buttons & Buttons_DOWN)  ? 1 : 0;
	BUTTON.LEFT 	= (state->buttons & Buttons_LEFT)  ? 1 : 0;
	BUTTON.RIGHT 	= (state->buttons & Buttons_RIGHT) ? 1 : 0;
		
	BUTTON.A 	= (state->buttons & Buttons_A) ? 1 : 0;
	BUTTON.B 	= (state->buttons & Buttons_B) ? 1 : 0;
	BUTTON.C 	= (state->buttons & Buttons_C) ? 1 : 0;
	BUTTON.D 	= (state->buttons & Buttons_D) ? 1 : 0;
	BUTTON.START 	= (state->buttons & Buttons_START) ? 1 : 0;
}

void Clear_Image(unsigned short a)
{
	if (sprites_img[a] != NULL)
	{
		SDL_FreeSurface(sprites_img[a]);
	}
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
	}
}

#ifdef SOUND_ENABLED

		void Init_sound()
		{
			#ifdef DEBUG
				fprintf(stderr, "Init sound system\n");
			#endif
			snd_stream_init();
			mp3_init();
			sndoggvorbis_init();
		}
		
		void Clean_Music()
		{
			#ifdef DEBUG
				fprintf(stderr, "Stop and clear music from memory\n");
			#endif

			if (music_type == 0)
			{
			}
			else if (music_type == 1)
			{
				mp3_stop();
			}		
			else if (music_type == 2)
			{
				sndoggvorbis_stop();
			}	
		}

		void Load_Music(const char* directory)
		{
			#ifdef DEBUG
				fprintf(stderr, "Load music in memory\n");
			#endif
			
			/* Stop music before loading another one */
			if (music_type == 0)
			{
			}
			else if (music_type == 1)
			{
				mp3_stop();
			}		
			else if (music_type == 2)
			{
				sndoggvorbis_stop();
			}	
			
			music_type = 0;
			
			char* temp = strstr(directory,".mp3");
			if (temp != NULL)
			{
				music_type = 1;
			}
			else
			{
				temp = strstr(directory, ".ogg");
				if (temp != NULL)
				{
					music_type = 2;
				}
			}
			
			sprintf(music_path, "/cd/%s", directory);
			 
			if (music_type == 0)
			{
				#ifdef DEBUG
				printf("Music : assuming to be a WAV (or unknown)\n"); 
				#endif
			}
			else if (music_type == 1)
			{
				#ifdef DEBUG
				printf("Music file is a MP3\n"); 
				#endif
			}		
			else if (music_type == 2)
			{
				#ifdef DEBUG
				printf("Music file is a OGG\n"); 
				#endif
			}	
				
		}

		void Play_Music(char loop)
		{
			if (music_type == 0)
			{
				/*
				if (loop == 0)
				{
					wave_stream(music_path);
				}
				else
				{
					wave_stream(music_path);
				}
				*/
			}
			else if (music_type == 1)
			{
				mp3_start(music_path, loop);
			}		
			else if (music_type == 2)
			{
				sndoggvorbis_start(music_path, loop);
			}	
		}


		void Load_SFX(unsigned char i, const char* directory)
		{
			char final[MAX_PATH];
			sprintf(final, "/cd/%s", directory);
			#ifdef DEBUG
				fprintf(stderr, "Load sound effect %d (%s) in memory\n", i, directory);
			#endif
			
			if (sfx_id[i] != SFXHND_INVALID)
			{
				snd_sfx_unload(sfx_id[i]);
			}
			
			sfx_id[i] = snd_sfx_load(final);
		}

		void Play_SFX(unsigned char i)
		{
			#ifdef DEBUG
				fprintf(stderr, "Play sound effect %d loaded in memory\n", i);
			#endif
			snd_sfx_play(sfx_id[i], 255, 0x80);
		}

		void Unload_SFX()
		{
			#ifdef DEBUG
				fprintf(stderr, "Free all sound effect from memory\n");
			#endif
			
			snd_sfx_unload_all();
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
		mp3_stop();
		mp3_shutdown();
		sndoggvorbis_stop();
		sndoggvorbis_shutdown();
		snd_stream_shutdown();
	#endif
	
   	SDL_Quit();
}
