/*
	Copyright (C) 2015, Gameblabla

	Licensed under the GAMEBLABLA's LICENSE version 3.1.
	See COPYING file for more details.

	If a COPYING file was not provided, you may obtain a copy of the License at
		http://gameblabla.olympe.in/license/

	This Program is distributed without WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GAMEBLABLA's LICENSE for more details.
*/

/*
 * The source code could be improved 
 * I could improve how the code is structured
 * I can also improve Input
 * 
 * At least it works
*/

#include "WRAPPER/API.h"
#include "WRAPPER/INPUT.h"
#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

short right_spot[5], left_spot[5], up_spot[5], down_spot[5];

short CaseX;
short CaseY;
short Case_pos;
short Case_COLOR = 1;

short reload = 0;
short gamemo = 0;
short frame_gamemo = 0;
char good = 1;

char lives = 3;
unsigned long score = 0;
unsigned long highscore = 0;

short gamemode = 0;
char score_showed[7] , conv_time[4], highscore_show[7];
short rtime , lowtime = 10;
short go_time = 0;

unsigned int game_id = 0;
FILE* fp2;

unsigned short check_square_score(unsigned char sqr1, unsigned char sqr2, unsigned char sqr3, unsigned char sqr4);

int main(int argc, char* argv[])
{
	game_name = "KillMinds";
	Init_video(argv);
	InitializeGame();
	srand(time(NULL));

	while (!done)
	{
		Clear_screen();
		
		Controls();

		GameLoop();

		Update_video();
	}

	Clearing();

	return 0;
}

void InitializeGame()
{
	unsigned char i;

	Load_Image(1,title_img);
	Load_Sounds();
	add_allsprites();

	CaseX = 141;
	CaseY = 117;
	
	for (i=0;i<7;i++)
	{
		score_showed[i] = 0;
		highscore_show[i] = 0;
	}
	
	for (i=0;i<4;i++)
	{
		conv_time[i] = 0;
	}

	Load_score();
	reset_case();
}

void GameLoop()
{
	Put_image(1,0,0);

	if (BUTTON.QUIT) done = 1;

	switch(gamemode)
	{
		case 0:
			Titlescreen();
			Show_highscore();
			Show_startbutton();
		break;

		case 1:
			Show_Square();
			if (gamemo == 0) Move_Square();
			Show_Game();
		break;
	}
}

void Titlescreen()
{
		if (BUTTON.START == 1 || BUTTON.A)
		{
			go_ingame();
		}
}

void Show_highscore()
{
	Put_sprite(5, 49, 200, 16, 16, highscore_show[0]);
	Put_sprite(6, 49+12, 200, 16, 16, highscore_show[1]);
	Put_sprite(7, 49+24, 200, 16, 16, highscore_show[2]);
	Put_sprite(8, 49+36, 200, 16, 16, highscore_show[3]);
	Put_sprite(9, 49+48, 200, 16, 16, highscore_show[4]);
	Put_sprite(10, 49+60, 200, 16, 16, highscore_show[5]);
	Put_sprite(11, 49+72, 200, 16, 16, highscore_show[6]);
}

void Show_startbutton()
{
	static unsigned char time_press = 0;
	
	time_press++;
	
	if (time_press < 30)
	{
		Put_image(40, 90, 160);
	}
	else if (time_press > 59) 
	{
		time_press = 0;
	}
}

void Show_Square()
{
	static short frame = 0;

	Put_sprite(2, CaseX, CaseY, 40, 38, frame);

	switch(gamemo)
	{
		case 2:
			go_time++;
			
			// Show "Game Over" frame
			frame = 14;

			if (go_time > 180) go_back_titlescreen();
		break;
		
		case 1:
			frame = 13;

			if (frame_gamemo > 35)
			{
				gamemo = 0;
				frame_gamemo = 0;
			}
			else frame_gamemo++;
		break;
		
		case 0:
			// Show the square you're holding
			frame = Case_pos;
		break;
	}


}

void Show_Game()
{
	unsigned char i;
	
	rtime = reload;

	conv_time[0] = (rtime / 100) % 10;
	conv_time[1] = (rtime / 10) % 10;
	conv_time[2] = rtime % 10;

	if (rtime < 33) 
	{
		lowtime = 10;
	}
	else 
	{
		lowtime = 0;
	}
	
	//	Score_0 ,..., Score_6
	for (i=0;i<7;i++)
	{
		Put_sprite(5, 49+(i*12), 6, 16, 16, score_showed[i]);
	}
	
	
	//	Lives_spr
	Put_sprite(12, 272, 6, 16, 16, lives);

	//	Time_0
	Put_sprite(13, 180, 6, 16, 16, conv_time[0]+lowtime);

	//	Time_1
	Put_sprite(14, 180+16, 6, 16, 16, conv_time[1]+lowtime);

	//	Time_2
	Put_sprite(14, 180+32, 6, 16, 16, conv_time[2]+lowtime);

	// Spr_ready
	if (reload > 100)
	{
		Put_image(3, 111, 120);
	}

	/* 
	 * The following put the squares on screen
	 * If the square is empty then it is not shown
	*/

	for (i=0;i<4;i++)
	{
		if (left_spot[i] != 0) Put_sprite(18+i, 77, 117, 40, 38, left_spot[i]);
		
		if (right_spot[i] != 0) Put_sprite(22+i, 205, 117, 40, 38, right_spot[i]);
		
		if (up_spot[i] != 0) Put_sprite(26+i, 141, 53, 40, 38, up_spot[i]);
		
		if (down_spot[i] != 0) Put_sprite(26+i, 141, 181, 40, 38, down_spot[i]);
		
	}

}

unsigned short check_square_score(unsigned char sqr1, unsigned char sqr2, unsigned char sqr3, unsigned char sqr4)
{
	// Check if all squares are green
	if (sqr1 == 9 && sqr2 == 10 && sqr3 == 11 && sqr4 == 12)
		return 300;
	else if (sqr1 == 5 && sqr2 == 6 && sqr3 == 7 && sqr4 == 8)
		return 300;
	else if (sqr1 == 1 && sqr2 == 2 && sqr3 == 3 && sqr4 == 4)
		return 300;
	else
		return 30;
}

void Move_Square()
{
	unsigned char i;
	unsigned short sqr_score_filled;
	
	reload--;

	if (reload < 1)	// Player lose a live after 1.3 seconds
	{
		lose_a_life();
		reset_case();
	}
	else if (reload < 90) // Player can play after some ms
	{
		if (BUTTON.UP)
		{
			for(i=0;i<4;i++)
			{
				if ((Case_pos == 1+i || Case_pos == 5+i || Case_pos == 9+i || Case_pos == 13+i))
				{
					if (up_spot[i] == 0)
					{
						Play_SFX(3);
						up_spot[i] = Case_pos;
					}
					else if (up_spot[i] > 0)
					{
						lose_a_life();
					}
				}	
			}
			
			if (up_spot[0] > 0 && up_spot[1] > 0 && up_spot[2] > 0 && up_spot[3] > 0)
			{
				sqr_score_filled = check_square_score(up_spot[0], up_spot[1], up_spot[2], up_spot[3]);
				for(i=0;i<4;i++) 
				{
					up_spot[i] = 0;
				}
				score = score + sqr_score_filled;
				calcul_score();
			}

			reset_case();
		}

		else if (BUTTON.DOWN)
		{

			for(i=0;i<4;i++)
			{
				if ((Case_pos == 1+i || Case_pos == 5+i || Case_pos == 9+i || Case_pos == 13+i))
				{
					if (down_spot[i] == 0)
					{
						Play_SFX(3);
						down_spot[i] = Case_pos;
					}
					else if (down_spot[i] > 0)
					{
						lose_a_life();
					}
				}	
			}
			
			if (down_spot[0] > 0 && down_spot[1] > 0 && down_spot[2] > 0 && down_spot[3] > 0)
			{
				sqr_score_filled = check_square_score(down_spot[0], down_spot[1], down_spot[2], down_spot[3]);
				for(i=0;i<4;i++) 
				{
					down_spot[i] = 0;
				}
				score = score + sqr_score_filled;
				calcul_score();
			}

			reset_case();
		}

		else if (BUTTON.LEFT)
		{
			
			for(i=0;i<4;i++)
			{
				if ((Case_pos == 1+i || Case_pos == 5+i || Case_pos == 9+i || Case_pos == 13+i))
				{
					if (left_spot[i] == 0)
					{
						Play_SFX(3);
						left_spot[i] = Case_pos;
					}
					else if (left_spot[i] > 0)
					{
						lose_a_life();
					}
				}	
			}

			if (left_spot[0] > 0 && left_spot[1] > 0 && left_spot[2] > 0 && left_spot[3] > 0)
			{
				sqr_score_filled = check_square_score(left_spot[0], left_spot[1], left_spot[2], left_spot[3]);
				for(i=0;i<4;i++) 
				{
					left_spot[i] = 0;
				}
				score = score + sqr_score_filled;
				calcul_score();
			}

			reset_case();
		}

		else if (BUTTON.RIGHT)
		{
			
				for(i=0;i<4;i++)
				{
					if ((Case_pos == 1+i || Case_pos == 5+i || Case_pos == 9+i || Case_pos == 13+i))
					{
						if (right_spot[i] == 0)
						{
							Play_SFX(3);
							right_spot[i] = Case_pos;
						}
						else if (right_spot[i] > 0)
						{
							lose_a_life();
						}
					}	
				}
			
				if (right_spot[0] > 0 && right_spot[1] > 0 && right_spot[2] > 0 && right_spot[3] > 0)
				{
					sqr_score_filled = check_square_score(right_spot[0], right_spot[1], right_spot[2], right_spot[3]);
					for(i=0;i<4;i++) 
					{
						right_spot[i] = 0;
					}
					score = score + sqr_score_filled;
					calcul_score();
				}

			reset_case();
		}
	}

}

void RESET_EVERYTHING()
{
	unsigned char i;
	
	reload = 0;
	gamemo = 0;
	frame_gamemo = 0;
	go_time = 0;
	score = 0;
	rtime = 0;
	
	lowtime = 10;
	lives = 3;
	
	gamemode = 1;
	good = 1;
	
	for (i=0;i<4;i++)
	{
		right_spot[i] = 0;
		left_spot[i] = 0;
		up_spot[i] = 0;
		down_spot[i] = 0;
	}
	
	for (i=0;i<7;i++)
	{
		score_showed[i] = 0;
	}

}

// Generate a random number
short reset_case() 
{
			good = 1;

			// Avoid to give a square where it can't be placed anywhere
			// Loop until the game finds a good combinaison
			// In theory, the game could loop indefinitely but it never happened to me
			
			// If good = 0 then it means it found a good combinaison
			while (good)
			{
				Case_pos = rand_a_b(1,12);

				if (up_spot[0] > 0 && right_spot[0] > 0 && left_spot[0] > 0 && down_spot[0] && (Case_pos == 1 || Case_pos == 5 || Case_pos == 9 || Case_pos == 13)) 
				{
					good = 1;
				}
				else if (up_spot[1] > 0 && right_spot[1] > 0 && left_spot[1] > 0 && down_spot[1] && (Case_pos == 2 || Case_pos == 6 || Case_pos == 10 || Case_pos == 14)) 
				{
					good = 1;
				}
				else if (up_spot[2] > 0 && right_spot[2] > 0 && left_spot[2] > 0 && down_spot[2] && (Case_pos == 3 || Case_pos == 7 || Case_pos == 11 || Case_pos == 15)) 
				{
					good = 1;
				}
				else if (up_spot[3] > 0 && right_spot[3] > 0 && left_spot[3] > 0 && down_spot[3] && (Case_pos == 4 || Case_pos == 8 || Case_pos == 12 || Case_pos == 16)) 
				{
					good = 1;
				}
				else
				{
					good = 0;
				}
			}
			
			switch(Case_pos)
			{
				case 1:
				case 2:
				case 3:
				case 4:
					Case_COLOR = 1;
				break;
				
				case 5:
				case 6:
				case 7:
				case 8:
					Case_COLOR = 2;
				break;
				
				case 9:
				case 10:
				case 11:
				case 12:
					Case_COLOR = 3;
				break;
				
				case 13:
				case 14:
				case 15:
				case 16:
					Case_COLOR = 4;
				break;
			}

			reload = 100;
			frame_gamemo = 0;

	return 1;
}

void lose_a_life()
{
	    lives = lives - 1;

		if (lives < 1)
		{
			Play_SFX(1);
			gamemo = 2;
		}
		else
		{
			Play_SFX(2);
			gamemo = 1;
		}

}

void add_allsprites()
{

	unsigned char i;

	// Sprite_case
	Load_Image(2,all_img);

	// Sprite_ready
	Load_Image(3,ready_img);

	// Score_0, ... , Score_6, Lives_spr , Time_0, ... , Time_2
	Load_Image(5,score_img);
	
	for (i=0;i<=10;i++)
	{
		Copy_Image(5, 6+i);
	}

	// SPR_RIGHT_EMP, SPR_LEFT_EMP, SPR_UP_EMP, SPR_DOWN_EMP
	for (i=0;i<=16;i++)
	{
		Copy_Image(2, 18+i);
	}
	
	// Press start button
	Load_Image(40,start_img);
}

void calcul_score()
{
	score_showed[0] = (score / 1000000) % 10;
	score_showed[1] = (score / 100000) % 10;
	score_showed[2] = (score / 10000) % 10;
	score_showed[3] = (score / 1000) % 10;
	score_showed[4] = (score / 100) % 10;
	score_showed[5] = (score / 10) % 10;
	score_showed[6] = score % 10;
}

static unsigned long int next = 1;

int rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

short rand_a_b(short a, short b)
{
    return rand()%(b-a) +a;
}

void go_back_titlescreen()
{
	Clear_screen();
	Load_Image(1,title_img);
	
	Update_highscore();
	Load_score();

	gamemode = 0;
}

void go_ingame()
{
	Clear_screen();
	RESET_EVERYTHING();

	Load_Image(1,background_img);

	reload = 200;

	gamemode = 1;
}


void Load_Sounds()
{
	Init_sound();

	Load_SFX(1, gameover_sfx);
	Load_SFX(2, miss_sfx);
	Load_SFX(3, move_sfx);
}


/*
 * 
 * 
 * SAVE FEATURE (Not yet supported on 3DO) 
 * 
 * 
*/

#ifdef SAVE

	void Load_score()
	{
		FILE* file;
		file = NULL;
		char* directory;
		unsigned char i;
		
		#ifdef NSPIRE
			directory = "/documents/ndless/killminds.save.tns";
		#else
			directory = "killminds.save";
		#endif
		
		file = fopen(directory, "r+");
		
		if (file != NULL)
		{
			for(i=0;i<7;i++)
			{
				highscore_show[i] = fgetc(file);
			}
		}
		else
		{
			// If the file does not exist then create it
			file = fopen(directory, "w");
		}
		
		fclose(file);
		
	}

	void Save_score()
	{
		FILE* file;
		char* directory;
		unsigned char i;
		
		#ifdef NSPIRE
			directory = "/documents/ndless/killminds.save.tns";
		#else
			directory = "killminds.save";
		#endif
		
		highscore_show[0] = (highscore / 1000000) % 10;
		highscore_show[1] = (highscore / 100000) % 10;
		highscore_show[2] = (highscore / 10000) % 10;
		highscore_show[3] = (highscore / 1000) % 10;
		highscore_show[4] = (highscore / 100) % 10;
		highscore_show[5] = (highscore / 10) % 10;
		highscore_show[6] = highscore % 10;
		
		file = fopen(directory, "w+");
		
		for(i=0;i<7;i++)
		{
			fputc( highscore_show[i], file );
		}
		
		fclose(file);
	}
	
#else

	void Load_score()
	{
	}

	void Save_score()
	{
	}
	
#endif

void Update_highscore()
{
	if (score > highscore)
	{
		highscore = score;
			
		highscore_show[0] = (highscore / 1000000) % 10;
		highscore_show[1] = (highscore / 100000) % 10;
		highscore_show[2] = (highscore / 10000) % 10;
		highscore_show[3] = (highscore / 1000) % 10;
		highscore_show[4] = (highscore / 100) % 10;
		highscore_show[5] = (highscore / 10) % 10;
		highscore_show[6] = highscore % 10;
		Save_score();
	}
}
