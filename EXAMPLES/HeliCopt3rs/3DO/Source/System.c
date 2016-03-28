// RasterSoft Invades 3DO demo by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#include <string.h>

#include <controlpad.h>
#include <event.h>
#include <mem.h>

#include "system.h"

// Number of control pads to use
static const int	sNumControlPads	= 2;

bool InitializeControlPads()
{

	// Initialize the control pads
	if (InitControlPad(sNumControlPads) <  0)
	{
	
		return	false;
	}

	return	true;
}

void DestroyControlPads()
{
	KillControlPad();
}
