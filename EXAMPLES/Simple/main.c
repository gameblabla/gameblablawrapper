#include "WRAPPER/API.h"
#include "WRAPPER/INPUT.h"

int main ( int argc, char* argv[] )
{
    short x, y;

    x = 50;
    y = 50;

    Init_video();
    Load_Image(1,"yourimage.bmp");

    while (!done)
    {
        Clear_screen();

        Controls();

		Put_image(1, x,y);

		if (BUTTON.UP) y = y - 1;
		if (BUTTON.DOWN) y = y + 1;
		if (BUTTON.LEFT) x = x - 1;
		if (BUTTON.RIGHT) x = x + 1;
 		if (BUTTON.QUIT) done = 1;

        Update_video();

    } // end main loop

    Clearing();
    return 0;
}
