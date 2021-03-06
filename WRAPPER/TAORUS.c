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

#include <stdio.h>
#include <stdint.h>
#include <syscall.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <kbd.h>

#include "lib/yutani.h"
#include "lib/graphics.h"
#include "lib/decorations.h"

#include "INPUT.h"
struct input BUTTON;
struct tt touch;

static const float real_FPS = 1000/FPS_VIDEO;
unsigned short done = 0;
static char* game_name;
static sprite_t * sprites[MAX_IMAGE];
static yutani_t * yctx;
static yutani_window_t * window;
static gfx_context_t * ctx;
static unsigned char ch;
static int mouse_state;
static struct timeval start;
static sprite_t alpha_tmp;

#define Buttons_UP KEY_ARROW_UP
#define Buttons_LEFT KEY_ARROW_LEFT
#define Buttons_RIGHT KEY_ARROW_RIGHT
#define Buttons_DOWN KEY_ARROW_DOWN
#define Buttons_A KEY_LEFT_CTRL
#define Buttons_B KEY_LEFT_ALT
#define Buttons_C KEY_LEFT_SHIFT
#define Buttons_D KEY_CTRL_S
#define Buttons_START KEY_CTRL_X
#define Buttons_SELECT KEY_BACKSPACE
#define Buttons_QUIT KEY_ESCAPE

static inline int32_t min(int32_t a, int32_t b) {
	return (a < b) ? a : b;
}

static inline int32_t max(int32_t a, int32_t b) {
	return (a > b) ? a : b;
}

static void draw_sprite_part(gfx_context_t * ctx, sprite_t * sprite, int32_t x, int32_t y, int32_t cut_x, int32_t cut_y, int32_t size_w, int32_t size_h) 
{
	int32_t _left   = max(x, 0);
	int32_t _top    = max(y, 0);
	int32_t _right  = min(x + sprite->width,  ctx->width - 1);
	int32_t _bottom = min(y + sprite->height, ctx->height - 1);
	uint16_t _y;
	uint16_t _x;
	
	for (_y = cut_y; _y < cut_y+size_h; ++_y) {
		for (_x = cut_x; _x < cut_x+size_w; ++_x) {
			if (x + _x < _left || x + _x > _right || y + _y < _top || y + _y > _bottom)
				continue;
			if (sprite->alpha == ALPHA_MASK) {
				GFX(ctx, x + _x, y + _y) = alpha_blend(GFX(ctx, x + _x, y + _y), SPRITE(sprite, _x, _y), SMASKS(sprite, _x, _y));
			} else if (sprite->alpha == ALPHA_EMBEDDED) {
				GFX(ctx, x + _x, y + _y) = alpha_blend_rgba(GFX(ctx, x + _x, y + _y), SPRITE(sprite, _x, _y));
			} else if (sprite->alpha == ALPHA_INDEXED) {
				if (SPRITE(sprite, _x, _y) != sprite->blank) {
					GFX(ctx, x + _x, y + _y) = SPRITE(sprite, _x, _y) | 0xFF000000;
				}
			} else {
				GFX(ctx, x + _x, y + _y) = SPRITE(sprite, _x, _y) | 0xFF000000;
			}
		}
	}
}

static void init_sprite(int i, char * filename, char * alpha) 
{
	sprites[i] = malloc(sizeof(sprite_t));
	load_sprite(sprites[i], filename);
	if (alpha) 
	{
		sprites[i]->alpha = ALPHA_MASK;
		load_sprite(&alpha_tmp, alpha);
		sprites[i]->masks = alpha_tmp.bitmap;
	} 
	else 
	{
		sprites[i]->alpha = ALPHA_INDEXED;
	}
	sprites[i]->blank = 0x0;
}

static void init_sprite_png(int i, char * filename, char * alpha) 
{
	sprites[i] = malloc(sizeof(sprite_t));
	load_sprite_png(sprites[i], filename);
	if (alpha) 
	{
		sprites[i]->alpha = ALPHA_MASK;
		load_sprite(&alpha_tmp, alpha);
		sprites[i]->masks = alpha_tmp.bitmap;
	} 
	else 
	{
		sprites[i]->alpha = ALPHA_INDEXED;
	}
	sprites[i]->blank = 0x0;
}

static unsigned int GetTicks (void)
{
	unsigned int ticks;
	struct timeval now;
	gettimeofday(&now, NULL);
	ticks=(now.tv_sec-start.tv_sec)*1000+(now.tv_usec-start.tv_usec)/1000;
	return(ticks);
}

void Init_video(char* argv[])
{
	yctx = yutani_init();
	window = yutani_window_create(yctx, 320, 240);
	yutani_window_move(yctx, window, 50, 50);
	ctx = init_graphics_yutani(window);
	draw_fill(ctx,rgb(0,0,0));
	flip(ctx);
	yutani_flip(yctx, window);

	yutani_window_advertise_icon(yctx, window, "My game", "applications-simulation");
	init_decorations();
	
	gettimeofday(&start, NULL);
	
	printf("Initialisation\n");
}

void Load_Image(unsigned short a, const char* directory)
{
	init_sprite_png(a, directory, 0);
	printf("Sprite %s is loaded in memory\n", directory);
}

void Put_image(unsigned short a, short x, short y)
{
	draw_sprite(ctx, sprites[a], 0, 0);
}

void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f)
{
	draw_sprite_part(ctx, sprites[a], x, y, f*w, 0, w, h);
}

void Clear_screen()
{
	draw_fill(ctx,rgb(0,0,0));
}

void Faster_clearing(short x, short y, unsigned short w, unsigned short h)
{
	draw_fill(ctx,rgb(0,0,0));
}

void Update_video()
{
	/*unsigned int start_tim;
	start_tim = GetTicks();
	if(real_FPS > GetTicks()-start_tim) msleep(real_FPS-(GetTicks()-start_tim));*/
	usleep(16666);
	
	flip(ctx);
	yutani_flip(yctx, window);
}

void Faster_update(short x, short y, short w, short h)
{
	Update_video();
}

void Sync_refresh()
{
	usleep(16666);
	/*unsigned int start_tim;
	start_tim = GetTicks();
	if(real_FPS > GetTicks()-start_tim) msleep(real_FPS-(GetTicks()-start_tim));*/
}

void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B)
{
}

void Draw_Rect(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char R, unsigned char G, unsigned char B)
{
}

static char handle_event(yutani_msg_t * m) 
{
	if (m) {
		switch (m->type) {
			case YUTANI_MSG_KEY_EVENT:
				{
					struct yutani_msg_key_event * ke = (void*)m->data;
					if (ke->event.action == KEY_ACTION_DOWN) {
						return ke->event.keycode;
					}
				}
				break;
			case YUTANI_MSG_WINDOW_FOCUS_CHANGE:
				{
					struct yutani_msg_window_focus_change * wf = (void*)m->data;
					yutani_window_t * win = hashmap_get(yctx->windows, (void*)wf->wid);
					if (win) {
						win->focused = wf->focused;
					}
				}
				break;
			case YUTANI_MSG_WINDOW_MOUSE_EVENT:
				if (decor_handle_event(yctx, m) == DECOR_CLOSE) {
					return 'q';
				}
				break;
			case YUTANI_MSG_SESSION_END:
				return 'q';
			default:
				break;
		}
		free(m);
	}
	return 0;
}



void Controls()
{
	struct yutani_msg_window_mouse_event * me;
	yutani_msg_t * m = NULL;
	unsigned char was, is;
	static unsigned char time_touch = 0;
	
	do {
		m = yutani_poll_async(yctx);
		handle_event(m);
	} while (m);

	m = yutani_poll(yctx);
	ch = handle_event(m);
	
	me = (void*)m->data;
	was = mouse_state & (1 << 0);
	is  = me->buttons & (1 << 0);
	mouse_state = me->buttons;
	
	switch(touch.state)
	{
		case 0:
			if (is && (was != is))
			{
				touch.state = 1;
			}
		break;
		case 1:
			if (me->buttons != 1)
			{
				touch.state = 2;
			}
		break;
		case 2:
			time_touch++;
			if (time_touch > 1)
			{
				touch.state = 0;
				time_touch = 0;
			}
		break;
	}
	touch.x = me->new_x;
	touch.y = me->new_y;
		
	BUTTON.UP = (ch == Buttons_UP) ? 1 : 0;
	BUTTON.DOWN = (ch == Buttons_DOWN) ? 1 : 0;
	BUTTON.LEFT = (ch == Buttons_LEFT) ? 1 : 0;
	BUTTON.RIGHT = (ch == Buttons_RIGHT) ? 1 : 0;
		
	BUTTON.A = (ch == Buttons_A) ? 1 : 0;
	BUTTON.B = (ch == Buttons_B) ? 1 : 0;
	BUTTON.C = (ch == Buttons_C) ? 1 : 0;
	BUTTON.D = (ch == Buttons_D) ? 1 : 0;
	BUTTON.START = (ch == Buttons_START) ? 1 : 0;
	BUTTON.SELECT = (ch == Buttons_SELECT) ? 1 : 0;
	BUTTON.QUIT = (ch == Buttons_QUIT) ? 1 : 0;
}

void Clear_Image(unsigned short a)
{

}

void Clear_Images()
{

}

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


void Clearing()
{
}



