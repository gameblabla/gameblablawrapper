extern unsigned short done;
extern struct input BUTTON;
extern const unsigned short FPS;
extern char* game_name;

extern void Init_video();

extern void Load_Image(unsigned short a, char* directory);
extern void Copy_Image(unsigned short a, unsigned short i);

extern void Put_image(unsigned short a, short x, short y);
extern void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f);

extern void Clear_Image(unsigned short a);
extern void Clear_Images();

extern void Clear_screen();

extern void Sync_refresh();
extern void Update_video();

extern void Controls();
extern void Clearing();

extern void Init_sound();

extern void Load_Music(char* directory);
extern void Play_Music(char loop);
extern void Clean_Music();

extern void Load_GFX(unsigned char i, char* directory);
extern void Play_GFX(unsigned char i);
extern void Unload_GFX();

/*						 
	WARNING : DANGEROUS !
    DON'T USE OR RELY ON THEM  
	IF YOU ARE HEAVILY USING 
	ALLEGRO 5.0 OR SDL 2.0 !
					          */

extern void Faster_clearing(short x, short y, unsigned short w, unsigned short h);
extern void Faster_update(short x, short y, short w, short h);


