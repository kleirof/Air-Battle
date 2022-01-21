#include "game.h"
#include "objects.h"
#include "skill.h"
#include "wave.h"
#include "globals.h"
#include "resource.h"
#include "boss.h"

static void PauseEventLoop(GlobalResources* globalresources);

static void OverEventLoop(GlobalResources* globalresources);

static void IfLose(GlobalResources* globalresources);

void PauseEventLoop(GlobalResources* globalresources)
{
	ObjectsParameters objectsparameters = { 10, 20, 10, 20 };

	const int pauseuiW = 300;
	const int pauseuiH = 120;

	const int resumeW = 90;
	const int resumeH = 30;

	const int leaveW = 90;
	const int leaveH = 30;

	const int lifelabelW = 90;
	const int lifelabelH = 37;

	const int scorelabelW = 120;
	const int scorelabelH = 37;

	const int pausebuttonW = 40;
	const int pausebuttonH = 40;

	SDL_Rect pauseuiRect = { windowW / 2 - pauseuiW / 2, windowH / 2 - pauseuiH / 2, pauseuiW , pauseuiH };
	SDL_Rect resumeRect = { pauseuiRect.x + pauseuiW / 2 + 60 - resumeW / 2 ,
	pauseuiRect.y + pauseuiH / 2 + 30 - resumeH / 2, resumeW , resumeH };
	SDL_Rect leaveRect = { pauseuiRect.x + pauseuiW / 2 - 60 - leaveW / 2 ,
		pauseuiRect.y + pauseuiH / 2 + 30 - leaveH / 2, leaveW , leaveH };

	SDL_Rect scorelabelRect = { windowW - scorelabelW - 140, 10, scorelabelW , scorelabelH };
	SDL_Rect lifelabelRect = { windowW - lifelabelW - 10, 10, lifelabelW , lifelabelH };
	SDL_Rect pausebuttonRect = { 10, 10, pausebuttonW , pausebuttonH };
	char str[5];
	SDL_itoa(playerlife, str, 10);
	SDL_Texture* lifelabelTexture = CreateTexturewithBlitedTextFromSurface(str, 80, globalresources, FONT_PATH, 255, 255, 255, 255,
		globalresources->lifelabelSurface, 60, 0, ALIGN_CENTER, ALIGN_CENTER);

	char score_str[10];
	SDL_itoa(score, score_str, 10);
	SDL_Texture* scorelabelTexture = CreateTexturewithBlitedTextFromSurface(score_str, 80, globalresources, FONT_PATH, 255, 255, 255, 255,
		globalresources->scorelabelSurface, 60, 0, ALIGN_CENTER, ALIGN_CENTER);

	SDL_Texture* resumeTexture = CreateTexturewithBlitedText("Resume", 80, globalresources, FONT_PATH, 255, 255, 255, 255,
		"pictures/button1.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
	SDL_Texture* leaveTexture = CreateTexturewithBlitedText("Leave", 80, globalresources, FONT_PATH, 255, 255, 255, 255,
		"pictures/button1.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);

	SDL_Texture* pauseuiTexture = CreateTexturewithBlitedText("Game paused.", 60, globalresources, FONT_PATH, 0, 0, 0, 255, "pictures/pauseui.png", 0, -60, ALIGN_CENTER, ALIGN_CENTER);

	int mouseX = 0;
	int mouseY = 0;

	int clickX = 0;
	int clickY = 0;

	int pauseuiXwhileclicked = 0;
	int pauseuiYwhileclicked = 0;

	SDL_bool mousepressed = SDL_FALSE;

	SDL_bool pause = SDL_TRUE;
	SDL_Event event;
	while (quit == SDL_FALSE && pause == SDL_TRUE && game == SDL_TRUE)
	{
		uint32_t begin = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			SDL_Rect resumeRect = { pauseuiRect.x + pauseuiW / 2 + 60 - resumeW / 2 ,
				pauseuiRect.y + pauseuiH / 2 + 30 - resumeH / 2, resumeW , resumeH };
			SDL_Rect leaveRect = { pauseuiRect.x + pauseuiW / 2 - 60 - leaveW / 2 ,
				pauseuiRect.y + pauseuiH / 2 + 30 - leaveH / 2, leaveW , leaveH };

			if (event.type == SDL_QUIT)
			{
				quit = SDL_TRUE;
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_BACKQUOTE)
				{
						pause = SDL_FALSE;
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.x >= pauseuiRect.x && event.button.x <= pauseuiRect.x + pauseuiRect.w
					&& event.button.y >= pauseuiRect.y && event.button.y <= pauseuiRect.y + pauseuiRect.h
					&&
					!((event.button.x >= resumeRect.x && event.button.x <= resumeRect.x + resumeRect.w
						&& event.button.y >= resumeRect.y && event.button.y <= resumeRect.y + resumeRect.h)
						||
						(event.button.x >= leaveRect.x && event.button.x <= leaveRect.x + leaveRect.w
							&& event.button.y >= leaveRect.y && event.button.y <= leaveRect.y + leaveRect.h)))
				{
					if (mousepressed == SDL_FALSE)
					{
						clickX = event.button.x;
						clickY = event.button.y;

						pauseuiXwhileclicked = pauseuiRect.x;
						pauseuiYwhileclicked = pauseuiRect.y;
					}
					mousepressed = SDL_TRUE;
				}
				if (event.button.x >= resumeRect.x && event.button.x <= resumeRect.x + resumeRect.w
					&& event.button.y >= resumeRect.y && event.button.y <= resumeRect.y + resumeRect.h)
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);

					SDL_DestroyTexture(resumeTexture);
					resumeTexture = CreateTexturewithBlitedText("Resume", 80, globalresources, FONT_PATH, 255, 255, 255, 255,
						"pictures/button2.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
				}
				if (event.button.x >= leaveRect.x && event.button.x <= leaveRect.x + leaveRect.w
					&& event.button.y >= leaveRect.y && event.button.y <= leaveRect.y + leaveRect.h)
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);

					SDL_DestroyTexture(leaveTexture);
					leaveTexture = CreateTexturewithBlitedText("Leave", 80, globalresources, FONT_PATH, 255, 255, 255, 255,
						"pictures/button2.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
				}
				if ((event.button.x >= pausebuttonRect.x && event.button.x <= pausebuttonRect.x + pausebuttonRect.w
					&& event.button.y >= pausebuttonRect.y && event.button.y <= pausebuttonRect.y + pausebuttonRect.h)
					&& (event.button.y <= pauseuiRect.y || event.button.x <= pauseuiRect.x))
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				SDL_DestroyTexture(resumeTexture);
				SDL_DestroyTexture(leaveTexture);
				resumeTexture = CreateTexturewithBlitedText("Resume", 80, globalresources, FONT_PATH, 255, 255, 255, 255,
					"pictures/button1.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
				leaveTexture = CreateTexturewithBlitedText("Leave", 80, globalresources, FONT_PATH, 255, 255, 255, 255,
					"pictures/button1.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
				mousepressed = SDL_FALSE;
				if ((event.button.x >= resumeRect.x && event.button.x <= resumeRect.x + resumeRect.w
					&& event.button.y >= resumeRect.y && event.button.y <= resumeRect.y + resumeRect.h)
					||
					((event.button.x >= pausebuttonRect.x && event.button.x <= pausebuttonRect.x + pausebuttonRect.w
						&& event.button.y >= pausebuttonRect.y && event.button.y <= pausebuttonRect.y + pausebuttonRect.h)
						&& (event.button.y <= pauseuiRect.y || event.button.x <= pauseuiRect.x)))
				{
					pause = SDL_FALSE;

				}
				else if (event.button.x >= leaveRect.x && event.button.x <= leaveRect.x + leaveRect.w
					&& event.button.y >= leaveRect.y && event.button.y <= leaveRect.y + leaveRect.h)
				{
					game = SDL_FALSE;
				}
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				mouseX = event.motion.x;
				mouseY = event.motion.y;

				if (mousepressed == SDL_TRUE)
				{
					pauseuiRect.x = pauseuiXwhileclicked + mouseX - clickX;
					pauseuiRect.y = pauseuiYwhileclicked + mouseY - clickY;

					if (pauseuiRect.x >= windowW - pauseuiW)
					{
						pauseuiRect.x = windowW - pauseuiW - 1;
					}
					if (pauseuiRect.y >= windowH - pauseuiH)
					{
						pauseuiRect.y = windowH - pauseuiH - 1;
					}
					if (pauseuiRect.x < 0)
					{
						pauseuiRect.x = 0;
					}
					if (pauseuiRect.y < 0)
					{
						pauseuiRect.y = 0;
					}
				}
			}

		}


		SDL_Rect resumeRect = { pauseuiRect.x + pauseuiW / 2 + 60 - resumeW / 2 ,
			pauseuiRect.y + pauseuiH / 2 + 30 - resumeH / 2, resumeW , resumeH };
		SDL_Rect leaveRect = { pauseuiRect.x + pauseuiW / 2 - 60 - leaveW / 2 ,
			pauseuiRect.y + pauseuiH / 2 + 30 - leaveH / 2, leaveW , leaveH };


		if ((mouseX >= resumeRect.x && mouseX <= resumeRect.x + resumeRect.w
			&& mouseY >= resumeRect.y && mouseY <= resumeRect.y + resumeRect.h)
			||
			(((mouseX >= pausebuttonRect.x && mouseX <= pausebuttonRect.x + pausebuttonRect.w
				&& mouseY >= pausebuttonRect.y && mouseY <= pausebuttonRect.x + pausebuttonRect.w)
				&& (mouseX <= pauseuiRect.x || mouseY <= pauseuiRect.y)))
			||
			(mouseX >= leaveRect.x && mouseX <= leaveRect.x + leaveRect.w
				&& mouseY >= leaveRect.y && mouseY <= leaveRect.y + leaveRect.h))
		{
			SDL_SetCursor(globalresources->cursor_hand);
		}
		else
		{
			SDL_SetCursor(globalresources->cursor_arrow);
		}


		SDL_RenderCopy(globalresources->rend, globalresources->backgroundTexture, NULL, NULL);
		DrawBullet(globalresources->rend, globalresources->bulletTexture, objectsparameters);
		DrawEnemy(globalresources);
		ShowShortAttack(globalresources);
		DrawEnemyBullets(objectsparameters, globalresources);

		ShowBoss(globalresources);
		DrawBossBullets(globalresources);

		DrawBomb(globalresources);

		ShowAllBlitedTexture(globalresources);

		ShowBossLife(globalresources);

		SDL_RenderCopy(globalresources->rend, globalresources->playerTexture, NULL, globalresources->box_player);

		SDL_RenderCopy(globalresources->rend, lifelabelTexture, NULL, &lifelabelRect);
		SDL_RenderCopy(globalresources->rend, scorelabelTexture, NULL, &scorelabelRect);
		SDL_RenderCopy(globalresources->rend, globalresources->pausebuttonTexture, NULL, &pausebuttonRect);

		ShowRageBarWhilePause(globalresources);
		ShowSkillIcons(globalresources);

		if (globalresources->show_wavenum == SDL_TRUE)
			SDL_RenderCopy(globalresources->rend, globalresources->wavenumberTexture, NULL, globalresources->wavenumberRect);

		SDL_RenderCopy(globalresources->rend, pauseuiTexture, NULL, &pauseuiRect);
		SDL_RenderCopy(globalresources->rend, resumeTexture, NULL, &resumeRect);
		SDL_RenderCopy(globalresources->rend, leaveTexture, NULL, &leaveRect);

		SDL_RenderPresent(globalresources->rend);

		Delay(begin);
		SDL_RenderClear(globalresources->rend);
	}

	SDL_DestroyTexture(lifelabelTexture);
	SDL_DestroyTexture(scorelabelTexture);
	SDL_DestroyTexture(resumeTexture);
	SDL_DestroyTexture(leaveTexture);
	SDL_DestroyTexture(pauseuiTexture);

	SDL_SetCursor(globalresources->cursor_arrow);
}

void OverEventLoop(GlobalResources* globalresources)
{
	ObjectsParameters objectsparameters = { 10, 20, 10, 20 };

	const int overuiW = 300;
	const int overuiH = 120;

	const int leaveW = 90;
	const int leaveH = 30;

	const int lifelabelW = 90;
	const int lifelabelH = 37;

	const int scorelabelW = 120;
	const int scorelabelH = 37;

	SDL_Rect overuiRect = { windowW / 2 - overuiW / 2, windowH / 2 - overuiH / 2, overuiW , overuiH };
	SDL_Rect leaveRect = { overuiRect.x + overuiW / 2 - 60 - leaveW / 2 ,
		overuiRect.y + overuiH / 2 + 30 - leaveH / 2, leaveW , leaveH };

	SDL_Rect scorelabelRect = { windowW - scorelabelW - 140, 10, scorelabelW , scorelabelH };
	SDL_Rect lifelabelRect = { windowW - lifelabelW - 10, 10, lifelabelW , lifelabelH };
	char str[5];
	SDL_itoa(playerlife, str, 10);
	SDL_Texture* lifelabelTexture = CreateTexturewithBlitedTextFromSurface(str, 80, globalresources, FONT_PATH, 255, 255, 255, 255,
		globalresources->lifelabelSurface, 60, 0, ALIGN_CENTER, ALIGN_CENTER);
	SDL_Texture* leaveTexture = CreateTexturewithBlitedText("Leave", 80, globalresources, FONT_PATH, 255, 255, 255, 255,
		"pictures/button1.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);

	char score_str[10];
	SDL_itoa(score, score_str, 10);
	SDL_Texture* scorelabelTexture = CreateTexturewithBlitedTextFromSurface(score_str, 80, globalresources, FONT_PATH, 255, 255, 255, 255,
		globalresources->scorelabelSurface, 60, 0, ALIGN_CENTER, ALIGN_CENTER);


	int overuiX = windowW / 2 - overuiW / 2;
	int overuiY = windowH / 2 - overuiH / 2;

	SDL_Texture* overuiTexture = CreateTexturewithBlitedText("Game over!", 60, globalresources, FONT_PATH, 0, 0, 0, 255, "pictures/pauseui.png", 0, -60, ALIGN_CENTER, ALIGN_CENTER);

	int mouseX = 0;
	int mouseY = 0;

	int clickX = 0;
	int clickY = 0;

	int overuiXwhileclicked = 0;
	int overuiYwhileclicked = 0;

	SDL_bool mousepressed = SDL_FALSE;

	SDL_Event event;
	while (quit == SDL_FALSE && game == SDL_TRUE)
	{
		uint32_t begin = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{

			SDL_Rect leaveRect = { overuiRect.x + overuiW / 2 - leaveW / 2 ,
				overuiRect.y + overuiH / 2 + 30 - leaveH / 2, leaveW , leaveH };

			if (event.type == SDL_QUIT)
			{
				quit = SDL_TRUE;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.x >= overuiRect.x && event.button.x <= overuiRect.x + overuiRect.w
					&& event.button.y >= overuiRect.y && event.button.y <= overuiRect.y + overuiRect.h
					&&
					!(event.button.x >= leaveRect.x && event.button.x <= leaveRect.x + leaveRect.w
						&& event.button.y >= leaveRect.y && event.button.y <= leaveRect.y + leaveRect.h))
				{
					if (mousepressed == SDL_FALSE)
					{
						clickX = event.button.x;
						clickY = event.button.y;

						overuiXwhileclicked = overuiRect.x;
						overuiYwhileclicked = overuiRect.y;
					}
					mousepressed = SDL_TRUE;
				}
				if (event.button.x >= leaveRect.x && event.button.x <= leaveRect.x + leaveRect.w
					&& event.button.y >= leaveRect.y && event.button.y <= leaveRect.y + leaveRect.h)
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);

					SDL_DestroyTexture(leaveTexture);
					leaveTexture = CreateTexturewithBlitedText("Leave", 80, globalresources, FONT_PATH, 255, 255, 255, 255,
						"pictures/button2.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				SDL_DestroyTexture(leaveTexture);
				leaveTexture = CreateTexturewithBlitedText("Leave", 80, globalresources, FONT_PATH, 255, 255, 255, 255,
					"pictures/button1.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
				mousepressed = SDL_FALSE;
				if (event.button.x >= leaveRect.x && event.button.x <= leaveRect.x + leaveRect.w
					&& event.button.y >= leaveRect.y && event.button.y <= leaveRect.y + leaveRect.h)
				{
					game = SDL_FALSE;
				}
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				mouseX = event.motion.x;
				mouseY = event.motion.y;

				if (mousepressed == SDL_TRUE)
				{
					overuiRect.x = overuiXwhileclicked + mouseX - clickX;
					overuiRect.y = overuiYwhileclicked + mouseY - clickY;

					if (overuiRect.x >= windowW - overuiW)
					{
						overuiRect.x = windowW - overuiW - 1;
					}
					if (overuiRect.y >= windowH - overuiH)
					{
						overuiRect.y = windowH - overuiH - 1;
					}
					if (overuiRect.x < 0)
					{
						overuiRect.x = 0;
					}
					if (overuiRect.y < 0)
					{
						overuiRect.y = 0;
					}
				}
			}

		}

		SDL_Rect leaveRect = { overuiRect.x + overuiW / 2 - leaveW / 2 ,
			overuiRect.y + overuiH / 2 + 30 - leaveH / 2, leaveW , leaveH };


		if (mouseX >= leaveRect.x && mouseX <= leaveRect.x + leaveRect.w
			&& mouseY >= leaveRect.y && mouseY <= leaveRect.y + leaveRect.h)
		{
			SDL_SetCursor(globalresources->cursor_hand);
		}
		else
		{
			SDL_SetCursor(globalresources->cursor_arrow);
		}


		SDL_RenderCopy(globalresources->rend, globalresources->backgroundTexture, NULL, NULL);
		DrawBullet(globalresources->rend, globalresources->bulletTexture, objectsparameters);
		DrawEnemy(globalresources);
		ShowShortAttack(globalresources);
		DrawEnemyBullets(objectsparameters, globalresources);

		ShowAllBlitedTexture(globalresources);

		ShowBoss(globalresources);
		DrawBossBullets(globalresources);

		DrawBomb(globalresources);

		ShowBossLife(globalresources);

		SDL_RenderCopy(globalresources->rend, globalresources->playerTexture, NULL, globalresources->box_player);

		SDL_RenderCopy(globalresources->rend, lifelabelTexture, NULL, &lifelabelRect);
		SDL_RenderCopy(globalresources->rend, scorelabelTexture, NULL, &scorelabelRect);

		ShowRageBarWhilePause(globalresources);
		ShowSkillIcons(globalresources);

		if (globalresources->show_wavenum == SDL_TRUE)
			SDL_RenderCopy(globalresources->rend, globalresources->wavenumberTexture, NULL, globalresources->wavenumberRect);

		SDL_RenderCopy(globalresources->rend, overuiTexture, NULL, &overuiRect);
		SDL_RenderCopy(globalresources->rend, leaveTexture, NULL, &leaveRect);

		SDL_RenderPresent(globalresources->rend);

		Delay(begin);
		SDL_RenderClear(globalresources->rend);
	}

	SDL_DestroyTexture(lifelabelTexture);
	SDL_DestroyTexture(scorelabelTexture);
	SDL_DestroyTexture(leaveTexture);
	SDL_DestroyTexture(overuiTexture);

}

void GameEventLoop(GlobalResources* globalresources)
{
	musicchannel = Mix_PlayChannel(-1, globalresources->gameMusic, -1);

	ObjectsParameters objectsparameters = { 10, 20, 10, 20 };
	SkillParameters skillparameters = { 50, 50 };
    GlobalParameters globalparameters = { 84, 68 };
	GameParameters gameparameters = { windowW / 2 - globalparameters.playerW / 2 , windowH - globalparameters.playerH };

	InitBlitedTexture(globalresources);

	trackmode = SDL_FALSE;

	playerlife = 5;
	score = 0;
	rage = 0;

	SDL_bool pause = SDL_FALSE;
	globalresources->show_wavenum = SDL_FALSE;

	int wavenumber = 1;


	globalresources->box_player->x = windowW / 2 - globalparameters.playerW / 2;
	globalresources->box_player->y = windowH - globalparameters.playerH;
	globalresources->box_player->w = globalparameters.playerW;
	globalresources->box_player->h = globalparameters.playerH;


	SDL_Keycode key = 0;

	SDL_bool pressedUp = SDL_FALSE;
	SDL_bool pressedDown = SDL_FALSE;
	SDL_bool pressedLeft = SDL_FALSE;
	SDL_bool pressedRight = SDL_FALSE;
	SDL_bool pressedSpace = SDL_FALSE;
	SDL_bool pressed0 = SDL_FALSE;

	const int lifelabelW = 90;
	const int lifelabelH = 37;

	const int scorelabelW = 120;
	const int scorelabelH = 37;

	const int pausebuttonW = 40;
	const int pausebuttonH = 40;

	const int wavenumberW = 400;
	const int wavenumberH = 100;

	int mouseX = 300;
	int mouseY = 300;

	uint32_t pauseT = 0;
	uint32_t resumeT = 0;
	uint32_t t0 = SDL_GetTicks();
	uint32_t cutT = 0;

	uint32_t gameT = 0;
	uint32_t gameTformer = 0;
	uint32_t gameTformer2 = 0;

	uint32_t bulletT = 0;
	uint32_t shortattackT = 0;

	uint32_t mistT = 0;
	uint32_t bombmistT = 0;
	uint32_t trackT = 0;

	uint32_t ragestartT = 0;
	SDL_bool is_ragejustfull = SDL_FALSE;

	int enemy_count = 0;

	Wave wave = DecideWave(wavenumber);;
	InitEnemyMode(wave.enemy_type);

	SDL_Texture* lifelabelTexture;
	SDL_Texture* scorelabelTexture;

	SDL_Event event;
	while (quit == SDL_FALSE && game == SDL_TRUE)
	{
		uint32_t begin = SDL_GetTicks();

		IfLose(globalresources);

		resumeT = 0;
		pauseT = 0;

		SDL_Rect scorelabelRect = { windowW - scorelabelW - 140, 10, scorelabelW , scorelabelH };
		SDL_Rect lifelabelRect = { windowW - lifelabelW - 10, 10, lifelabelW , lifelabelH };
		SDL_Rect pausebuttonRect = { 10, 10, pausebuttonW , pausebuttonH };
		char str_life[5];
		SDL_itoa(playerlife, str_life, 10);

		lifelabelTexture = CreateTexturewithBlitedTextFromSurface(str_life, 80, globalresources, FONT_PATH, 255, 255, 255, 255,
			globalresources->lifelabelSurface, 60, 0, ALIGN_CENTER, ALIGN_CENTER);

		char score_str[10];
		SDL_itoa(score, score_str, 10);
		scorelabelTexture = CreateTexturewithBlitedTextFromSurface(score_str, 80, globalresources, FONT_PATH, 255, 255, 255, 255,
			globalresources->scorelabelSurface, 60, 0, ALIGN_CENTER, ALIGN_CENTER);

		while (SDL_PollEvent(&event))
		{

			if (event.type == SDL_QUIT)
			{
				quit = SDL_TRUE;
			}
			if (event.type == SDL_KEYUP)
			{
				key = 0;
				if (event.key.keysym.sym == SDLK_UP)
				{
					pressedUp = SDL_FALSE;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					pressedDown = SDL_FALSE;
				}
				else if (event.key.keysym.sym == SDLK_LEFT)
				{
					pressedLeft = SDL_FALSE;
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					pressedRight = SDL_FALSE;
				}
				else if (event.key.keysym.sym == SDLK_SPACE)
				{
					pressedSpace = SDL_FALSE;
				}
				else if (event.key.keysym.sym == SDLK_KP_0)
				{
					pressed0 = SDL_FALSE;
				}

				else if (event.key.keysym.sym == SDLK_1)
				{
					RespondSkills(1, gameT, skillparameters, globalresources);
				}
				else if (event.key.keysym.sym == SDLK_2)
				{
					RespondSkills(2, gameT, skillparameters, globalresources);
				}
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_UP)
				{
					key = SDLK_UP;
					pressedUp = SDL_TRUE;
				}
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					key = SDLK_DOWN;
					pressedDown = SDL_TRUE;
				}
				if (event.key.keysym.sym == SDLK_LEFT)
				{
					key = SDLK_LEFT;
					pressedLeft = SDL_TRUE;
				}
				if (event.key.keysym.sym == SDLK_RIGHT)
				{
					key = SDLK_RIGHT;
					pressedRight = SDL_TRUE;
				}
				if (event.key.keysym.sym == SDLK_SPACE)
				{
					key = SDLK_SPACE;
					pressedSpace = SDL_TRUE;
				}
				if (event.key.keysym.sym == SDLK_KP_0)
				{
					key = SDLK_KP_0;
					pressed0 = SDL_TRUE;
				}

				if (event.key.keysym.sym == SDLK_BACKQUOTE)
				{
					pause = SDL_TRUE;
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.x >= pausebuttonRect.x && event.button.x <= pausebuttonRect.x + pausebuttonRect.w
					&& event.button.y >= pausebuttonRect.y && event.button.y <= pausebuttonRect.y + pausebuttonRect.h)
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{

				if (event.button.x >= pausebuttonRect.x && event.button.x <= pausebuttonRect.x + pausebuttonRect.w
					&& event.button.y >= pausebuttonRect.y && event.button.y <= pausebuttonRect.y + pausebuttonRect.h)
				{
					pause = SDL_TRUE;
				}
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				mouseX = event.motion.x;
				mouseY = event.motion.y;
			}
		}
		if (pressedUp == SDL_TRUE)
		{
			gameparameters.playerlY -= sensitivity;
		}
		if (pressedDown == SDL_TRUE)
		{
			gameparameters.playerlY += sensitivity;
		}
		if (pressedLeft == SDL_TRUE)
		{
			gameparameters.playerlX -= sensitivity;
		}
		if (pressedRight == SDL_TRUE)
		{
			gameparameters.playerlX += sensitivity;
		}
		if (pressedSpace == SDL_TRUE && pressed0 == SDL_FALSE && (gameT - bulletT >= 350 || bulletT == 0))
		{
			Mix_PlayChannel(-1, globalresources->shootSound, 0);
			ShootBullet(globalresources->box_player->x, globalresources->box_player->y, objectsparameters, globalparameters);
			bulletT = gameT;
		}
		if (pressed0 == SDL_TRUE && (gameT - shortattackT >= 1250 || shortattackT == 0))
		{
			Mix_PlayChannel(-1, globalresources->shootSound, 0);
			AddShortAttack(globalresources->box_player->x, globalresources->box_player->y, globalresources, gameT);
			shortattackT = gameT;
			bulletT = gameT;
		}
		if (mouseX >= pausebuttonRect.x && mouseX <= pausebuttonRect.x + pausebuttonRect.w
			&& mouseY >= pausebuttonRect.y && mouseY <= pausebuttonRect.y + pausebuttonRect.h)
		{
			SDL_SetCursor(globalresources->cursor_hand);
		}
		else
		{
			SDL_SetCursor(globalresources->cursor_arrow);
		}

		globalresources->box_player->x = (int)gameparameters.playerlX;
		globalresources->box_player->y = (int)gameparameters.playerlY;

		if (globalresources->box_player->x >= windowW - globalparameters.playerW)
		{
			globalresources->box_player->x = windowW - globalparameters.playerW - 1;
		}
		if (globalresources->box_player->y >= windowH - globalparameters.playerH)
		{
			globalresources->box_player->y = windowH - globalparameters.playerH - 1;
		}
		if (globalresources->box_player->x < 0)
		{
			globalresources->box_player->x = 0;
		}
		if (globalresources->box_player->y < 0)
		{
			globalresources->box_player->y = 0;
		}


		if (pause == SDL_TRUE)
		{
			pauseT = SDL_GetTicks();
			PauseEventLoop(globalresources);
			resumeT = SDL_GetTicks();
			cutT += resumeT - pauseT;
			begin += resumeT - pauseT;
			resumeT = 0;
			pauseT = 0;
			pause = SDL_FALSE;

			mouseX = 0;
			mouseY = 0;
		}

		gameT = SDL_GetTicks() - t0 - cutT;
		if (enemy_count == 0 && gameT - gameTformer >= wave.fore_interval
			||
			enemy_count != 0 && gameT - gameTformer >= wave.enemy_interval)
		{
			if (wave.is_boss_wave == SDL_FALSE)
			{
				AddEnemy(EnemyList, wave.num - enemy_count, gameT, globalresources);
				enemy_count++;
			}
			else if (wave.is_boss_wave == SDL_TRUE && enemy_count == 0)
			{
				AddBoss();
				enemy_count = wave.num;
			}

			gameTformer = gameT;
		}
		

		if (enemy_count == wave.num && wave.is_boss_wave == SDL_FALSE)
		{

			wavenumber++;
			wave = DecideWave(wavenumber);
			InitEnemyMode(wave.enemy_type);
			enemy_count = 0;
			gameTformer2 = gameT + cutT;
		}
		if (enemy_count == wave.num && wave.is_boss_wave == SDL_TRUE && boss == NULL)
		{
			wavenumber++;
			wave = DecideWave(wavenumber);
			InitEnemyMode(wave.enemy_type);
			enemy_count = 0;
			gameTformer2 = gameT + cutT;
		}

		char str_wave[5];
		SDL_itoa(wavenumber, str_wave, 10);
		char str_tmp[20] = "Wave - ";
		SDL_strlcat(str_tmp, str_wave, 20);

		SDL_DestroyTexture(globalresources->wavenumberTexture);
		globalresources->wavenumberTexture = CreateTexturewithBlitedTextFromSurface(str_tmp, 40, globalresources, FONT_PATH, 0, 0, 0, 255,
			globalresources->noneSurface, 0, 0, ALIGN_CENTER, ALIGN_CENTER);

		SDL_SetTextureBlendMode(globalresources->logoTexture, SDL_BLENDMODE_BLEND);

		globalresources->wavenumberRect->x = windowW / 2 - wavenumberW / 2;
		globalresources->wavenumberRect->y = (int)(windowH * 0.1);
		globalresources->wavenumberRect->w = wavenumberW;
		globalresources->wavenumberRect->h = wavenumberH;

		if (gameT - gameTformer2 <= 500)
		{
			int time = gameT - gameTformer2;
			SDL_SetTextureAlphaMod(globalresources->wavenumberTexture, LinearVar(time, 0, 255, 1, 500));
		}
		if (gameT - gameTformer2 > 3000 && gameT - gameTformer2 <= 3000 + 500)
		{
			int time = gameT - gameTformer2;
			SDL_SetTextureAlphaMod(globalresources->wavenumberTexture, LinearVar(time, 255, 0, 3001, 3500));
		}

		VanishShortAttack(gameT);
		ShortAttackTakeEffect(objectsparameters);
		CalcSkillTimeLeft(gameT);
		UseSkills(gameT);

		NewBossAttackMode(gameT);
		BossAttack(gameT, globalresources);

		BulletTracking(20, gameT, &trackT);

		IfShot(&score, &rage, objectsparameters, globalresources);
		IfBossShot(&score, &rage, objectsparameters, globalresources);
		IfPlayerShot(objectsparameters, globalresources);
		IfPlayerShotByBoss(globalresources);

		MoveBossBullet(gameT, globalresources);
		MoveBullet(objectsparameters);
		MoveEnemy(globalresources);
		ChangeEnemyRout(gameT);
		MoveBomb();
		BombExplode();

		MoveEnemyBullet();

		MoveBoss(gameT);

		AggresiveEnemyAttack(gameT, 1800, objectsparameters, globalresources);

		AddEnemyMist(12, gameT, &mistT);
		AddBombMist(60, gameT, &bombmistT);
		PlayAllBlitedTexture(gameT);

		SDL_RenderCopy(globalresources->rend, globalresources->backgroundTexture, NULL, NULL);

		ShowShortAttack(globalresources);
		DrawBullet(globalresources->rend, globalresources->bulletTexture, objectsparameters);
		DrawEnemy(globalresources);

		DrawEnemyBullets(objectsparameters, globalresources);


		ShowBoss(globalresources);
		DrawBossBullets(globalresources);
		DrawBomb(globalresources);

		ShowAllBlitedTexture(globalresources);

		SDL_RenderCopy(globalresources->rend, globalresources->playerTexture, NULL, globalresources->box_player);
		SDL_RenderCopy(globalresources->rend, lifelabelTexture, NULL, &lifelabelRect);
		SDL_RenderCopy(globalresources->rend, scorelabelTexture, NULL, &scorelabelRect);
		SDL_RenderCopy(globalresources->rend, globalresources->pausebuttonTexture, NULL, &pausebuttonRect);
		ShowRageBar(gameT, &ragestartT, &is_ragejustfull, globalresources);
		ShowSkillIcons(globalresources);

		ShowBossLife(globalresources);

		if (gameT - gameTformer2 <= 3500)
		{
			globalresources->show_wavenum = SDL_TRUE;
			SDL_RenderCopy(globalresources->rend, globalresources->wavenumberTexture, NULL, globalresources->wavenumberRect);
		}
		else
		{
			globalresources->show_wavenum = SDL_FALSE;
		}

		SDL_RenderPresent(globalresources->rend);
		
		Delay(begin);

		SDL_RenderClear(globalresources->rend);
		SDL_DestroyTexture(lifelabelTexture);
		SDL_DestroyTexture(scorelabelTexture);
		
	}
	SDL_DestroyTexture(globalresources->wavenumberTexture);

	DestroyAllEnemies(enemies, ENEMY_AMOUNT_MAX);
	DestroyAllBullets(bullets, BULLET_AMOUNT_MAX);
	DestroyAllBlitedTexture(blitedtextures, BLITEDTEXTURE_AMOUNT_MAX);
	DestroyAllEnemyBullets(enemybullets, ENEMYBULLET_AMOUNT_MAX);
	DestroyAllBombs(bombs, BOMB_AMOUNT_MAX);
	DestroyBoss(&boss);
	DestroyAllBossBullets();

	Mix_HaltChannel(musicchannel);
}

void IfLose(GlobalResources* globalresources)
{
	if (playerlife <= 0)
	{
		playerlife = 0;
		OverEventLoop(globalresources);
	}
}


