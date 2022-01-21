#include "start.h"
#include "globals.h"
#include "resource.h"
#include "almanac.h"

static void SettingsEvents(SDL_Event event, DragRecord* dragrecord, StartParameters* parameters, SDL_bool* setting, SDL_bool* quitsetting, GlobalResources* globalresources);

static void GearSensEvents(SDL_Event event, DragRecord* dragrecord, StartParameters* parameters);

static void ShowSensitivityGear(int w, int h, StartParameters* parameters, GlobalResources* globalresources);

static void GearMusEvents(SDL_Event event, DragRecord* dragrecord, StartParameters* parameters);

static void ShowMusicGear(int w, int h, StartParameters* parameters, GlobalResources* globalresources);

static void GearSndEvents(SDL_Event event, DragRecord* dragrecord, StartParameters* parameters);

static void ShowMusicGear(int w, int h, StartParameters* parameters, GlobalResources* globalresources);

static void Settings(StartResources* resource, StartParameters* parameters, GlobalResources* globalresources);

static void HelpEvents(SDL_Event event, DragRecord* dragrecord, StartParameters* parameters, SDL_bool* help, SDL_bool* quithelp, GlobalResources* globalresources);

static void Help(StartResources* resource, StartParameters* parameters, GlobalResources* globalresources);

void StartEventLoop(GlobalResources* globalresources)
{
	musicchannel = Mix_PlayChannel(-1, globalresources->startMusic, -1);

	SDL_bool setting = SDL_FALSE;
	SDL_bool quitsetting = SDL_FALSE;
	SDL_bool entersetting = SDL_FALSE;
	SDL_bool help = SDL_FALSE;
	SDL_bool enterhelp = SDL_FALSE;
	SDL_bool quithelp = SDL_FALSE;
	SDL_bool almanac = SDL_FALSE;

	const int logoW = 124 * 3;
	const int logoH = 78 * 3;

	const int logo1W = 23 * 3;
	const int logo1H = 18 * 3;

	const int ButtonStartW = 58 * 3;
	const int ButtonStartH = 20 * 3;

	const int ButtonQuitW = 90;
	const int ButtonQuitH = 32;

	const int ButtonSettingsW = 58 * 3;
	const int ButtonSettingsH = 20 * 3;

	const int settingsuiW = 400;
	const int settingsuiH = 300;

	const int helpuiW = 500;
	const int helpuiH = 500;

	int frame = 0;

	DragRecord dragrecordsetting =
	{
		0, 0, 0, 0, 0, 0, SDL_FALSE
	};
	DragRecord dragrecordsensgear =
	{
		0, 0, 0, 0, 0, 0, SDL_FALSE
	};
	DragRecord dragrecordhelp =
	{
		0, 0, 0, 0, 0, 0, SDL_FALSE
	};
	DragRecord dragrecordmusgear =
	{
		0, 0, 0, 0, 0, 0, SDL_FALSE
	};
	DragRecord dragrecordsndgear =
	{
		0, 0, 0, 0, 0, 0, SDL_FALSE
	};

	SDL_Rect rectLogo;

	SDL_Texture* buttonstartTexture = CreateTexturewithBlitedText("Start", 80, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button1.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
	SDL_Texture* buttonquitTexture = CreateTexturewithBlitedText("Quit", 80, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button1.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
	SDL_Texture* buttonsettingsTexture = CreateTexturewithBlitedText("Settings", 60, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button1.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
	SDL_Texture* buttonhelpTexture = CreateTexturewithBlitedText("Help", 80, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button1.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
	SDL_Texture* buttonalmanacTexture = CreateTexturewithBlitedText("Almanac", 60, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button1.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);

	SDL_Texture* logo1Texture = GetPackedResource("pictures/logo1ani.png", UNPACK_TEXTURE, globalresources);

	StartResources resources;
	StartParameters parameters;

	SDL_Rect rectButtonStart = { windowW / 2 - ButtonStartW / 2, (int)(windowH * 0.6 - ButtonStartH / 2) ,
		ButtonStartW , ButtonStartH };
	SDL_Rect rectButtonQuit = { 20, windowH - 20 - ButtonQuitH , ButtonQuitW , ButtonQuitH };
	SDL_Rect rectButtonSettings = { windowW / 2 - ButtonSettingsW / 2, (int)(windowH * 0.7 - ButtonSettingsH / 2) ,
		ButtonSettingsW , ButtonSettingsH };
	SDL_Rect rectButtonHelp = { windowW / 2 - ButtonSettingsW / 2, (int)(windowH * 0.8 - ButtonSettingsH / 2) ,
		ButtonSettingsW , ButtonSettingsH };
	SDL_Rect rectButtonAlmanac = { windowW / 2 - ButtonSettingsW / 2, (int)(windowH * 0.9 - ButtonSettingsH / 2) ,
	ButtonSettingsW , ButtonSettingsH };

	parameters.settingsuiRect = SDL_malloc(sizeof(SDL_Rect));
	parameters.gearsensRect = SDL_malloc(sizeof(SDL_Rect));
	parameters.closeRect = SDL_malloc(sizeof(SDL_Rect));
	parameters.slidersensRect = SDL_malloc(sizeof(SDL_Rect));

	parameters.gearmusRect = SDL_malloc(sizeof(SDL_Rect));
	parameters.slidermusRect = SDL_malloc(sizeof(SDL_Rect));

	parameters.gearsndRect = SDL_malloc(sizeof(SDL_Rect));
	parameters.slidersndRect = SDL_malloc(sizeof(SDL_Rect));

	parameters.helpuiRect = SDL_malloc(sizeof(SDL_Rect));
	parameters.closehelpRect = SDL_malloc(sizeof(SDL_Rect));

	parameters.settingsuiRect->x = windowW / 2 - settingsuiW / 2;
	parameters.settingsuiRect->y = windowH / 2 - settingsuiH / 2;
	parameters.settingsuiRect->w = settingsuiW;
	parameters.settingsuiRect->h = settingsuiH;

	parameters.helpuiRect->x = windowW / 2 - helpuiW / 2;
	parameters.helpuiRect->y = windowH / 2 - helpuiH / 2;
	parameters.helpuiRect->w = helpuiW;
	parameters.helpuiRect->h = helpuiH;

	parameters.sensitivity_max = 30;
	parameters.sensitivity_min = 10;

	parameters.musicVol_max = 16;
	parameters.musicVol_min = 0;

	parameters.soundVol_max = 16;
	parameters.soundVol_min = 0;

	SDL_Rect gearsensRect = { 0, 0, 0, 0 };

	SDL_Rect sliderRectout = { 0, 0, 0, 0 };

	SDL_Rect closeRectout = { 0, 0, 0, 0 };

	SDL_Rect logo1Rect = { 100, 0, 0, 0 };
	SDL_Rect logo1Cut = { 0, 0, logo1W / 3,logo1H / 3 };

	int logo1midY = logo1Rect.y;
	const int logo1midY0 = logo1Rect.y;
	const int logo1X0 = logo1Rect.x;

	const int logo1dstX = (int)(windowW / 2 - logoW  * 0.13);
	const int logo1dstY = (int)(windowH * 0.36 - logoH * 0.6);

	double svar = 0;
	double A = 0;

	SDL_Event event;
	while (quit == SDL_FALSE && game == SDL_FALSE)
	{
		uint32_t begin = SDL_GetTicks();
		frame++;

		static int mouseX = 0;
		static int mouseY = 0;
		while (SDL_PollEvent(&event))
		{

			if (event.type == SDL_QUIT)
			{
				quit = SDL_TRUE;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && frame >= 150 && setting == SDL_FALSE && help == SDL_FALSE)
			{
				if (event.button.x >= rectButtonStart.x && event.button.x <= rectButtonStart.x + rectButtonStart.w
					&& event.button.y >= rectButtonStart.y && event.button.y <= rectButtonStart.y + rectButtonStart.h)
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
					SDL_DestroyTexture(buttonstartTexture);
					buttonstartTexture = CreateTexturewithBlitedText("Start", 80, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button2.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
				}
				if (event.button.x >= rectButtonQuit.x && event.button.x <= rectButtonQuit.x + rectButtonQuit.w
					&& event.button.y >= rectButtonQuit.y && event.button.y <= rectButtonQuit.y + rectButtonQuit.h)
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
					SDL_DestroyTexture(buttonquitTexture);
					buttonquitTexture = CreateTexturewithBlitedText("Quit", 80, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button2.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
				}
				if (event.button.x >= rectButtonSettings.x && event.button.x <= rectButtonSettings.x + rectButtonSettings.w
					&& event.button.y >= rectButtonSettings.y && event.button.y <= rectButtonSettings.y + rectButtonSettings.h)
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
					SDL_DestroyTexture(buttonsettingsTexture);
					buttonsettingsTexture = CreateTexturewithBlitedText("Settings", 60, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button2.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
				}
				if (event.button.x >= rectButtonHelp.x && event.button.x <= rectButtonHelp.x + rectButtonHelp.w
					&& event.button.y >= rectButtonHelp.y && event.button.y <= rectButtonHelp.y + rectButtonHelp.h)
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
					SDL_DestroyTexture(buttonhelpTexture);
					buttonhelpTexture = CreateTexturewithBlitedText("Help", 80, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button2.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
				}
				if (event.button.x >= rectButtonAlmanac.x && event.button.x <= rectButtonAlmanac.x + rectButtonAlmanac.w
					&& event.button.y >= rectButtonAlmanac.y && event.button.y <= rectButtonAlmanac.y + rectButtonAlmanac.h)
				{
					Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
					SDL_DestroyTexture(buttonalmanacTexture);
					buttonalmanacTexture = CreateTexturewithBlitedText("Almanac", 60, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button2.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
				}

			}
			if (event.type == SDL_MOUSEBUTTONUP && frame >= 150 && setting == SDL_FALSE && help == SDL_FALSE)
			{
				SDL_DestroyTexture(buttonstartTexture);
				buttonstartTexture = CreateTexturewithBlitedText("Start", 80, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button1.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
				SDL_DestroyTexture(buttonquitTexture);
				buttonquitTexture = CreateTexturewithBlitedText("Quit", 80, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button1.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
				SDL_DestroyTexture(buttonsettingsTexture);
				buttonsettingsTexture = CreateTexturewithBlitedText("Settings", 60, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button1.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
				SDL_DestroyTexture(buttonhelpTexture);
				buttonhelpTexture = CreateTexturewithBlitedText("Help", 80, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button1.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);
				SDL_DestroyTexture(buttonalmanacTexture);
				buttonalmanacTexture = CreateTexturewithBlitedText("Almanac", 60, globalresources, FONT_PATH, 78, 24, 18, 255, "pictures/button1.png", 0, 10, ALIGN_CENTER, ALIGN_CENTER);

				if (event.button.x >= rectButtonStart.x && event.button.x <= rectButtonStart.x + rectButtonStart.w
					&& event.button.y >= rectButtonStart.y && event.button.y <= rectButtonStart.y + rectButtonStart.h)
				{
					game = SDL_TRUE;
				}
				if (event.button.x >= rectButtonQuit.x && event.button.x <= rectButtonQuit.x + rectButtonQuit.w
					&& event.button.y >= rectButtonQuit.y && event.button.y <= rectButtonQuit.y + rectButtonQuit.h)
				{
					quit = SDL_TRUE;
				}
				if (event.button.x >= rectButtonSettings.x && event.button.x <= rectButtonSettings.x + rectButtonSettings.w
					&& event.button.y >= rectButtonSettings.y && event.button.y <= rectButtonSettings.y + rectButtonSettings.h)
				{
					setting = SDL_TRUE;
					entersetting = SDL_TRUE;
				}
				if (event.button.x >= rectButtonHelp.x && event.button.x <= rectButtonHelp.x + rectButtonHelp.w
					&& event.button.y >= rectButtonHelp.y && event.button.y <= rectButtonHelp.y + rectButtonHelp.h)
				{
					help = SDL_TRUE;
					enterhelp = SDL_TRUE;
				}
				if (event.button.x >= rectButtonAlmanac.x && event.button.x <= rectButtonAlmanac.x + rectButtonAlmanac.w
					&& event.button.y >= rectButtonAlmanac.y && event.button.y <= rectButtonAlmanac.y + rectButtonAlmanac.h)
				{
					almanac = SDL_TRUE;
				}
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				mouseX = event.motion.x;
				mouseY = event.motion.y;
			}
			if (setting == SDL_TRUE)
			{
				SettingsEvents(event, &dragrecordsetting, &parameters, &setting, &quitsetting, globalresources);
				GearSensEvents(event, &dragrecordsensgear, &parameters);
				GearMusEvents(event, &dragrecordmusgear, &parameters);
				GearSndEvents(event, &dragrecordsndgear, &parameters);
			}
			if (help == SDL_TRUE)
			{
				HelpEvents(event, &dragrecordhelp, &parameters, &help, &quithelp, globalresources);
			}
		}
		if (frame >= 150 && game == SDL_FALSE && setting == SDL_FALSE && help == SDL_FALSE)
		{
			if ((mouseX >= rectButtonStart.x && mouseX <= rectButtonStart.x + rectButtonStart.w
				&& mouseY >= rectButtonStart.y && mouseY <= rectButtonStart.y + rectButtonStart.h)
				||
				(mouseX >= rectButtonQuit.x && mouseX <= rectButtonQuit.x + rectButtonQuit.w
					&& mouseY >= rectButtonQuit.y && mouseY <= rectButtonQuit.y + rectButtonQuit.h)
				||
				(mouseX >= rectButtonSettings.x && mouseX <= rectButtonSettings.x + rectButtonSettings.w
					&& mouseY >= rectButtonSettings.y && mouseY <= rectButtonSettings.y + rectButtonSettings.h)
				||
				(mouseX >= rectButtonHelp.x && mouseX <= rectButtonHelp.x + rectButtonHelp.w
					&& mouseY >= rectButtonHelp.y && mouseY <= rectButtonHelp.y + rectButtonHelp.h)
				||
				(mouseX >= rectButtonAlmanac.x && mouseX <= rectButtonAlmanac.x + rectButtonAlmanac.w
					&& mouseY >= rectButtonAlmanac.y && mouseY <= rectButtonAlmanac.y + rectButtonAlmanac.h))
			{
				SDL_SetCursor(globalresources->cursor_hand);
			}
			else
			{
				SDL_SetCursor(globalresources->cursor_arrow);
			}
		}
		if (frame >= 150 && game == SDL_FALSE && setting == SDL_TRUE)
		{
			if ((mouseX >= parameters.closeRect->x && mouseX <= parameters.closeRect->x + parameters.closeRect->w
				&& mouseY >= parameters.closeRect->y && mouseY <= parameters.closeRect->y + parameters.closeRect->h)
				||
				(mouseX >= parameters.gearsensRect->x && mouseX <= parameters.gearsensRect->x + parameters.gearsensRect->w
					&& mouseY >= parameters.gearsensRect->y && mouseY <= parameters.gearsensRect->y + parameters.gearsensRect->h)
				||
				(mouseX >= parameters.gearmusRect->x && mouseX <= parameters.gearmusRect->x + parameters.gearmusRect->w
					&& mouseY >= parameters.gearmusRect->y && mouseY <= parameters.gearmusRect->y + parameters.gearmusRect->h)
				||
				(mouseX >= parameters.gearsndRect->x && mouseX <= parameters.gearsndRect->x + parameters.gearsndRect->w
					&& mouseY >= parameters.gearsndRect->y && mouseY <= parameters.gearsndRect->y + parameters.gearsndRect->h))
			{
				SDL_SetCursor(globalresources->cursor_hand);
			}
			else
			{
				SDL_SetCursor(globalresources->cursor_arrow);
			}
		}
		if (frame >= 150 && game == SDL_FALSE && help == SDL_TRUE)
		{
			if ((mouseX >= parameters.closehelpRect->x && mouseX <= parameters.closehelpRect->x + parameters.closehelpRect->w
				&& mouseY >= parameters.closehelpRect->y && mouseY <= parameters.closehelpRect->y + parameters.closehelpRect->h))
			{
				SDL_SetCursor(globalresources->cursor_hand);
			}
			else
			{
				SDL_SetCursor(globalresources->cursor_arrow);
			}
		}

		if (frame <= 40)
			SDL_SetRenderDrawColor(globalresources->rend,
				(int)LinearVar(frame, 1, 40, 1, 40), (int)LinearVar(frame, 1, 40, 1, 40), (int)LinearVar(frame, 1, 40, 1, 40),
				255);
		else
			SDL_SetRenderDrawColor(globalresources->rend, 40, 40, 40, 255);
		if (frame > 80 && frame <= 120)
			SDL_SetTextureAlphaMod(globalresources->logoTexture, (int)LinearVar(frame, 100, 255, 81, 120));
		else if (frame > 120)
			SDL_SetTextureAlphaMod(globalresources->logoTexture, 255);

		if (frame > 50 && frame <= 120)
		{
			SDL_SetTextureAlphaMod(globalresources->backgroundTexture, (int)LinearVar(frame, 0, 255, 51, 120));
			SDL_RenderCopy(globalresources->rend, globalresources->backgroundTexture, NULL, NULL);
		}

		if (frame > 50 && frame <= 120)
		{
			logo1Cut.x = (frame / 10) % 4 * (logo1W / 3);
			svar = SinVar(frame, 51, 0l, 0.1l, 120 - 50, 0);

			logo1midY = (int)LinearVar(frame, logo1midY0, logo1dstY, 51, 120);
			A = LinearVar(frame, 0, 400, 51, 120);
			logo1Rect.x = (int)LinearVar(frame, logo1X0, logo1dstX, 51, 120);
			logo1Rect.y = (int)(logo1midY + A * svar);
			logo1Rect.w = (int)LinearVar(frame, 0, logo1W, 51, 120);
			logo1Rect.h = (int)LinearVar(frame, 0, logo1H, 51, 120);

			SDL_RenderCopy(globalresources->rend, logo1Texture, &logo1Cut, &logo1Rect);
		}

		if (frame > 80 && frame <= 120)
		{
			SDL_SetTextureAlphaMod(globalresources->logoTexture, (int)LinearVar(frame, 0, 255, 51, 120));
			rectLogo.w = (int)(logoW * LinearVar(frame, 5, 1, 81, 120));
			rectLogo.h = (int)(logoH * LinearVar(frame, 5, 1, 81, 120));
			rectLogo.x = windowW / 2 - rectLogo.w / 2;
			rectLogo.y = (int)(windowH * 0.36 - rectLogo.h / 2);
			SDL_RenderCopy(globalresources->rend, globalresources->logoTexture, NULL, &rectLogo);

		}

		if (frame >= 120)
		{
			logo1Cut.x = (frame / 10) % 4 * (logo1W / 3);
			svar = SinVar(frame, 121, 0l, 0.1l, 120 - 50, 0);
			logo1Rect.y = (int)(logo1midY + A * svar);
			SDL_RenderCopy(globalresources->rend, globalresources->backgroundTexture, NULL, NULL);
			SDL_RenderCopy(globalresources->rend, globalresources->logoTexture, NULL, &rectLogo);
			SDL_RenderCopy(globalresources->rend, logo1Texture, &logo1Cut, &logo1Rect);
		}
		if (frame >= 150)
		{
			

			rectButtonStart.w = (int)(ButtonStartW * SinVar(frame, 151, 1l, 0.1, 100l, 1l));;
			rectButtonStart.h = (int)(ButtonStartH * SinVar(frame, 151, 1l, 0.1, 100l, 1l));
			rectButtonStart.x = windowW / 2 - rectButtonStart.w / 2;
			rectButtonStart.y = (int)(windowH * 0.6 - rectButtonStart.h / 2);

			SDL_RenderCopy(globalresources->rend, buttonstartTexture, NULL, &rectButtonStart);
			SDL_RenderCopy(globalresources->rend, buttonquitTexture, NULL, &rectButtonQuit);
			SDL_RenderCopy(globalresources->rend, buttonsettingsTexture, NULL, &rectButtonSettings);
			SDL_RenderCopy(globalresources->rend, buttonhelpTexture, NULL, &rectButtonHelp);
			SDL_RenderCopy(globalresources->rend, buttonalmanacTexture, NULL, &rectButtonAlmanac);
		}
		if (entersetting == SDL_TRUE)
		{
			SDL_SetCursor(globalresources->cursor_arrow);

			resources.settinguiTexture = CreateTexturewithBlitedText("Settings", 60, globalresources, FONT_PATH, 0, 0, 0, 255, "pictures/settingsui.png", 0, -160, ALIGN_CENTER, ALIGN_CENTER);
			resources.sensitivityTexture = CreateTexturewithBlitedText("sensitivity", 65, globalresources, FONT_PATH, 0, 0, 0, 255, "pictures/none.png", 0, 0, ALIGN_LEFT, ALIGN_CENTER);
			resources.slidersensTexture = GetPackedResource("pictures/slider.png", UNPACK_TEXTURE, globalresources);
			resources.closeTexture = GetPackedResource("pictures/close.png", UNPACK_TEXTURE, globalresources);

			resources.musicTexture = CreateTexturewithBlitedText("music", 65, globalresources, FONT_PATH, 0, 0, 0, 255, "pictures/none.png", 0, 0, ALIGN_LEFT, ALIGN_CENTER);
			resources.slidermusTexture = GetPackedResource("pictures/slider.png", UNPACK_TEXTURE, globalresources);

			resources.soundTexture = CreateTexturewithBlitedText("sound", 65, globalresources, FONT_PATH, 0, 0, 0, 255, "pictures/none.png", 0, 0, ALIGN_LEFT, ALIGN_CENTER);
			resources.slidersndTexture = GetPackedResource("pictures/slider.png", UNPACK_TEXTURE, globalresources);

			entersetting = SDL_FALSE;
			
		}
		if (setting == SDL_TRUE)
		{
			Settings(&resources, &parameters, globalresources);
		}

		if (quitsetting == SDL_TRUE)
		{


			SDL_DestroyTexture(resources.closeTexture);
			SDL_DestroyTexture(resources.sensitivityTexture);
			SDL_DestroyTexture(resources.settinguiTexture);
			SDL_DestroyTexture(resources.slidersensTexture);

			SDL_DestroyTexture(resources.musicTexture);
			SDL_DestroyTexture(resources.slidermusTexture);

			SDL_DestroyTexture(resources.soundTexture);
			SDL_DestroyTexture(resources.slidersndTexture);

			quitsetting = SDL_FALSE;
		}
		if (enterhelp == SDL_TRUE)
		{
			SDL_SetCursor(globalresources->cursor_arrow);

			resources.helpuiTexture = CreateTexturewithBlitedText("Help", 60, globalresources, FONT_PATH, 0, 0, 0, 255, "pictures/helpsui.png", 0, -295, ALIGN_CENTER, ALIGN_CENTER);
			resources.closehelpTexture = GetPackedResource("pictures/close.png", UNPACK_TEXTURE, globalresources);

			resources.keysTexture = GetPackedResource("pictures/keys.png", UNPACK_TEXTURE, globalresources);
			resources.texts1Texture = CreateTexturewithBlitedText("move", 40, globalresources, FONT_PATH, 0, 0, 0, 255,
				"pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
			resources.texts2Texture = CreateTexturewithBlitedText("shoot", 40, globalresources, FONT_PATH, 0, 0, 0, 255,
				"pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
			resources.texts3Texture = CreateTexturewithBlitedText("use skill", 40, globalresources, FONT_PATH, 0, 0, 0, 255,
				"pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
			resources.texts4Texture = CreateTexturewithBlitedText("pause", 40, globalresources, FONT_PATH, 0, 0, 0, 255,
				"pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);
			resources.texts5Texture = CreateTexturewithBlitedText("defence", 40, globalresources, FONT_PATH, 0, 0, 0, 255,
				"pictures/none.png", 0, 0, ALIGN_CENTER, ALIGN_CENTER);

			enterhelp = SDL_FALSE;
		}
		if (help == SDL_TRUE)
		{
			Help(&resources, &parameters, globalresources);
		}
		if (quithelp == SDL_TRUE)
		{
			SDL_DestroyTexture(resources.helpuiTexture);
			SDL_DestroyTexture(resources.closehelpTexture);

			SDL_DestroyTexture(resources.keysTexture);
			SDL_DestroyTexture(resources.texts1Texture);
			SDL_DestroyTexture(resources.texts2Texture);
			SDL_DestroyTexture(resources.texts3Texture);
			SDL_DestroyTexture(resources.texts4Texture);
			SDL_DestroyTexture(resources.texts5Texture);

			quithelp = SDL_FALSE;
		}
		if (almanac == SDL_TRUE)
		{
			AlmanacEventLoop(globalresources);
			almanac = SDL_FALSE;
		}


		SDL_RenderPresent(globalresources->rend);

		Delay(begin);
		SDL_RenderClear(globalresources->rend);
	}
	if (game == SDL_TRUE)
	{
		SDL_SetCursor(globalresources->cursor_arrow);
	}

	SDL_DestroyTexture(buttonstartTexture);
	SDL_DestroyTexture(buttonquitTexture);
	SDL_DestroyTexture(buttonsettingsTexture);
	SDL_DestroyTexture(buttonhelpTexture);
	SDL_DestroyTexture(buttonalmanacTexture);

	SDL_free(parameters.settingsuiRect);
	SDL_free(parameters.slidersensRect);
	SDL_free(parameters.closeRect);
	SDL_free(parameters.gearsensRect);

	SDL_free(parameters.gearmusRect);
	SDL_free(parameters.slidermusRect);

	SDL_free(parameters.gearsndRect);
	SDL_free(parameters.slidersndRect);

	SDL_free(parameters.helpuiRect);
	SDL_free(parameters.closehelpRect);

	parameters.settingsuiRect = NULL;
	parameters.slidersensRect = NULL;
	parameters.closeRect = NULL;
	parameters.gearsensRect = NULL;

	parameters.gearmusRect = NULL;
	parameters.slidermusRect = NULL;

	parameters.gearsndRect = NULL;
	parameters.slidersndRect = NULL;

	parameters.helpuiRect = NULL;
	parameters.closehelpRect = NULL;

	Mix_HaltChannel(musicchannel);
}

void SettingsEvents(SDL_Event event, DragRecord* dragrecord, StartParameters* parameters, SDL_bool* setting, SDL_bool* quitsetting, GlobalResources* globalresources)
{
	static SDL_bool presound = SDL_FALSE;
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.x >= parameters->settingsuiRect->x && event.button.x <= parameters->settingsuiRect->x + parameters->settingsuiRect->w
			&& event.button.y >= parameters->settingsuiRect->y && event.button.y <= parameters->settingsuiRect->y + parameters->settingsuiRect->h
			&&
			!(event.button.x >= parameters->gearsensRect->x && event.button.x <= parameters->gearsensRect->x + parameters->gearsensRect->w
				&& event.button.y >= parameters->gearsensRect->y && event.button.y <= parameters->gearsensRect->y + parameters->gearsensRect->h)
			&&
			!(event.button.x >= parameters->gearmusRect->x && event.button.x <= parameters->gearmusRect->x + parameters->gearmusRect->w
				&& event.button.y >= parameters->gearmusRect->y && event.button.y <= parameters->gearmusRect->y + parameters->gearmusRect->h)
			&&
			!(event.button.x >= parameters->gearsndRect->x && event.button.x <= parameters->gearsndRect->x + parameters->gearsndRect->w
				&& event.button.y >= parameters->gearsndRect->y && event.button.y <= parameters->gearsndRect->y + parameters->gearsndRect->h)
			&&
			!(event.button.x >= parameters->closeRect->x && event.button.x <= parameters->closeRect->x + parameters->closeRect->w
				&& event.button.y >= parameters->closeRect->y && event.button.y <= parameters->closeRect->y + parameters->closeRect->h))
		{
			if (dragrecord->mousepressed == SDL_FALSE)
			{
				dragrecord->clickX = event.button.x;
				dragrecord->clickY = event.button.y;

				dragrecord->Xwhileclicked = parameters->settingsuiRect->x;
				dragrecord->Ywhileclicked = parameters->settingsuiRect->y;
			}
			dragrecord->mousepressed = SDL_TRUE;

		}
		if (event.button.x >= parameters->gearsndRect->x && event.button.x <= parameters->gearsndRect->x + parameters->gearsndRect->w
			&& event.button.y >= parameters->gearsndRect->y && event.button.y <= parameters->gearsndRect->y + parameters->gearsndRect->h)
		{
			presound = SDL_TRUE;
		}
		if (event.button.x >= parameters->closeRect->x && event.button.x <= parameters->closeRect->x + parameters->closeRect->w
			&& event.button.y >= parameters->closeRect->y && event.button.y <= parameters->closeRect->y + parameters->closeRect->h)
		{
			Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
		}
	}
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		dragrecord->mousepressed = SDL_FALSE;
		if (event.button.x >= parameters->closeRect->x && event.button.x <= parameters->closeRect->x + parameters->closeRect->w
			&& event.button.y >= parameters->closeRect->y && event.button.y <= parameters->closeRect->y + parameters->closeRect->h)
		{
			*setting = SDL_FALSE;
			*quitsetting = SDL_TRUE;
		}
		if (presound == SDL_TRUE)
		{
			Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
			presound = SDL_FALSE;
		}
	}
	if (event.type == SDL_MOUSEMOTION)
	{
		dragrecord->mouseX = event.motion.x;
		dragrecord->mouseY = event.motion.y;

		if (dragrecord->mousepressed == SDL_TRUE)
		{
			parameters->settingsuiRect->x = dragrecord->Xwhileclicked + dragrecord->mouseX - dragrecord->clickX;
			parameters->settingsuiRect->y = dragrecord->Ywhileclicked + dragrecord->mouseY - dragrecord->clickY;

			if (parameters->settingsuiRect->x >= windowW - parameters->settingsuiRect->w)
			{
				parameters->settingsuiRect->x = windowW - parameters->settingsuiRect->w - 1;
			}
			if (parameters->settingsuiRect->y >= windowH - parameters->settingsuiRect->h)
			{
				parameters->settingsuiRect->y = windowH - parameters->settingsuiRect->h - 1;
			}
			if (parameters->settingsuiRect->x < 0)
			{
				parameters->settingsuiRect->x = 0;
			}
			if (parameters->settingsuiRect->y < 0)
			{
				parameters->settingsuiRect->y = 0;
			}
		}
	}
}

void GearSensEvents(SDL_Event event, DragRecord* dragrecord, StartParameters* parameters)
{
	int x_min = parameters->slidersensRect->x;
	int x_max = parameters->slidersensRect->x + parameters->slidersensRect->w;
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.x >= parameters->gearsensRect->x && event.button.x <= parameters->gearsensRect->x + parameters->gearsensRect->w
			&& event.button.y >= parameters->gearsensRect->y && event.button.y <= parameters->gearsensRect->y + parameters->gearsensRect->h)
		{
			if (dragrecord->mousepressed == SDL_FALSE)
			{
				dragrecord->clickX = event.button.x;
				dragrecord->clickY = event.button.y;

				dragrecord->Xwhileclicked = parameters->gearsensRect->x;

			}
			dragrecord->mousepressed = SDL_TRUE;

		}
	}
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		dragrecord->mousepressed = SDL_FALSE;
	}
	if (event.type == SDL_MOUSEMOTION)
	{
		dragrecord->mouseX = event.motion.x;
		dragrecord->mouseY = event.motion.y;

		if (dragrecord->mousepressed == SDL_TRUE)
		{
			parameters->gearsensRect->x = dragrecord->Xwhileclicked + dragrecord->mouseX - dragrecord->clickX;

			if (parameters->gearsensRect->x > x_max)
			{
				parameters->gearsensRect->x = x_max;
			}
			if (parameters->gearsensRect->x < x_min)
			{
				parameters->gearsensRect->x = x_min;
			}
			sensitivity = (parameters->gearsensRect->x + parameters->gearsensRect->w / 2 - parameters->slidersensRect->x) *
				(parameters->sensitivity_max - parameters->sensitivity_min) / parameters->slidersensRect->w + parameters->sensitivity_min;

		}
	}
}

void ShowSensitivityGear(int w, int h, StartParameters* parameters, GlobalResources* globalresources)
{


	parameters->gearsensRect->x = (int)(parameters->slidersensRect->x + ((double)sensitivity - parameters->sensitivity_min) / ((double)parameters->sensitivity_max - parameters->sensitivity_min) * parameters->slidersensRect->w - w / 2);
	parameters->gearsensRect->y = parameters->slidersensRect->y + parameters->slidersensRect->h / 2 - h / 2;
	parameters->gearsensRect->w = w;
	parameters->gearsensRect->h = h;

	SDL_Texture* gearTexture = GetPackedResource("pictures/gear.png", UNPACK_TEXTURE, globalresources);
	SDL_RenderCopy(globalresources->rend, gearTexture, NULL, parameters->gearsensRect);
	SDL_DestroyTexture(gearTexture);
}

void GearMusEvents(SDL_Event event, DragRecord* dragrecord, StartParameters* parameters)
{
	int x_min = parameters->slidermusRect->x;
	int x_max = parameters->slidermusRect->x + parameters->slidermusRect->w;
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.x >= parameters->gearmusRect->x && event.button.x <= parameters->gearmusRect->x + parameters->gearmusRect->w
			&& event.button.y >= parameters->gearmusRect->y && event.button.y <= parameters->gearmusRect->y + parameters->gearmusRect->h)
		{
			if (dragrecord->mousepressed == SDL_FALSE)
			{
				dragrecord->clickX = event.button.x;
				dragrecord->clickY = event.button.y;

				dragrecord->Xwhileclicked = parameters->gearmusRect->x;

			}
			dragrecord->mousepressed = SDL_TRUE;

		}
	}
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		dragrecord->mousepressed = SDL_FALSE;
	}
	if (event.type == SDL_MOUSEMOTION)
	{
		dragrecord->mouseX = event.motion.x;
		dragrecord->mouseY = event.motion.y;

		if (dragrecord->mousepressed == SDL_TRUE)
		{
			parameters->gearmusRect->x = dragrecord->Xwhileclicked + dragrecord->mouseX - dragrecord->clickX;

			if (parameters->gearmusRect->x > x_max)
			{
				parameters->gearmusRect->x = x_max;
			}
			if (parameters->gearmusRect->x < x_min)
			{
				parameters->gearmusRect->x = x_min;
			}
			musicVol = (int)((parameters->gearmusRect->x + parameters->gearmusRect->w / 2 - parameters->slidermusRect->x) *
				(parameters->musicVol_max - parameters->musicVol_min) / parameters->slidermusRect->w + parameters->musicVol_min)
				* 8;
			Mix_Volume(musicchannel, musicVol);
		}
	}
}

void GearSndEvents(SDL_Event event, DragRecord* dragrecord, StartParameters* parameters)
{
	int x_min = parameters->slidersndRect->x;
	int x_max = parameters->slidersndRect->x + parameters->slidersndRect->w;
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.x >= parameters->gearsndRect->x && event.button.x <= parameters->gearsndRect->x + parameters->gearsndRect->w
			&& event.button.y >= parameters->gearsndRect->y && event.button.y <= parameters->gearsndRect->y + parameters->gearsndRect->h)
		{
			if (dragrecord->mousepressed == SDL_FALSE)
			{
				dragrecord->clickX = event.button.x;
				dragrecord->clickY = event.button.y;

				dragrecord->Xwhileclicked = parameters->gearsndRect->x;

			}
			dragrecord->mousepressed = SDL_TRUE;

		}
	}
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		dragrecord->mousepressed = SDL_FALSE;
	}
	if (event.type == SDL_MOUSEMOTION)
	{
		dragrecord->mouseX = event.motion.x;
		dragrecord->mouseY = event.motion.y;

		if (dragrecord->mousepressed == SDL_TRUE)
		{
			parameters->gearsndRect->x = dragrecord->Xwhileclicked + dragrecord->mouseX - dragrecord->clickX;

			if (parameters->gearsndRect->x > x_max)
			{
				parameters->gearsndRect->x = x_max;
			}
			if (parameters->gearsndRect->x < x_min)
			{
				parameters->gearsndRect->x = x_min;
			}
			soundVol = (int)((parameters->gearsndRect->x + parameters->gearsndRect->w / 2 - parameters->slidersndRect->x) *
				(parameters->soundVol_max - parameters->soundVol_min) / parameters->slidersndRect->w + parameters->soundVol_min)
				* 8;
			int i;
			for (i = 0; i < CHANNEL_AMOUNT; i++)
			{
				if (i != musicchannel)
				{
					Mix_Volume(i, soundVol);
				}
			}
		}
	}
}

void ShowMusicGear(int w, int h, StartParameters* parameters, GlobalResources* globalresources)
{

	parameters->gearmusRect->x = (int)(parameters->slidermusRect->x + (musicVol / 8 - (double)parameters->musicVol_min) / ((double)parameters->musicVol_max - parameters->musicVol_min) * parameters->slidermusRect->w - w / 2);
	parameters->gearmusRect->y = parameters->slidermusRect->y + parameters->slidermusRect->h / 2 - h / 2;
	parameters->gearmusRect->w = w;
	parameters->gearmusRect->h = h;

	SDL_Texture* gearTexture = GetPackedResource("pictures/gear.png", UNPACK_TEXTURE, globalresources);
	SDL_RenderCopy(globalresources->rend, gearTexture, NULL, parameters->gearmusRect);
	SDL_DestroyTexture(gearTexture);
}

void ShowSoundGear(int w, int h, StartParameters* parameters, GlobalResources* globalresources)
{

	parameters->gearsndRect->x = (int)(parameters->slidersndRect->x + (soundVol / 8 - (double)parameters->soundVol_min) / ((double)parameters->soundVol_max - parameters->soundVol_min) * parameters->slidersndRect->w - w / 2);
	parameters->gearsndRect->y = parameters->slidersndRect->y + parameters->slidersndRect->h / 2 - h / 2;
	parameters->gearsndRect->w = w;
	parameters->gearsndRect->h = h;

	SDL_Texture* gearTexture = GetPackedResource("pictures/gear.png", UNPACK_TEXTURE, globalresources);
	SDL_RenderCopy(globalresources->rend, gearTexture, NULL, parameters->gearsndRect);
	SDL_DestroyTexture(gearTexture);
}

void Settings(StartResources* resource, StartParameters* parameters, GlobalResources* globalresources)
{
	const int sensitivityW = 150;
	const int sensitivityH = 32;

	const int musicW = 150;
	const int musicH = 32;

	const int soundW = 150;
	const int soundH = 32;

	const int sliderW = 300;
	const int sliderH = 12;

	const int closeW = 30;
	const int closeH = 30;

	SDL_Rect sensitivityRect = { parameters->settingsuiRect->x + parameters->settingsuiRect->w / 2 - 170, parameters->settingsuiRect->y + 95,
		sensitivityW , sensitivityH };
	SDL_Rect musicRect = { parameters->settingsuiRect->x + parameters->settingsuiRect->w / 2 - 170, parameters->settingsuiRect->y + 150,
		musicW , musicH };
	SDL_Rect soundRect = { parameters->settingsuiRect->x + parameters->settingsuiRect->w / 2 - 170, parameters->settingsuiRect->y + 205,
		soundW , soundH };

	parameters->slidersensRect->x = parameters->settingsuiRect->x + parameters->settingsuiRect->w / 2 - sliderW / 2;
	parameters->slidersensRect->y = parameters->settingsuiRect->y + 125;
	parameters->slidersensRect->w = sliderW;
	parameters->slidersensRect->h = sliderH;

	parameters->slidermusRect->x = parameters->settingsuiRect->x + parameters->settingsuiRect->w / 2 - sliderW / 2;
	parameters->slidermusRect->y = parameters->settingsuiRect->y + 180;
	parameters->slidermusRect->w = sliderW;
	parameters->slidermusRect->h = sliderH;

	parameters->slidersndRect->x = parameters->settingsuiRect->x + parameters->settingsuiRect->w / 2 - sliderW / 2;
	parameters->slidersndRect->y = parameters->settingsuiRect->y + 235;
	parameters->slidersndRect->w = sliderW;
	parameters->slidersndRect->h = sliderH;

	parameters->closeRect->x = parameters->settingsuiRect->x + parameters->settingsuiRect->w - 40 - closeW / 2;
	parameters->closeRect->y = parameters->settingsuiRect->y + 40 - closeH / 2;
	parameters->closeRect->w = closeW;
	parameters->closeRect->h = closeH;

	SDL_RenderCopy(globalresources->rend, resource->settinguiTexture, NULL, parameters->settingsuiRect);

	SDL_RenderCopy(globalresources->rend, resource->sensitivityTexture, NULL, &sensitivityRect);
	SDL_RenderCopy(globalresources->rend, resource->musicTexture, NULL, &musicRect);
	SDL_RenderCopy(globalresources->rend, resource->soundTexture, NULL, &soundRect);
	SDL_RenderCopy(globalresources->rend, resource->slidersensTexture, NULL, parameters->slidersensRect);
	ShowSensitivityGear(25, 25, parameters, globalresources);

	SDL_RenderCopy(globalresources->rend, resource->slidermusTexture, NULL, parameters->slidermusRect);
	ShowMusicGear(25, 25, parameters, globalresources);

	SDL_RenderCopy(globalresources->rend, resource->slidersndTexture, NULL, parameters->slidersndRect);
	ShowSoundGear(25, 25, parameters, globalresources);

	SDL_RenderCopy(globalresources->rend, resource->closeTexture, NULL, parameters->closeRect);
}

void HelpEvents(SDL_Event event, DragRecord* dragrecord, StartParameters* parameters, SDL_bool* help, SDL_bool* quithelp, GlobalResources* globalresources)
{

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.x >= parameters->helpuiRect->x && event.button.x <= parameters->helpuiRect->x + parameters->helpuiRect->w
			&& event.button.y >= parameters->helpuiRect->y && event.button.y <= parameters->helpuiRect->y + parameters->helpuiRect->h
			&&
			!(event.button.x >= parameters->closehelpRect->x && event.button.x <= parameters->closehelpRect->x + parameters->closehelpRect->w
				&& event.button.y >= parameters->closehelpRect->y && event.button.y <= parameters->closehelpRect->y + parameters->closehelpRect->h))
		{
			if (dragrecord->mousepressed == SDL_FALSE)
			{
				dragrecord->clickX = event.button.x;
				dragrecord->clickY = event.button.y;

				dragrecord->Xwhileclicked = parameters->helpuiRect->x;
				dragrecord->Ywhileclicked = parameters->helpuiRect->y;
			}
			dragrecord->mousepressed = SDL_TRUE;

		}
		if (event.button.x >= parameters->closehelpRect->x && event.button.x <= parameters->closehelpRect->x + parameters->closehelpRect->w
			&& event.button.y >= parameters->closehelpRect->y && event.button.y <= parameters->closehelpRect->y + parameters->closehelpRect->h)
		{
			Mix_PlayChannel(-1, globalresources->buttonclickSound, 0);
		}

	}
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		dragrecord->mousepressed = SDL_FALSE;
		if (event.button.x >= parameters->closehelpRect->x && event.button.x <= parameters->closehelpRect->x + parameters->closehelpRect->w
			&& event.button.y >= parameters->closehelpRect->y && event.button.y <= parameters->closehelpRect->y + parameters->closehelpRect->h)
		{
			*help = SDL_FALSE;
			*quithelp = SDL_TRUE;
		}
	}
	if (event.type == SDL_MOUSEMOTION)
	{
		dragrecord->mouseX = event.motion.x;
		dragrecord->mouseY = event.motion.y;

		if (dragrecord->mousepressed == SDL_TRUE)
		{
			parameters->helpuiRect->x = dragrecord->Xwhileclicked + dragrecord->mouseX - dragrecord->clickX;
			parameters->helpuiRect->y = dragrecord->Ywhileclicked + dragrecord->mouseY - dragrecord->clickY;

			if (parameters->helpuiRect->x >= windowW - parameters->helpuiRect->w)
			{
				parameters->helpuiRect->x = windowW - parameters->helpuiRect->w - 1;
			}
			if (parameters->helpuiRect->y >= windowH - parameters->helpuiRect->h)
			{
				parameters->helpuiRect->y = windowH - parameters->helpuiRect->h - 1;
			}
			if (parameters->helpuiRect->x < 0)
			{
				parameters->helpuiRect->x = 0;
			}
			if (parameters->helpuiRect->y < 0)
			{
				parameters->helpuiRect->y = 0;
			}
		}
	}
}

void Help(StartResources* resource, StartParameters* parameters, GlobalResources* globalresources)
{
	const int closeW = 30;
	const int closeH = 30;

	const int keysW = 170;
	const int keysH = 360;

	parameters->closehelpRect->x = parameters->helpuiRect->x + parameters->helpuiRect->w - 40 - closeW / 2;
	parameters->closehelpRect->y = parameters->helpuiRect->y + 40 - closeH / 2;
	parameters->closehelpRect->w = closeW;
	parameters->closehelpRect->h = closeH;

	SDL_Rect keysRect = { parameters->helpuiRect->x + 70, parameters->helpuiRect->y + 105 , keysW , keysH };
	SDL_Rect texts1Rect = { parameters->helpuiRect->x + 160, parameters->helpuiRect->y + 128 , 400 , 100 };
	SDL_Rect texts2Rect = { parameters->helpuiRect->x + 160, parameters->helpuiRect->y + 194 , 400 , 100 };
	SDL_Rect texts3Rect = { parameters->helpuiRect->x + 160, parameters->helpuiRect->y + 260 , 400 , 100 };
	SDL_Rect texts4Rect = { parameters->helpuiRect->x + 160, parameters->helpuiRect->y + 326 , 400 , 100 };
	SDL_Rect texts5Rect = { parameters->helpuiRect->x + 160, parameters->helpuiRect->y + 392 , 400 , 100 };



	SDL_RenderCopy(globalresources->rend, resource->helpuiTexture, NULL, parameters->helpuiRect);
	SDL_RenderCopy(globalresources->rend, resource->closehelpTexture, NULL, parameters->closehelpRect);

	SDL_RenderCopy(globalresources->rend, resource->keysTexture, NULL, &keysRect);
	SDL_RenderCopy(globalresources->rend, resource->texts1Texture, NULL, &texts1Rect);
	SDL_RenderCopy(globalresources->rend, resource->texts2Texture, NULL, &texts2Rect);
	SDL_RenderCopy(globalresources->rend, resource->texts3Texture, NULL, &texts3Rect);
	SDL_RenderCopy(globalresources->rend, resource->texts4Texture, NULL, &texts4Rect);
	SDL_RenderCopy(globalresources->rend, resource->texts5Texture, NULL, &texts5Rect);
}