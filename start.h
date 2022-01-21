#include "main.h"
#include "globals.h"



typedef struct StartResources
{
	SDL_Texture* settinguiTexture;
	SDL_Texture* sensitivityTexture;
	SDL_Texture* slidersensTexture;
	SDL_Texture* closeTexture;

	SDL_Texture* musicTexture;
	SDL_Texture* slidermusTexture;

	SDL_Texture* soundTexture;
	SDL_Texture* slidersndTexture;

	SDL_Texture* helpuiTexture;
	SDL_Texture* closehelpTexture;

	SDL_Texture* keysTexture;
	SDL_Texture* texts1Texture;
	SDL_Texture* texts2Texture;
	SDL_Texture* texts3Texture;
	SDL_Texture* texts4Texture;
	SDL_Texture* texts5Texture;
} StartResources;

typedef struct StartParameters
{
	SDL_Rect* settingsuiRect;
	SDL_Rect* gearsensRect;
	SDL_Rect* closeRect;
	SDL_Rect* slidersensRect;
	int sensitivity_max;
	int sensitivity_min;

	SDL_Rect* slidermusRect;
	SDL_Rect* gearmusRect;

	int musicVol_max;
	int musicVol_min;

	SDL_Rect* slidersndRect;
	SDL_Rect* gearsndRect;

	int soundVol_max;
	int soundVol_min;

	SDL_Rect* helpuiRect;
	SDL_Rect* closehelpRect;
} StartParameters;

extern void StartEventLoop(GlobalResources* globalresources);

