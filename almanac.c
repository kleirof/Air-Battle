#include "almanac.h"
#include "globals.h"
#include "resource.h"
#include "objects.h"

static void InitCards(GlobalResources* globalresources, int enemysize[][2]);

static void DestroyCards();

static void DecideCardPosition();

static void ShowCards(GlobalResources* globalresources);

static void SliderEvents(SDL_Event event, GlobalResources* globalresources, DragRecord* dragrecord, AlmanacParameters* parameters);

static void InitDescriptions(GlobalResources* globalresources);

static void DestroyDescriptions();

void AlmanacEventLoop(GlobalResources* globalresources)
{
	SDL_SetCursor(globalresources->cursor_arrow);

	const int almanacuiW = 550;
	const int almanacuiH = 750;

	const int closeW = 30;
	const int closeH = 30;

	const int back1W = 500;
	const int back1H = 470;

	const int back2W = 500;
	const int back2H = 180;

	const int imageWtime = 2;

	AlmanacParameters parameters;

	parameters.autoslideleft = SDL_FALSE;
	parameters.autoslideright = SDL_FALSE;
	parameters.dragging = SDL_FALSE;
	parameters.draw = SDL_FALSE;

	const int w = 460;

	parameters.relaX0 = 0;
	parameters.t1 = 0;

	parameters.ID = 0;

	parameters.mouse_on_cards = SDL_FALSE;

	int enemysize[CARD_AMOUNT_MAX][2] = { {80,70},{65,50},{40,30},{75,25},{65,33},{89,89} };

	InitCards(globalresources, enemysize);
	InitDescriptions(globalresources);

	SDL_Rect stageRect = { 80, 130, 240, 300 };
	SDL_Rect sizeRect = { 360, 120, 300, 75 };
	SDL_Rect speedRect = { 360, 250, 300, 75 };

	SDL_Rect almanacuiRect = { windowW / 2 - almanacuiW / 2, windowH / 2 - almanacuiH / 2, almanacuiW, almanacuiH };
	SDL_Rect closeRect = { almanacuiRect.x + almanacuiRect.w - 40 - closeW / 2, almanacuiRect.y + 40 - closeH / 2, closeW, closeH };
	SDL_Rect back1Rect = { almanacuiRect.x + almanacuiRect.w / 2 - back1W / 2, almanacuiRect.y + 80, back1W, back1H };
	SDL_Rect back2Rect = { almanacuiRect.x + almanacuiRect.w / 2 - back2W / 2, almanacuiRect.y + 560, back2W, back2H };

	SDL_Rect sizecontentRect = { 440 - 400 / 2, 180, 400, 100 };
	SDL_Rect speedcontentRect = { 440 - 400 / 2, 310, 400, 100 };
	SDL_Rect detailsRect = { windowW / 2 - 450 / 2, 450, 450, 100 };

	SDL_Rect imageRect;

	uint32_t begin = SDL_GetTicks();

	uint32_t t0 = begin;

	int mouseX = 0;
	int mouseY = 0;

	imageRect.w = enemysize[parameters.ID][0] * imageWtime;
	imageRect.h = enemysize[parameters.ID][1] * imageWtime;
	imageRect.x = 200 - imageRect.w / 2;
	imageRect.y = SinVar(begin, t0, 285 - imageRect.h / 2, 20, 2000, 285 - imageRect.h / 2);

	SDL_Texture* stageTexture = GetPackedResource("pictures/card.png", UNPACK_TEXTURE, globalresources);
	SDL_Texture* sizeTexture = CreateTexturewithBlitedText("size", 40, globalresources, FONT_PATH, 0, 0, 0, 255, "pictures/none.png", 0, 0, ALIGN_LEFT, ALIGN_CENTER);
	SDL_Texture* speedTexture = CreateTexturewithBlitedText("speed", 40, globalresources, FONT_PATH, 0, 0, 0, 255, "pictures/none.png", 0, 0, ALIGN_LEFT, ALIGN_CENTER);


	SDL_Texture* almanacuiTexture = CreateTexturewithBlitedText("Almanac", 60, globalresources, FONT_PATH, 0, 0, 0, 255, "pictures/almanacui.png", 0, -505, ALIGN_CENTER, ALIGN_CENTER);
	SDL_Texture* closeTexture = GetPackedResource("pictures/close.png", UNPACK_TEXTURE, globalresources);
	SDL_Texture* back1Texture = GetPackedResource("pictures/almanacuiback.png", UNPACK_TEXTURE, globalresources);
	SDL_Texture* back2Texture = GetPackedResource("pictures/almanacuiback2.png", UNPACK_TEXTURE, globalresources);

	SDL_Texture* sizecontentTexture = CreateTexturewithBlitedText(descriptions[parameters.ID].size, 40, globalresources, FONT_PATH, 39, 95, 56, 255, "pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);;
	SDL_Texture* speedcontentTexture= CreateTexturewithBlitedText(descriptions[parameters.ID].speed, 40, globalresources, FONT_PATH, 39, 95, 56, 255, "pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
	SDL_Texture* detailsTexture = CreateTexturewithBlitedText(descriptions[parameters.ID].details, 30, globalresources, FONT_PATH, 39, 95, 56, 255, "pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);

	SDL_bool almanac = SDL_TRUE;

	DragRecord dragrecord = { 0, 0, 0, 0, 0, 0, SDL_FALSE };

	SDL_Event event;
	while (almanac == SDL_TRUE && quit == SDL_FALSE)
	{
		begin = SDL_GetTicks();
		parameters.mouse_on_cards = SDL_FALSE;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = SDL_TRUE;
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (event.button.x >= closeRect.x && event.button.x <= closeRect.x + closeRect.w
					&& event.button.y >= closeRect.y && event.button.y <= closeRect.y + closeRect.h)
				{
					almanac = SDL_FALSE;
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.x >= closeRect.x && event.button.x <= closeRect.x + closeRect.w
					&& event.button.y >= closeRect.y && event.button.y <= closeRect.y + closeRect.h)
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
				}
			}
			SliderEvents(event, globalresources, &dragrecord, &parameters);
			if (event.type == SDL_MOUSEMOTION)
			{
				dragrecord.mouseX = event.motion.x;
				dragrecord.mouseY = event.motion.y;
			}
		}

		int i;
		for (i = 0; i < CARD_AMOUNT_MAX; i++)
		{
			if (dragrecord.mouseX >= cards[i].rect->x && dragrecord.mouseX <= cards[i].rect->x + cards[i].rect->w
				&& dragrecord.mouseY >= cards[i].rect->y && dragrecord.mouseY <= cards[i].rect->y + cards[i].rect->h)
			{
				parameters.mouse_on_cards = SDL_TRUE;
				break;
			}

		}

		if ((dragrecord.mouseX >= closeRect.x && dragrecord.mouseX <= closeRect.x + closeRect.w
			&& dragrecord.mouseY >= closeRect.y && dragrecord.mouseY <= closeRect.y + closeRect.h)
			||
			parameters.mouse_on_cards == SDL_TRUE)
		{
			SDL_SetCursor(globalresources->cursor_hand);
		}
		else
		{
			SDL_SetCursor(globalresources->cursor_arrow);
		}

		if (parameters.autoslideleft == SDL_TRUE && parameters.dragging == SDL_FALSE)
		{
			uint32_t t = SDL_GetTicks();
			int i;
			for (i = 0; i < CARD_AMOUNT_MAX; i++)
			{
				cards[i].relaX = LinearVar(t, parameters.relaX0, 0, parameters.t1, parameters.t1 + 300) + i * 140;
			}
			if (cards[0].relaX <= 0)
			{
				int i;
				for (i = 0; i < CARD_AMOUNT_MAX; i++)
				{
					cards[i].relaX = i * 140;
				}
				parameters.autoslideleft = SDL_FALSE;
			}
		}

		if (parameters.autoslideright == SDL_TRUE && parameters.dragging == SDL_FALSE)
		{
			uint32_t t = SDL_GetTicks();
			int i;
			for (i = 0; i < CARD_AMOUNT_MAX; i++)
			{
				cards[i].relaX = LinearVar(t, parameters.relaX0, w - (CARD_AMOUNT_MAX - 1) * 140 - cards[0].w, parameters.t1, parameters.t1 + 300) + i * 140;
			}
			if (cards[CARD_AMOUNT_MAX - 1].relaX + cards[CARD_AMOUNT_MAX - 1].w >= w)
			{
				int i;
				for (i = 0; i < CARD_AMOUNT_MAX; i++)
				{
					cards[i].relaX = w - cards[0].w - (CARD_AMOUNT_MAX - 1) * 140 + i * 140;
				}
				parameters.autoslideright = SDL_FALSE;
			}
		}

		if (parameters.draw == SDL_TRUE)
		{

			SDL_DestroyTexture(sizecontentTexture);
			SDL_DestroyTexture(speedcontentTexture);
			SDL_DestroyTexture(detailsTexture);

			sizecontentTexture = CreateTexturewithBlitedText(descriptions[parameters.ID].size, 40, globalresources, FONT_PATH, 39, 95, 56, 255, "pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
			speedcontentTexture = CreateTexturewithBlitedText(descriptions[parameters.ID].speed, 40, globalresources, FONT_PATH, 39, 95, 56, 255, "pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
			detailsTexture = CreateTexturewithBlitedText(descriptions[parameters.ID].details, 30, globalresources, FONT_PATH, 39, 95, 56, 255, "pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);

			parameters.draw = SDL_FALSE;
		}

		imageRect.w = enemysize[parameters.ID][0] * imageWtime;
		imageRect.h = enemysize[parameters.ID][1] * imageWtime;
		imageRect.x = 200 - imageRect.w / 2;
		imageRect.y = SinVar(begin, t0, 285 - imageRect.h / 2, 20, 2000, 285 - imageRect.h / 2);


		DecideCardPosition();

		SDL_RenderCopy(globalresources->rend, globalresources->backgroundTexture, NULL, NULL);
		SDL_RenderCopy(globalresources->rend, almanacuiTexture, NULL, &almanacuiRect);
		SDL_RenderCopy(globalresources->rend, closeTexture, NULL, &closeRect);

		SDL_RenderCopy(globalresources->rend, back1Texture, NULL, &back1Rect);
		SDL_RenderCopy(globalresources->rend, back2Texture, NULL, &back2Rect);

		SDL_RenderCopy(globalresources->rend, stageTexture, NULL, &stageRect);

		SDL_RenderCopy(globalresources->rend, sizeTexture, NULL, &sizeRect);
		SDL_RenderCopy(globalresources->rend, speedTexture, NULL, &speedRect);

		SDL_RenderCopy(globalresources->rend, sizecontentTexture, NULL, &sizecontentRect);
		SDL_RenderCopy(globalresources->rend, speedcontentTexture, NULL, &speedcontentRect);
		SDL_RenderCopy(globalresources->rend, detailsTexture, NULL, &detailsRect);

		SDL_RenderCopy(globalresources->rend, descriptions[parameters.ID].tex, NULL, &imageRect);

		ShowCards(globalresources);

		SDL_RenderPresent(globalresources->rend);

		Delay(begin);
		SDL_RenderClear(globalresources->rend);
	}

	DestroyCards();
	DestroyDescriptions();

	SDL_DestroyTexture(stageTexture);
	SDL_DestroyTexture(sizeTexture);
	SDL_DestroyTexture(speedTexture);

	SDL_DestroyTexture(almanacuiTexture);
	SDL_DestroyTexture(closeTexture);
	SDL_DestroyTexture(back1Texture);
	SDL_DestroyTexture(back2Texture);

	SDL_DestroyTexture(sizecontentTexture);
	SDL_DestroyTexture(speedcontentTexture);
	SDL_DestroyTexture(detailsTexture);
	SDL_SetCursor(globalresources->cursor_arrow);
}

void InitCards(GlobalResources* globalresources, int enemysize[][2])
{
	const int cardW = 120;
	const int cardH = 150;

	SDL_Surface* tmpsurf[CARD_AMOUNT_MAX];
	SDL_Surface* tmpsrcsurf[CARD_AMOUNT_MAX];

	int i;
	for (i = 0; i < CARD_AMOUNT_MAX; i++)
	{
		cards[i].ID = i;
		cards[i].relaX = 140 * i;
		cards[i].relaY = 0;
		cards[i].w = cardW;

		cards[i].rect = (SDL_Rect*)SDL_malloc(sizeof(SDL_Rect));
		cards[i].rect->h = cardH;

		cards[i].cut = (SDL_Rect*)SDL_malloc(sizeof(SDL_Rect));
		cards[i].cut->h = cardH * 2;

		tmpsurf[i] = GetPackedResource("pictures/card.png", UNPACK_SURFACE, globalresources);
	}
	tmpsrcsurf[0] = GetPackedResource("pictures/plane-enemy.png", UNPACK_SURFACE, globalresources);
	tmpsrcsurf[1] = GetPackedResource("pictures/plane-enemy2.png", UNPACK_SURFACE, globalresources);
	tmpsrcsurf[2] = GetPackedResource("pictures/plane-enemy3.png", UNPACK_SURFACE, globalresources);
	tmpsrcsurf[3] = GetPackedResource("pictures/plane-enemy4.png", UNPACK_SURFACE, globalresources);
	tmpsrcsurf[4] = GetPackedResource("pictures/plane-enemy5.png", UNPACK_SURFACE, globalresources);
	tmpsrcsurf[5] = GetPackedResource("pictures/boss.png", UNPACK_SURFACE, globalresources);
	for (i = 0; i < CARD_AMOUNT_MAX; i++)
	{
		int srcW = 80;
		int srcH = (double)srcW / enemysize[i][0] * enemysize[i][1];

		SDL_Rect dstRect = { (cardW / 2 - srcW / 2) * 2, (cardH / 2 - srcH / 2) * 2, srcW * 2, srcH * 2 };

		SDL_BlitScaled(tmpsrcsurf[i], NULL, tmpsurf[i], &dstRect);
		cards[i].tex = SDL_CreateTextureFromSurface(globalresources->rend, tmpsurf[i]);
		SDL_FreeSurface(tmpsurf[i]);
		SDL_FreeSurface(tmpsrcsurf[i]);
	}

}

void DestroyCards()
{
	int i;
	for (i = 0; i < CARD_AMOUNT_MAX; i++)
	{
		SDL_free(cards[i].rect);
		cards[i].rect = NULL;
		SDL_free(cards[i].cut);
		cards[i].cut = NULL;
		SDL_DestroyTexture(cards[i].tex);
	}
}

void ShowCards(GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < CARD_AMOUNT_MAX; i++)
	{
		SDL_RenderCopy(globalresources->rend, cards[i].tex, cards[i].cut, cards[i].rect);
	}
}

void DecideCardPosition()
{
	const int w = 460;
	const int x0 = windowW / 2 - w / 2;
	const int y0 = 600;

	int i;
	for (i = 0; i < CARD_AMOUNT_MAX; i++)
	{
		cards[i].rect->y = y0 + cards[i].relaY;
		cards[i].cut->y = cards[i].relaY;
		if (cards[i].relaX > 0 && cards[i].relaX + cards[i].w < w)
		{
			cards[i].rect->x = x0 + cards[i].relaX;
			cards[i].rect->w = cards[i].w;
			cards[i].cut->x = 0;
			cards[i].cut->w = cards[i].w * 2;
		}
		else if (cards[i].relaX <= 0 && cards[i].relaX + cards[i].w > 0)
		{
			cards[i].rect->x = x0;
			cards[i].rect->w = cards[i].w + cards[i].relaX;
			cards[i].cut->x = -cards[i].relaX * 2;
			cards[i].cut->w = (cards[i].w + cards[i].relaX) * 2;
		}
		else if (cards[i].relaX + cards[i].w <= 0)
		{
			cards[i].rect->x = x0 + cards[i].relaX;
			cards[i].rect->w = 0;
			cards[i].cut->x = 0;
			cards[i].cut->w = 0;
		}
		else if (cards[i].relaX + cards[i].w >= w && cards[i].relaX < w)
		{
			cards[i].rect->x = x0 + cards[i].relaX;
			cards[i].rect->w = w - cards[i].relaX;
			cards[i].cut->x = 0;
			cards[i].cut->w = (w - cards[i].relaX) * 2;
		}
		else if (cards[i].relaX >= w)
		{
			cards[i].rect->x = x0 + cards[i].relaX;
			cards[i].rect->w = 0;
			cards[i].cut->x = 0;
			cards[i].cut->w = 0;
		}
	}
}

void SliderEvents(SDL_Event event, GlobalResources* globalresources, DragRecord* dragrecord, AlmanacParameters* parameters)
{
	const int w = 460;
	const int x0 = windowW / 2 - w / 2;
	const int y0 = 600;
	const int h = 150;

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.x >= x0 && event.button.x <= x0 + w
			&& event.button.y >= y0 && event.button.y <= y0 + h)
		{
			parameters->dragging = SDL_TRUE;
			if (dragrecord->mousepressed == SDL_FALSE)
			{
				dragrecord->clickX = event.button.x;
				dragrecord->clickY = event.button.y;

				dragrecord->Xwhileclicked = cards[0].relaX;
			}
			dragrecord->mousepressed = SDL_TRUE;
		}
	}
	if (event.type == SDL_MOUSEBUTTONUP)
	{

		if (cards[0].relaX > 0)
		{
			parameters->relaX0 = cards[0].relaX;
			parameters->t1 = SDL_GetTicks();
			parameters->autoslideleft = SDL_TRUE;
		}
		if (cards[CARD_AMOUNT_MAX - 1].relaX + cards[CARD_AMOUNT_MAX - 1].w < w)
		{
			parameters->relaX0 = cards[0].relaX;
			parameters->t1 = SDL_GetTicks();
			parameters->autoslideright = SDL_TRUE;
		}

		if (dragrecord->mousepressed == SDL_TRUE && event.button.x == dragrecord->clickX && event.button.y == dragrecord->clickY)
		{
			int i;
			for (i = 0; i < CARD_AMOUNT_MAX; i++)
			{
				if (event.button.x >= cards[i].rect->x && event.button.x <= cards[i].rect->x + cards[i].rect->w
					&& event.button.y >= cards[i].rect->y && event.button.y <= cards[i].rect->y + cards[i].rect->h)
				{
					parameters->ID = cards[i].ID;
					parameters->draw = SDL_TRUE;
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
					break;
				}
			}
		}
		dragrecord->mousepressed = SDL_FALSE;
		parameters->dragging = SDL_FALSE;
	}
	if (event.type == SDL_MOUSEMOTION)
	{
		dragrecord->mouseX = event.motion.x;
		dragrecord->mouseY = event.motion.y;

		
		if (dragrecord->mousepressed == SDL_TRUE)
		{
			int i;
			for (i = 0; i < CARD_AMOUNT_MAX; i++)
			{
				cards[i].relaX = dragrecord->Xwhileclicked + dragrecord->mouseX - dragrecord->clickX + i * 140;
			}
		}
	}
}

void InitDescriptions(GlobalResources* globalresources)
{
	Description description_1 = { 0, "big", "medium", "A common type of plane."};
	Description description_2 = { 1, "small", "medium", "Faster somehow." };
	Description description_3 = { 2, "smaller", "slow", "Not a mosquito."};
	Description description_4 = { 3, "big", "fast", "Speed up!"};
	Description description_5 = { 4, "big", "fast", "Just drunk."};
	Description description_6 = { 5, "huge", "medium", "It can shrink." };

	SDL_memcpy(&descriptions[0], &description_1, sizeof(Description));
	SDL_memcpy(&descriptions[1], &description_2, sizeof(Description));
	SDL_memcpy(&descriptions[2], &description_3, sizeof(Description));
	SDL_memcpy(&descriptions[3], &description_4, sizeof(Description));
	SDL_memcpy(&descriptions[4], &description_5, sizeof(Description));
	SDL_memcpy(&descriptions[5], &description_6, sizeof(Description));

	descriptions[0].tex = GetPackedResource("pictures/plane-enemy.png", UNPACK_TEXTURE, globalresources);
	descriptions[1].tex = GetPackedResource("pictures/plane-enemy2.png", UNPACK_TEXTURE, globalresources);
	descriptions[2].tex = GetPackedResource("pictures/plane-enemy3.png", UNPACK_TEXTURE, globalresources);
	descriptions[3].tex = GetPackedResource("pictures/plane-enemy4.png", UNPACK_TEXTURE, globalresources);
	descriptions[4].tex = GetPackedResource("pictures/plane-enemy5.png", UNPACK_TEXTURE, globalresources);
	descriptions[5].tex = GetPackedResource("pictures/boss.png", UNPACK_TEXTURE, globalresources);
}

void DestroyDescriptions()
{
	int i;
	for (i = 0; i < CARD_AMOUNT_MAX; i++)
	{
		SDL_DestroyTexture(descriptions[i].tex);
	}
}

