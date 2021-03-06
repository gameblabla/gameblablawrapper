/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

/*
 * I have used (C)SFML version 2.1 for writing this 
 * Contact me if you are using a more recent version
 * and this does not work
*/

#include <SFML/Audio.h>
#include <SFML/Graphics.h>

#define FPS_VIDEO 60
#define MAX_IMAGE 512

#ifdef SOUND_ENABLED
	#define MAX_SFX 32
	sfMusic* music;
	sfSound* sfx_id[MAX_SFX];
	sfSoundBuffer* gfx_data[MAX_SFX];
#endif

#include "INPUT.h"
struct input BUTTON;

unsigned short done = 0;
const float real_FPS = 1000/FPS_VIDEO;
char* game_name = 0;

sfSprite* sprites_img[MAX_IMAGE];
sfTexture* sprites_textures[MAX_IMAGE];

#define Buttons_UP sfKeyUp 
#define Buttons_LEFT sfKeyLeft
#define Buttons_RIGHT sfKeyRight
#define Buttons_DOWN sfKeyDown
#define Buttons_A sfKeyX 
#define Buttons_B sfKeyC 
#define Buttons_C sfKeyV
#define Buttons_D sfKeyB
#define Buttons_START sfKeySpace
#define Buttons_SELECT sfKeyBack
#define Buttons_QUIT sfKeyEscape

/*
	The controller used here was an official Xbox 360 controller
*/
#define Joypad_A 0 
#define Joypad_B 1
#define Joypad_C 2
#define Joypad_D 3
#define Joypad_START 7 
#define Joypad_SELECT 5 

sfRenderWindow* window;

void Init_video(char* argv[])
{
	sfVideoMode mode = {320, 240, 32};
	window = sfRenderWindow_create(mode, game_name, sfResize | sfClose, NULL);
	sfRenderWindow_setFramerateLimit(window, FPS_VIDEO);
	sfRenderWindow_setVerticalSyncEnabled(window, 1);
}

void Load_Image(unsigned short a, const char* directory)
{
	sfImage* temp;
	
	if (sprites_img[a] != NULL)
	{
		sfTexture_destroy(sprites_textures[a]);
		sfSprite_destroy(sprites_img[a]);
	}
	
	temp = sfImage_createFromFile(directory);
	sfImage_createMaskFromColor(temp, sfMagenta, 0);
	sprites_textures[a] = sfTexture_createFromImage(temp, NULL);
	
	sfImage_destroy(temp);
	
	sprites_img[a] = sfSprite_create();
	sfSprite_setTexture(sprites_img[a], sprites_textures[a], sfTrue);
}

void Put_image(unsigned short a, short x, short y)
{
	sfVector2f position;
	position.x = x;
	position.y = y;

	sfSprite_setPosition(sprites_img[a], position);
	sfRenderWindow_drawSprite(window, sprites_img[a], NULL);
	
}

void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f)
{
	sfVector2f position;
	position.x = x;
	position.y = y;

	sfIntRect size = {w*f,0,w,h};

	sfSprite_setTextureRect(sprites_img[a], size);
	sfSprite_setPosition(sprites_img[a], position);
	sfRenderWindow_drawSprite(window, sprites_img[a], NULL);
}


void Clear_screen()
{
    sfRenderWindow_clear(window, sfBlack);
}

void Faster_clearing(short x, short y, unsigned short w, unsigned short h)
{
    Clear_screen();
}

void Update_video()
{
	sfRenderWindow_display(window);
}

void Faster_update(short x, short y, short w, short h)
{
	Update_video();
}

void Sync_refresh()
{
	Update_video();
}

void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B)
{
    sfVector2f position;
    sfVector2f scale;
    sfColor sfcolor_draw = { R, G, B, 255};
    /*sfColor sfcolor_draw = sfColor_fromRGBA( R, G, B, 255);*/
    sfRectangleShape* shape = sfRectangleShape_create();
    
    position.x = x;
    position.y = y;
    scale.x = 1;
    scale.y = 1;
      
    sfRectangleShape_setPosition(shape, position);
    sfRectangleShape_setSize(shape, scale);
s	fRectangleShape_setFillColor(shape, sfcolor_draw);
	sfRenderWindow_drawRectangleShape(window, shape, NULL);
}

void Controls()
{
        sfEvent event;
        char x, y;
        
		x = sfJoystick_getAxisPosition(0, sfJoystickX);
		y = sfJoystick_getAxisPosition(0, sfJoystickY);

        while (sfRenderWindow_pollEvent(window, &event))
        {
			if (event.type == sfEvtClosed)
			{
				sfRenderWindow_close(window);
			}
        }
        
		done = (!sfRenderWindow_isOpen(window)) ? 1 : 0;

		BUTTON.UP = sfKeyboard_isKeyPressed(Buttons_UP) ? 1 : 0;
		BUTTON.DOWN = sfKeyboard_isKeyPressed(Buttons_DOWN) ? 1 : 0;
		BUTTON.LEFT = sfKeyboard_isKeyPressed(Buttons_LEFT) ? 1 : 0;
		BUTTON.RIGHT = sfKeyboard_isKeyPressed(Buttons_RIGHT) ? 1 : 0;
		
		BUTTON.A = (sfKeyboard_isKeyPressed(Buttons_A) || sfJoystick_isButtonPressed(0, Joypad_A)) ? 1 : 0;
		BUTTON.B = (sfKeyboard_isKeyPressed(Buttons_B) || sfJoystick_isButtonPressed(0, Joypad_B)) ? 1 : 0;
		BUTTON.C = (sfKeyboard_isKeyPressed(Buttons_C) || sfJoystick_isButtonPressed(0, Joypad_C)) ? 1 : 0;
		BUTTON.D = (sfKeyboard_isKeyPressed(Buttons_D) || sfJoystick_isButtonPressed(0, Joypad_D)) ? 1 : 0;
		BUTTON.START = (sfKeyboard_isKeyPressed(Buttons_START) || sfJoystick_isButtonPressed(0, Joypad_START)) ? 1 : 0;
		BUTTON.SELECT = (sfKeyboard_isKeyPressed(Buttons_SELECT) || sfJoystick_isButtonPressed(0, Joypad_SELECT)) ? 1 : 0;
		BUTTON.QUIT = sfKeyboard_isKeyPressed(Buttons_QUIT) ? 1 : 0;

        if (y < -16)
        {
			BUTTON.UP = 1;
        }
		else if (y > 16)
        {
			BUTTON.DOWN = 1;
        }

        if (x < -16)
        {
			BUTTON.LEFT = 1;
        }
		else if (x > 16)
        {
			BUTTON.RIGHT = 1;
        }
}

void Clear_Image(unsigned short a)
{
	if (sprites_img[a] != NULL)
	{
		sfTexture_destroy(sprites_textures[a]);
		sfSprite_destroy(sprites_img[a]);
	}
}

void Clear_Images()
{
	unsigned short i;

	for (i=0;i<MAX_IMAGE+1;i++)
	{
		if (sprites_img[i] != NULL)
		{
			sfTexture_destroy(sprites_textures[i]);
			sfSprite_destroy(sprites_img[i]);
		}
	}
}

#ifdef SOUND_ENABLED

		void Init_sound()
		{
		}
		
		void Clean_Music()
		{
			if (music)
			{
				sfMusic_destroy(music);
			}
		}

		void Load_Music(const char* directory)
		{
			Clean_Music();
			music = sfMusic_createFromFile(directory);
		}

		void Play_Music(char loop)
		{
			sfMusic_play(music);
		}

		void Load_SFX(unsigned char i, const char* directory)
		{
			if (sfx_id[i])
			{
				sfSound_destroy(sfx_id[i]);
				sfSoundBuffer_destroy(gfx_data[i]);
			}
			
			gfx_data[i] = sfSoundBuffer_createFromFile(directory);
			sfx_id[i] = sfSound_create();
			sfSound_setBuffer(sfx_id[i],gfx_data[i]);
		}

		void Play_SFX(unsigned char i)
		{
			sfSound_play(sfx_id[i]);
		}

		void Unload_SFX()
		{
			unsigned short i;

			for (i=0;i<MAX_SFX;i++)
			{
				if (sfx_id[i])
				{
					sfSound_destroy(sfx_id[i]);
					sfSoundBuffer_destroy(gfx_data[i]);
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
		Unload_SFX();
	#endif

	Clear_Images();
}
