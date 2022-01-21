#include "main.h"
#include "globals.h"

#ifndef ALMANAC_H

#define ALMANAC_H

typedef struct Card
{
	int ID;
	int relaX, relaY;
	int w;
	SDL_Rect* rect;
	SDL_Rect* cut;
	SDL_Texture* tex;
} Card;

typedef struct AlmanacParameters
{
	SDL_bool autoslideleft;
	SDL_bool autoslideright;
	uint32_t t1;
	int relaX0;
	SDL_bool dragging;
	SDL_bool draw;
	int ID;
	SDL_bool mouse_on_cards;
} AlmanacParameters;

typedef struct Description
{
	int ID;
	char size[10];
	char speed[10];
	char details[32];
	SDL_Texture* tex;
} Description;

#endif

Card cards[CARD_AMOUNT_MAX];

Description descriptions[CARD_AMOUNT_MAX];

extern void AlmanacEventLoop(GlobalResources* globalresources);