______________

-- KILLMINDS --
______________

Platforms : PC (Windows,Linux), 3DO, Nuon
Author : Gameblabla
Soon for PSP, Ti nspire, GCW Zero, Atari Falcon

_______________

-- Description --
_______________

Killminds is a clone of SupraKillMinds, itself based on Quartet.
You have to move a quarter of a square and you can place it in one of the four directions.
You can't move a 1/4 of a square over another one and if you do so, you will lose a life.
You can score points by clearing one of the fields with 4 squares.

Two binairies are provided for the Windows version :
One is using SDL 1.2 and the other one is using SDL 2.0.

If you have a decent computer, then you should choose the SDL 2 version.
The SDL 1.2 version is recommended for old computers with CRT monitors.

Allegro4, Allegro5 (Experimental) and SFML versions are also available but they need to be compiled from source.

A joystick or a keyboard can be used for playing the game.
The game automatically save your highscore.

The save file is called "killminds.save".

_______________

-- COMPILING  --
_______________

KillMinds requires the Gameblabla's Wrapper in order to compile.
To compile KillMinds, you first need to download the Gameblabla's Wrapper.
You can get it here : https://gameblabla.olympe.in/_wrapper.html

Put the WRAPPER folder in this directory or put a symlink. 
(This WRAPPER folder can be found inside Gameblabla's Wrapper archive)

You can compile a SDL 1.2, SDL 2.0, Allegro4, Allegro5 (Experimental) or (C)SFML based version.
For example, to compile the SDL 1.2 version :
make -f Makefile.SDL

For example, to compile the (C)SFML version :
make -f Makefile.SFML

And so on.