#ifndef GLOBALS_H

#define GLOBALS_H

#define MAX_PATH 260

typedef struct Index
{
	int order;
	char filename[MAX_PATH];
	long size;
	long long offset;
} Index;

typedef struct DragRecord
{
	int mouseX;
	int mouseY;

	int clickX;
	int clickY;

	int Xwhileclicked;
	int Ywhileclicked;

	SDL_bool mousepressed;
} DragRecord;

typedef struct GlobalParameters
{
	const int playerW;
	const int playerH;
} GlobalParameters;

typedef struct GlobalResources
{
	SDL_bool show_wavenum;

	SDL_Window* window;
	SDL_Renderer* rend;
	SDL_Texture* playerTexture;

	SDL_Texture* bulletTexture;
	SDL_Texture* enemybulletTexture;

	SDL_Texture* backgroundTexture;
	SDL_Texture* logoTexture;

	SDL_Texture* enemyTexture1;
	SDL_Texture* enemyTexture2;
	SDL_Texture* enemyTexture3;
	SDL_Texture* enemyTexture4;
	SDL_Texture* enemyTexture5;

	SDL_Texture* bossTexture;

	SDL_Texture* mistTexture;

	SDL_Texture* bombTexture;

	SDL_Texture* explosionTexture;

	SDL_Texture* skill1Texture;
	SDL_Texture* skill2Texture;

	SDL_Texture* ragebar_fullTexture;
	SDL_Texture* ragebar_empetyTexture;
	SDL_Texture* ragebar_lightTexture;

	SDL_Texture* bosslifebar_fullTexture;
	SDL_Texture* bosslifebar_emptyTexture;

	SDL_Texture* bombexplosionTexture;

	SDL_Texture* bossbullet1Texture;
	SDL_Texture* bossbullet2Texture;
	SDL_Texture* bossbullet3Texture;

	SDL_Texture* shortattackTexture;

	SDL_Texture* lifelabelSurface;
	SDL_Texture* scorelabelSurface;
	SDL_Texture* noneSurface;
	SDL_Texture* pausebuttonTexture;

	SDL_Rect* box_player;
	SDL_Rect* ragebar_empetyRect;
	SDL_Rect* ragebar_fullRect;
	SDL_Rect* ragebar_fullCut;

	SDL_Rect* bosslifebar_emptyRect;
	SDL_Rect* bosslifebar_fullRect;
	SDL_Rect* bosslifebar_fullCut;

	SDL_Texture* wavenumberTexture;

	SDL_Rect* wavenumberRect;

	SDL_Cursor* cursor_hand;
	SDL_Cursor* cursor_arrow;

	Mix_Chunk* startMusic;
	Mix_Chunk* gameMusic;

	Mix_Chunk* buttonclickSound;
	Mix_Chunk* shootSound;
	Mix_Chunk* laserSound;
	Mix_Chunk* explosionSound;
	Mix_Chunk* bombexplosionSound;

	SDL_RWops* pak;

	SDL_Surface* icon;

	Index* p_index;
	int filenum;

} GlobalResources;

#endif

extern GlobalParameters;

extern GlobalResources;