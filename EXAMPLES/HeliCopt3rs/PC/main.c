#include "WRAPPER/API.h"
#include "WRAPPER/INPUT.h"
#include "main.h"

#include <stdio.h>
#include <stdlib.h>

/*
 *
 * 	HELI-COPT3RS
 * 	RECREATION
 *
 * 	FOR GAMEBLABLA'S WRAPPER
 * 	ORIGINALLY PROGRAMMED FOR 3DO
 *
 */

/* 
    This program is free software. It comes without any warranty, to
    the extent permitted by applicable law. You can redistribute it
    and/or modify it under the terms of the Do What The Fuck You Want
    To Public License, Version 2, as published by Sam Hocevar. 
    See http://www.wtfpl.net/ for more details. 
*/

short helice_frame = 0;
short helice_time = 0;

// Alien position
short Player_X = 150;
short Player_Y = 196;

// Animation
unsigned char Frame_player	 = 0;
unsigned char Facing_player	 = 0;
unsigned char Max_Frame_player = 3;
unsigned char Delay_Frame_player = 0;
unsigned char state_player = 0;
char time_speed = 0;
char speed_player = 0;
short helice_diff = -2;

//Controls
unsigned char A_state = 0;
unsigned char A_time = 0;
unsigned char begin = 0;
unsigned char death = 0;
char platform[2];

short score = 0, highscore = 0;
short platforme_x_left[3];
short platforme_x_right[3];
short platforme_y[3];
short cloud_x[2], cloud_y[2];
short bottom_y = 192;
short gameover_x = -128;
short number_x[3] , number_y[3];

short hammer_x[5], hammer_y[5];
short hammer_anim_time = 0 , hammer_frame = 0 , hammer_facing = 0;
short hammer_collisions[5];


unsigned char game_mode;

int main ( int argc, char* argv[] )
{
		game_name = "Heli-Copt3rs";
		
		Init_video(argv);
		Init_sound();
		
		game_mode = 0;

	
		while(!done)
		{
			Clear_screen();
			Controls();
			
			if (BUTTON.QUIT) done = 1;
					
			if (game_mode == 0)
			{
				InitializeGame();
				
			}
			else if (game_mode == 1)
			{
				GameLoop();
				
			}
			

			Update_video();
		}
		
	Clearing();
	return 0;
}

void load_game_memory()
{
	Load_Image(1,"data/background.bmp");
	Load_Image(2,"data/back.bmp");
	Load_Image(3,"data/bar_gauche.bmp");
	Load_Image(4,"data/bar_droite.bmp");
	Load_Image(5,"data/character.bmp");
	Load_Image(6,"data/hammer_spr.bmp");
	Load_Image(7,"data/cloud.bmp");
	Load_Image(8,"data/gameover.bmp");
	Load_Image(9,"data/try.bmp");
	Load_Image(10,"data/score.bmp");
	Load_Image(11,"data/helice.bmp");
	Load_Image(12,"data/result.bmp");

	game_mode = 1;
	
}

void InitializeGame()
{
	
	load_game_memory();
	
	Init_Audio();
	Reset_game();
	
	game_mode = 1;
}

void GameLoop()
{
	
	Put_image(1 ,0, 0);

    Show_Bottom_back();
	
	A_button();
	
	Collisions();
	Platforms_manage();
	Show_Clouds();
	Show_Hammer();
	
	Show_platforme_gauche_0();
	Show_platforme_gauche_1();
	Show_platforme_droite_0();
	Show_platforme_droite_1();
	
	Show_GameOver();
	
	Show_Player();
	Show_helices();
	
	Show_score();

	if (begin == 1) Move_player();

	Gameplay();
}

void Show_platforme_gauche_0()
{
	Put_image(4, platforme_x_right[0], platforme_y[0]);
}
void Show_platforme_gauche_1()
{
	Put_image(3, platforme_x_right[1], platforme_y[1]);
}
void Show_platforme_droite_0()
{
	Put_image(3, platforme_x_left[0], platforme_y[0]);
}
void Show_platforme_droite_1()
{
	Put_image(3, platforme_x_left[1], platforme_y[1]);
}

void Show_GameOver()
{	

	if (state_player == 2)
	{
		Put_image(8, gameover_x, 24);
		Put_image(12, gameover_x-16, 96);
		Put_image(9, gameover_x-18, 184);
		if (gameover_x < 96) gameover_x = gameover_x + 6;
	}
	else
	{
		gameover_x = -320;
		if (gameover_x == -320)
		{
			Put_image(8, gameover_x, 24);
			Put_image(12, gameover_x-16, 96);
			Put_image(9, gameover_x-18, 184);
		}
	}
}

void Show_score()
{
	if (death == 0)
	{
		Put_sprite(10, 144, 8, 16, 16, (score/10)%10);
		Put_sprite(10, 160, 8, 16, 16, (score)%10);
		Put_sprite(10, -320, 133, 16, 16, 0);
		Put_sprite(10, -320, 133, 16, 16, 0);
	}
	else if (death == 2)
	{
		Put_sprite(10, gameover_x+59, 101, 16, 16, (score/10)%10 );
		Put_sprite(10, gameover_x+75, 101, 16, 16, (score)%10 );
		Put_sprite(10, gameover_x+59, 133, 16, 16, (highscore/10)%10 );
		Put_sprite(10, gameover_x+75, 133, 16, 16, (highscore)%10 );
	}

}

void Add_score()
{
	score = score + 1;
}

void Show_helices()
{
	static short X_helice;

	helice_time=helice_time+1;
	
	if (death == 0)
	{
			Put_sprite(11, X_helice, Player_Y, 24, 12, helice_frame );
			if (begin == 1)
			{
					if (Facing_player == 0)
					{
						X_helice = Player_X+4;
						
						if (helice_frame > 2) helice_frame = 0;
						
						if (helice_time > 2)
						{
							helice_frame = helice_frame + 1;
							if (helice_frame > 2) helice_frame = 0;
							helice_time = 0;
						}
					}	
					if (Facing_player == 4)
					{
						X_helice = Player_X-7;
						
						if (helice_frame < 3) helice_frame = 2;
						
						if (helice_time > 2)
						{
							helice_frame = helice_frame + 1;
							if (helice_frame > 5) helice_frame = 3;
							helice_time = 0;
						}
					}	
			}
			else{
				X_helice = Player_X-2;
				helice_frame = 6;
			}
	}
	else
	{
		Put_sprite(11, X_helice, Player_Y, 24, 12, helice_frame );
		X_helice = -320;
	}

}

void Init_Audio()
{
	Init_sound();
	Load_SFX(1, "gfx/smack.wav");
	Load_SFX(2, "gfx/switch.wav");
}

void play_smack()
{
	Play_SFX(1);
}

void play_switch()
{
	Play_SFX(2);
}

void Reset_game()
{
	platforme_x_right[0] = rand_a_b(100,210);
	platforme_x_left[0] = platforme_x_right[0] - 296 - rand_a_b(32,64);
	platforme_x_right[1] = rand_a_b(100,210);
	platforme_x_left[1] = platforme_x_right[1] - 296 - rand_a_b(32,64);
	
	platforme_y[0] = 40;
	platforme_y[1] = -140;
	cloud_x[0] = rand_a_b(10,60);
	cloud_x[1] = rand_a_b(185,220);
	cloud_y[0] = rand_a_b(-128,-220);
	cloud_y[1] = rand_a_b(-128,-220);
	
	Player_X = 150;
	Player_Y = 196;
	bottom_y = 192;
	
	Frame_player	 = 0;
	Facing_player	 = 0;
	Max_Frame_player = 3;
	Delay_Frame_player	 = 0;
	state_player = 0;
	time_speed = 0;
	speed_player = 0;

	A_state = 0;
	A_time = 0;
	begin = 0;
	death = 0;
	
	helice_frame = 0;
	helice_time = 0;
	
	gameover_x = -128;
	score = 0;

	platform[0] = 0;
	platform[1] = 0;
}

void Gameplay()
{
	unsigned char i;

	if (begin == 0 && death == 0 && A_state > 2) 
	{
		A_state = 3;
		A_time = 3;
		begin = 1;
	}
	if (state_player == 2 && death == 2 && A_state > 2) 
	{
		A_state = 3;
		A_time = 3;
		Reset_game();
	}
	if (BUTTON.B) Trigger_Death();
	
	for (i=0; i<2; i++)
	{
		if (platform[i] < 1 && Player_Y<platforme_y[i])
		{
			platform[i] = 1;
			Add_score();
		}
	}
	
	if (death == 1)
	{
		Frame_player = 0;
		state_player = 1;
		Delay_Frame_player = 0;
		Facing_player = 6;
		if (score > highscore) highscore=score;
		death = 2;
	}	
	if (death == 2)
	{	
		
		if (Player_Y-5 > bottom_y)
		{
			state_player = 2;
		}
		else
		{
			if (bottom_y > 199)
			{
				bottom_y = bottom_y - 8;
				platforme_y[0] = platforme_y[0] - 8;
				platforme_y[1] = platforme_y[1] - 8;
				cloud_y[0] = cloud_y[0] - 8;
				cloud_y[1] = cloud_y[1] - 8;
			}
			else if (bottom_y < 200)
			{
				Player_Y = Player_Y + 5;
			}
		}
	}
	
}

void Collisions()
{
	unsigned char i;

	//Handle Collisions between 'barres' and the 'Player'
	for (i=0; i<2; i++)
	{
		if ((Player_X + 18 > platforme_x_right[i]) && (Player_X+2 < platforme_x_right[i] + 256) && (Player_Y + 24 > platforme_y[i]) && (Player_Y+4 < platforme_y[i] + 16)) Trigger_Death();
	}
	
	for (i=0; i<2; i++)
	{
		if ((Player_X + 18 > platforme_x_left[i]) && (Player_X+2 < platforme_x_left[i] + 256) && (Player_Y + 24 > platforme_y[i]) && (Player_Y+4 < platforme_y[i] + 16)) Trigger_Death();
	}
	
	//Between Hammers and the Player
	if (hammer_frame==0)
	{
		hammer_collisions[0] = 0;
		hammer_collisions[2] = 0;
		hammer_collisions[1] = 21;
		hammer_collisions[3] = 30;
	}
	else if (hammer_frame==1)
	{
		hammer_collisions[0] = 0;
		hammer_collisions[2] = 0;
		hammer_collisions[1] = 22;
		hammer_collisions[3] = 32;
	}
	else if (hammer_frame==2)
	{
		hammer_collisions[0] = 0;
		hammer_collisions[2] = 0;
		hammer_collisions[1] = 34;
		hammer_collisions[3] = 32;
	}
	else if (hammer_frame==3)
	{
		hammer_collisions[0] = 0;
		hammer_collisions[2] = 0;
		hammer_collisions[1] = 42;
		hammer_collisions[3] = 32;
	}
	else if (hammer_frame==4)
	{
		hammer_collisions[0] = 0;
		hammer_collisions[2] = 0;
		hammer_collisions[1] = 48;
		hammer_collisions[3] = 30;
	}

	
	for (i=0; i<5; i++)
	{
		if ( (Player_X + 18 > hammer_x[i]) && (Player_X+2 < hammer_x[i] + hammer_collisions[1]) && (Player_Y + 24 > hammer_y[i]) && (Player_Y+4 < hammer_y[i]+hammer_collisions[3]) )
		{
			Trigger_Death();
		}
	}
}

void Platforms_manage()
{
	unsigned char i;
	
	if (begin == 1) for (i=0; i<2; i++) platforme_y[i] = platforme_y[i] + 1;
	
	for (i=0; i<2; i++)
	{
		if (platforme_y[i] > 240)
		{
			platform[i] = 0;
			platforme_y[i] = -32;
			platforme_x_right[i] = rand_a_b(100,300);
			platforme_x_left[i] = platforme_x_right[i] - 296 - rand_a_b(32,64);
		}
	}
	
	if ((platforme_y[1] < -15) && (platforme_y[0]-platforme_y[1] < 180)) platforme_y[1] = platforme_y[1] - 60;
	if ((platforme_y[0] < -15) && (platforme_y[1]-platforme_y[0] < 180)) platforme_y[0] = platforme_y[0] - 60;
}

void Show_Bottom_back()
{
	if (bottom_y < 350) Put_image(2, 0, bottom_y);
	
	if (begin == 1) 
	{
		if (bottom_y < 480) bottom_y = bottom_y + 1;
	}
}

void Show_Clouds()
{
	unsigned char i;

	Put_image(7, cloud_x[0], cloud_y[0]);
	Put_image(7, cloud_x[1], cloud_y[1]);
	
	if (begin == 1) for (i=0; i<2; i++) cloud_y[i] = cloud_y[i] + 1;
	
	if (cloud_y[0] > 240)
	{
	cloud_x[0] = rand_a_b(10,60);
	cloud_y[0] = rand_a_b(-160,-220);
	}
	
	if (cloud_y[1]  > 240)
	{
	cloud_x[1] = rand_a_b(185,220);
	cloud_y[1] = rand_a_b(-128,-220);
	}
}

void Show_Hammer()
{
	Put_sprite(6, hammer_x[0], hammer_y[0], 48, 32, hammer_frame );
	Put_sprite(6, hammer_x[1], hammer_y[1], 48, 32, hammer_frame );
	Put_sprite(6, hammer_x[2], hammer_y[2], 48, 32, hammer_frame );
	Put_sprite(6, hammer_x[3], hammer_y[3], 48, 32, hammer_frame );
	
	hammer_x[0] = 226+platforme_x_left[0];
	hammer_x[1] = platforme_x_right[0]-6;
	hammer_x[2] = 226+platforme_x_left[1];
	hammer_x[3] = platforme_x_right[1]-6;
	
	hammer_y[0] = platforme_y[0]+12;
	hammer_y[1] = platforme_y[0]+12;
	hammer_y[2] = platforme_y[1]+12;
	hammer_y[3] = platforme_y[1]+12;
	
	// This part animates the Hammer and it goes from right to left and etc... (Refer to hammer_spr.cel)
	if (hammer_facing == 0) // Hammer goes to right
	{ 
		hammer_anim_time = hammer_anim_time + 1;
		if (hammer_anim_time > 9)
		{
			hammer_frame = hammer_frame + 1;
				if (hammer_frame > 3)
				{
					hammer_frame = 4;
					hammer_facing = 1;
				}
			hammer_anim_time = 0;
		}
	}
	else if (hammer_facing == 1) // Hammer goes to left
	{ 
		hammer_anim_time = hammer_anim_time + 1;
		if (hammer_anim_time > 9)
		{
			hammer_frame = hammer_frame - 1;
				if (hammer_frame < 1)
				{
					hammer_frame = 0;
					hammer_facing = 0;
				}
			hammer_anim_time = 0;
		}
	}
	
}

void Show_Player()
{
	Put_sprite(5, Player_X, Player_Y, 20, 24, Frame_player + Facing_player );
	
	if (Player_X > 320-16) Trigger_Death();
	if (Player_X < 0+4) Trigger_Death();
	
	if (begin == 1)
	{
		if (Player_Y > 150) Player_Y = Player_Y - 1;
	}

	// If Player is falling
	if (state_player == 1)
	{
		Delay_Frame_player = Delay_Frame_player + 1;
		if (Delay_Frame_player > 2)
		{
			Frame_player = Frame_player + 1;
			if (Frame_player > 5) Frame_player = 5;
			Delay_Frame_player = 0;
		}
	}
	
}

void Move_player()
{
	
	if (Facing_player == 0)
	{
			if (A_state == 1) 
			{
				play_switch();
				Facing_player = 4;
				A_time = 0;
				A_state = 2;
				time_speed = 0;
				speed_player = 0;
			}
			Player_X = Player_X + speed_player;
	}
	else if (Facing_player == 4)
	{
			if (A_state == 1) 
			{
				play_switch();
				Facing_player = 0;
				A_time = 0;
				A_state = 2;
				time_speed = 0;
				speed_player = 0;
			}
			Player_X = Player_X - speed_player;
	}
	
	time_speed = time_speed + 1;
	if (time_speed > 2 && speed_player < 2)
	{
		speed_player = speed_player + 1;
		time_speed = 0;
	}
	else if (time_speed > 3 && speed_player < 4)
	{
		speed_player = speed_player + 1;
		time_speed = 0;
	}
	else if (time_speed > 6 && speed_player < 8)
	{
		speed_player = speed_player + 1;
		if (speed_player > 5) speed_player = 5;
		time_speed = 0;
	}

	if (speed_player == 0)
	{
		Frame_player = 0;
	}
	else if (speed_player < 2)
	{
		Frame_player = 1;
	}
	else if (speed_player < 3)
	{
		Frame_player = 2;
	}
	else if (speed_player < 5)
	{
		Frame_player = 3;
	}
	
}

void A_button()
{
	if (A_state == 0) // A not pressed , detect if A pressed
	{ 
		if (BUTTON.A==1)
		{
		A_state = 1;
		A_time = 0;
		}
	}
	else if (A_state == 1)  // A_state = 1 means that A was just pressed
	{
		A_time = A_time + 1;
		
		if (A_time > 1){
		A_state = 2;
		A_time = 0;
		}
	}
	else if (A_state == 2) // A_state = 2 means that A is hold
	{ 
		
		if (BUTTON.A==0)
		{
		A_state = 3;
		A_time = 0;
		}
	}
	else if (A_state == 3) // A_state = 3 means that A was released
	{ 
		A_time = A_time + 1;
		
		if (A_time > 1){
		A_state = 0;
		A_time = 0;
		}
	}
}

void Trigger_Death()
{
	if (death == 0 && begin == 1)
	{
		play_smack();
		begin = 0;
		death = 1;
	}
}


// Used for random coordinate for the clouds
short rand_a_b(short a, short b)
{
    return rand()%(b-a) +a;
}
