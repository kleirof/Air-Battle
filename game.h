#include "main.h"
#include "globals.h"

typedef struct GameParameters
{
	int playerlX;
	int playerlY;
} GameParameters;

extern void GameEventLoop(GlobalResources* globalresources);