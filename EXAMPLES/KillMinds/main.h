
#ifdef THREEDO
	char* title_img = "data/title.cel";
	char* all_img = "data/all.cel";
	char* ready_img = "data/ready.cel";
	char* score_img = "data/score.cel";
	char* start_img = "data/startbutton.cel";
	char* background_img = "data/background.cel";
	
	char* gameover_sfx = "data/gameover.aiff";
	char* miss_sfx = "data/miss.aiff";
	char* move_sfx = "data/move.aiff";
#else
	char* title_img = "data/title.bmp";
	char* all_img = "data/all.bmp";
	char* ready_img = "data/ready.bmp";
	char* score_img = "data/score.bmp";
	char* start_img = "data/startbutton.bmp";
	char* background_img = "data/background.bmp";
	
	char* gameover_sfx = "data/gameover.wav";
	char* miss_sfx = "data/miss.wav";
	char* move_sfx = "data/move.wav";
#endif


// Initialize game
void InitializeGame();

// Game loop
void GameLoop();

// Show 1/4 chosen
void Show_Square();
void Move_Square();

void Show_highscore();
void Show_startbutton();

void Show_Game();

short reset_case();

void lose_a_life();
void add_allsprites();

void calcul_score();
void Titlescreen();
void RESET_EVERYTHING();

void go_back_titlescreen();
void go_ingame();

// Reset
void Reset();

void Load_Sounds();

short rand_a_b(short a, short b);

void Load_score();
void Save_score();

void Update_highscore();

int rand(void);
short rand_a_b(short a, short b);
