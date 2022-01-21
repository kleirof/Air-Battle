#include "main.h"
#include "globals.h"
#include "game.h"
#include "skill.h"
#include "start.h"
#include "resource.h"

const int windowW = 600;
const int windowH = 800;

int sensitivity = 16l;
int musicVol = 128;
int soundVol = 128;

int musicchannel;

SDL_bool quit = SDL_FALSE;
SDL_bool game = SDL_FALSE;

SDL_bool trackmode = SDL_TRUE;

int playerlife = 5;
int score = 0;
int rage = 0;

static void Init(GlobalResources* globalresources);

static void Destroy(GlobalResources* globalresources);

double LinearVar(int t, int fromX, int toX, int fromT, int toT)
{
	return (((double)toX - fromX) / ((double)toT - fromT) * ((double)t - fromT) + fromX);
}

double SinVar(int t, int fromT, double fromX, double A, double cycle, double h)
{
	double frequency = 2 * PI / cycle;
	double primaryphase = SDL_asin((double)(fromX - h) / A) - frequency * fromT;
	return A * SDL_sin(frequency * t + primaryphase) + h;
}

double CalcComponentX(int x, int y, int dst_x, int dst_y)
{
	if (((double)dst_x - x) * ((double)dst_x - x) + ((double)dst_y - y) * ((double)dst_y - y) != 0)
	{
		return ((double)dst_x - x) / sqrt(((double)dst_x - x) * ((double)dst_x - x) + ((double)dst_y - y) * ((double)dst_y - y));
	}
	else if ((double)dst_x - x == 0)
	{
		return 0;
	}
	else if ((double)dst_y - y == 0)
	{
		return 1;
	}
}

double CalcComponentY(int x, int y, int dst_x, int dst_y)
{
	if (((double)dst_x - x) * ((double)dst_x - x) + ((double)dst_y - y) * ((double)dst_y - y) != 0)
	{
		return ((double)dst_y - y) / sqrt(((double)dst_x - x) * ((double)dst_x - x) + ((double)dst_y - y) * ((double)dst_y - y));
	}
	else if ((double)dst_y - y == 0)
	{
		return 0;
	}
	else if ((double)dst_x - x == 0)
	{
		return 1;
	}
}

void Init(GlobalResources* globalresources)
{
	globalresources->box_player = SDL_malloc(sizeof(SDL_Rect));
	globalresources->ragebar_empetyRect = SDL_malloc(sizeof(SDL_Rect));
	globalresources->ragebar_fullRect = SDL_malloc(sizeof(SDL_Rect));
	globalresources->ragebar_fullCut = SDL_malloc(sizeof(SDL_Rect));

	globalresources->bosslifebar_emptyRect = SDL_malloc(sizeof(SDL_Rect));
	globalresources->bosslifebar_fullRect = SDL_malloc(sizeof(SDL_Rect));
	globalresources->bosslifebar_fullCut = SDL_malloc(sizeof(SDL_Rect));

	globalresources->wavenumberRect = SDL_malloc(sizeof(SDL_Rect));
	
	srand((unsigned int)time(NULL));
	SDL_Init(SDL_INIT_VIDEO);

	globalresources->pak = SDL_RWFromFile(PAK_FILE, "rb");
	InitIndex(globalresources);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(CHANNEL_AMOUNT);
	
	globalresources->icon = GetPackedResource("pictures/icon.png", UNPACK_SURFACE, globalresources);

	globalresources->window = SDL_CreateWindow("Air Battle - by xlc", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		windowW, windowH, SDL_WINDOW_SHOWN);
	SDL_SetWindowIcon(globalresources->window, globalresources->icon);
	globalresources->rend = SDL_CreateRenderer(globalresources->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(globalresources->rend);

	globalresources->playerTexture = GetPackedResource("pictures/plane-big.png", UNPACK_TEXTURE, globalresources);
	globalresources->enemyTexture1 = GetPackedResource("pictures/plane-enemy.png", UNPACK_TEXTURE, globalresources);
	globalresources->enemyTexture2 = GetPackedResource("pictures/plane-enemy2.png", UNPACK_TEXTURE, globalresources);
	globalresources->enemyTexture3 = GetPackedResource("pictures/plane-enemy3.png", UNPACK_TEXTURE, globalresources);
	globalresources->enemyTexture4 = GetPackedResource("pictures/plane-enemy4.png", UNPACK_TEXTURE, globalresources);
	globalresources->enemyTexture5 = GetPackedResource("pictures/plane-enemy5.png", UNPACK_TEXTURE, globalresources);

	globalresources->bossTexture = GetPackedResource("pictures/boss.png", UNPACK_TEXTURE, globalresources);

	globalresources->bulletTexture = GetPackedResource("pictures/bullet.png", UNPACK_TEXTURE, globalresources);
	globalresources->enemybulletTexture = GetPackedResource("pictures/enemybullet.png", UNPACK_TEXTURE, globalresources);

	globalresources->backgroundTexture = GetPackedResource("pictures/background.png", UNPACK_TEXTURE, globalresources);
	SDL_SetTextureBlendMode(globalresources->backgroundTexture, SDL_BLENDMODE_BLEND);

	globalresources->logoTexture = GetPackedResource("pictures/logo.png", UNPACK_TEXTURE, globalresources);
	SDL_SetTextureBlendMode(globalresources->logoTexture, SDL_BLENDMODE_BLEND);

	globalresources->mistTexture = GetPackedResource("pictures/mistani.png", UNPACK_TEXTURE, globalresources);
	globalresources->explosionTexture = GetPackedResource("pictures/explosionani.png", UNPACK_TEXTURE, globalresources);

	globalresources->skill1Texture = GetPackedResource("pictures/skill1.png", UNPACK_TEXTURE, globalresources);
	globalresources->skill2Texture = GetPackedResource("pictures/skill2.png", UNPACK_TEXTURE, globalresources);

	globalresources->ragebar_fullTexture = GetPackedResource("pictures/ragebar-full.png", UNPACK_TEXTURE, globalresources);
	globalresources->ragebar_empetyTexture = GetPackedResource("pictures/ragebar-empety.png", UNPACK_TEXTURE, globalresources);
	globalresources->ragebar_lightTexture = GetPackedResource("pictures/ragebar-light.png", UNPACK_TEXTURE, globalresources);
	SDL_SetTextureBlendMode(globalresources->ragebar_lightTexture, SDL_BLENDMODE_BLEND);

	globalresources->bosslifebar_fullTexture = GetPackedResource("pictures/bosslifebar.png", UNPACK_TEXTURE, globalresources);
	globalresources->bosslifebar_emptyTexture = GetPackedResource("pictures/bosslifebar_empty.png", UNPACK_TEXTURE, globalresources);

	globalresources->bombexplosionTexture = GetPackedResource("pictures/nuclearexplosionani.png", UNPACK_TEXTURE, globalresources);

	globalresources->bossbullet1Texture = GetPackedResource("pictures/boss-bullet1.png", UNPACK_TEXTURE, globalresources);
	globalresources->bossbullet2Texture = GetPackedResource("pictures/boss-bullet2.png", UNPACK_TEXTURE, globalresources);
	globalresources->bossbullet3Texture = GetPackedResource("pictures/boss-bullet3.png", UNPACK_TEXTURE, globalresources);

	globalresources->shortattackTexture = GetPackedResource("pictures/shortattack.png", UNPACK_TEXTURE, globalresources);

	globalresources->bombTexture = GetPackedResource("pictures/bomb.png", UNPACK_TEXTURE, globalresources);

	globalresources->wavenumberTexture = GetPackedResource("pictures/none.png", UNPACK_TEXTURE, globalresources);

	globalresources->lifelabelSurface = GetPackedResource("pictures/lifelabel.png", UNPACK_SURFACE, globalresources);
	globalresources->scorelabelSurface = GetPackedResource("pictures/scorelabel.png", UNPACK_SURFACE, globalresources);
	globalresources->noneSurface = GetPackedResource("pictures/none.png", UNPACK_SURFACE, globalresources);
	globalresources->pausebuttonTexture = GetPackedResource("pictures/pausebutton.png", UNPACK_TEXTURE, globalresources);

	globalresources->cursor_hand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	globalresources->cursor_arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

	globalresources->startMusic = GetPackedResource("audios/start.mp3", UNPACK_CHUNK, globalresources);
	globalresources->gameMusic = GetPackedResource("audios/game.mp3", UNPACK_CHUNK, globalresources);

	globalresources->buttonclickSound = GetPackedResource("audios/buttonclick.wav", UNPACK_CHUNK, globalresources);
	globalresources->shootSound = GetPackedResource("audios/shoot.wav", UNPACK_CHUNK, globalresources);
	globalresources->laserSound = GetPackedResource("audios/laser.wav", UNPACK_CHUNK, globalresources);
	globalresources->explosionSound = GetPackedResource("audios/explosion.wav", UNPACK_CHUNK, globalresources);
	globalresources->bombexplosionSound = GetPackedResource("audios/bombexplosion.wav", UNPACK_CHUNK, globalresources);



	InitSkills();

	IMG_Quit();

	TTF_Init();
}

void Delay(uint32_t begin)
{
	uint32_t current = SDL_GetTicks();
	uint32_t cost = current - begin;
	uint32_t frame = 1000 / FRAMERATE;
	int delay = frame - cost;
	if (delay > 0)
	{
		SDL_Delay(delay);
	}
}

void Destroy(GlobalResources* globalresources)
{

	SDL_DestroyTexture(globalresources->bulletTexture);
	SDL_DestroyTexture(globalresources->playerTexture);
	SDL_DestroyTexture(globalresources->enemybulletTexture);
	SDL_DestroyTexture(globalresources->backgroundTexture);
	SDL_DestroyTexture(globalresources->logoTexture);
	SDL_DestroyTexture(globalresources->enemyTexture1);
	SDL_DestroyTexture(globalresources->enemyTexture2);
	SDL_DestroyTexture(globalresources->enemyTexture3);
	SDL_DestroyTexture(globalresources->enemyTexture4);
	SDL_DestroyTexture(globalresources->enemyTexture5);
	SDL_DestroyTexture(globalresources->bossTexture);
	SDL_DestroyTexture(globalresources->mistTexture);
	SDL_DestroyTexture(globalresources->bombTexture);
	SDL_DestroyTexture(globalresources->explosionTexture);
	SDL_DestroyTexture(globalresources->skill1Texture);
	SDL_DestroyTexture(globalresources->skill2Texture);
	SDL_DestroyTexture(globalresources->ragebar_fullTexture);
	SDL_DestroyTexture(globalresources->ragebar_empetyTexture);
	SDL_DestroyTexture(globalresources->ragebar_lightTexture);
	SDL_DestroyTexture(globalresources->bosslifebar_fullTexture);
	SDL_DestroyTexture(globalresources->bosslifebar_emptyTexture);
	SDL_DestroyTexture(globalresources->bombexplosionTexture);
	SDL_DestroyTexture(globalresources->bossbullet1Texture);
	SDL_DestroyTexture(globalresources->bossbullet2Texture);
	SDL_DestroyTexture(globalresources->bossbullet3Texture);
	SDL_DestroyTexture(globalresources->shortattackTexture);
	SDL_DestroyTexture(globalresources->wavenumberTexture);
	SDL_DestroyTexture(globalresources->pausebuttonTexture);
	SDL_FreeSurface(globalresources->lifelabelSurface);
	SDL_FreeSurface(globalresources->scorelabelSurface);
	SDL_FreeSurface(globalresources->noneSurface);

	SDL_FreeSurface(globalresources->icon);

	SDL_DestroyRenderer(globalresources->rend);
	SDL_DestroyWindow(globalresources->window);

	SDL_free(globalresources->p_index);
	globalresources->p_index = NULL;

	SDL_free(globalresources->box_player);
	SDL_free(globalresources->ragebar_empetyRect);
	SDL_free(globalresources->ragebar_fullCut);
	SDL_free(globalresources->ragebar_fullRect);
	SDL_free(globalresources->wavenumberRect);

	SDL_free(globalresources->bosslifebar_emptyRect);
	SDL_free(globalresources->bosslifebar_fullRect);
	SDL_free(globalresources->bosslifebar_fullCut);

	globalresources->box_player = NULL;
	globalresources->ragebar_empetyRect = NULL;
	globalresources->ragebar_fullCut = NULL;
	globalresources->ragebar_fullRect = NULL;
	globalresources->wavenumberRect = NULL;

	globalresources->bosslifebar_emptyRect = NULL;
	globalresources->bosslifebar_fullRect = NULL;
	globalresources->bosslifebar_fullCut = NULL;

	SDL_FreeCursor(globalresources->cursor_hand);
	SDL_FreeCursor(globalresources->cursor_arrow);

	Mix_FreeChunk(globalresources->startMusic);
	Mix_FreeChunk(globalresources->gameMusic);

	Mix_FreeChunk(globalresources->buttonclickSound);
	Mix_FreeChunk(globalresources->shootSound);
	Mix_FreeChunk(globalresources->laserSound);
	Mix_FreeChunk(globalresources->explosionSound);
	Mix_FreeChunk(globalresources->bombexplosionSound);

	SDL_RWclose(globalresources->pak);

	SDL_Quit();
}

int main(int argc, char* argv[])
{
	GlobalResources globalresources;

	Init(&globalresources);
	while (!quit)
	{
		StartEventLoop(&globalresources);
		GameEventLoop(&globalresources);
	}
	Destroy(&globalresources);

	return 0;
}