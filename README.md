====================================
GAMEBLABLA'S WRAPPER VERSION 6.0 
====================================

For :
* Computers (Windows, GNU/Linux)
	- SDL 1.2
	- SDL 2.0
	- Allegro 4
	- SFML 2.0+ (CSFML)
* 3DO 
* PSP
* GCW ZERO
* TI NSPIRE
* Atari Falcon (through the SDL port)
* Sega Dreamcast 
* Gizmondo

Tested: Windows, GNU/Linux, TI Nspire, Atari Falcon, 3DO, Sega Dreamcast

=============
DESCRIPTION
=============

What the hell is this ?

The Gameblabla's Wrapper is an ANSI C library whose goal is to simplify the development of cross-platform games.
It was originally designed to enable cross-platform development without using an emulator for 3DO.
It is now available for several platforms and it is suited for simple single-screen games. 
(You can do scrolling games too but you will have to copy each tile individually in memory)

With the Gameblabla's Wrapper, you can :
- Draw Pixels on screen
- Move images and sprites
- Play music and sound effects
- Play with a keyboard or a controller

All of this with the same piece of code !
The only differences are the limitations of each platform and library.

===============
TELL ME MORE 
===============

#include "API.h"
#include "INPUT.h"

int main ( void )
{
    short x,y;
    x = 50;
    y = 50;

    Init_video();
    Load_Image(1,"noname.bmp");

    while (!done)
    {
    
        Clear_screen();

        Controls();

		Put_image(1, x, y);

		if (BUTTON.UP) y = y - 1;
		if (BUTTON.DOWN) y = y + 1;
		if (BUTTON.LEFT) x = x - 1;
		if (BUTTON.RIGHT) x = x + 1;
 		if (BUTTON.QUIT) done = 1;

        Update_video();
    }

    Clearing();
    return 0;
}

The code above run on all the supported platforms without any modifications.
It should be fairly easy too for anyone to add support for other platforms too. 

==========
LICENSE
==========

The Gameblabla's wrapper is licensed under the Apache License Version 2.0.
See the NOTICE file for more details.
