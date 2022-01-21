#include "resource.h"

static int GetFileNum(GlobalResources* globalresources);

static SDL_RWops* GetCertainData(char* filename, GlobalResources* globalresources);

SDL_Texture* CreateTextureFromText(const char* Fonts, int FontSize, SDL_Renderer* Render, const char* FontsPath, int r, int g, int b, int a)
{
	TTF_Init();
	TTF_Font* Font = TTF_OpenFont(FONT_PATH, FontSize);
	SDL_Color color = { r, g, b, a };
	SDL_Surface* temp = TTF_RenderUTF8_Blended(Font, Fonts, color);
	SDL_Texture* fonts = SDL_CreateTextureFromSurface(Render, temp);
	SDL_FreeSurface(temp);
	TTF_CloseFont(Font);
	TTF_Quit();
	if (fonts != NULL)
	{
		return fonts;
	}
	else
	{
		return NULL;
	}
}

SDL_Surface* CreateSurfaceFromText(const char* Fonts, int FontSize, SDL_Renderer* Render, const char* FontsPath, int r, int g, int b, int a)
{
	TTF_Init();
	TTF_Font* Font = TTF_OpenFont(FONT_PATH, FontSize);
	SDL_Color color = { r, g, b, a };
	SDL_Surface* temp = TTF_RenderUTF8_Blended(Font, Fonts, color);
	TTF_CloseFont(Font);
	TTF_Quit();
	if (temp != NULL)
	{
		return temp;
	}
	else
	{
		return NULL;
	}
}

SDL_Texture* CreateTexturewithBlitedText(const char* Fonts, int FontSize, GlobalResources* globalresources, const char* FontsPath, int r, int g, int b, int a,
	const char* FileName, int delX, int delY, TextBlitMode modeX, TextBlitMode modeY)
{
	SDL_Surface* dstSurface = GetPackedResource(FileName, UNPACK_SURFACE, globalresources);
	SDL_Surface* rscSurface = CreateSurfaceFromText(Fonts, FontSize, globalresources->rend, FontsPath, r, g, b, a);
	SDL_Rect Rect = { 0 , 0 , rscSurface->w , rscSurface->h };
	switch (modeX)
	{
	case ALIGN_LEFT:
		Rect.x = 0 + delX;
		break;
	case ALIGN_CENTER:
		Rect.x = dstSurface->w / 2 - rscSurface->w / 2 + delX;
		break;
	case ALIGN_RIGHT:
		Rect.x = dstSurface->w - rscSurface->w + delX;
		break;
	}
	switch (modeY)
	{
	case ALIGN_LEFT:
		Rect.y = 0 + delY;
		break;
	case ALIGN_CENTER:
		Rect.y = dstSurface->h / 2 - rscSurface->h / 2 + delY;
		break;
	case ALIGN_RIGHT:
		Rect.y = dstSurface->h - rscSurface->h + delY;
		break;
	}
	SDL_BlitScaled(rscSurface, NULL, dstSurface, &Rect);
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(globalresources->rend, dstSurface);
	SDL_FreeSurface(dstSurface);
	SDL_FreeSurface(rscSurface);
	return Texture;
}

SDL_Texture* CreateTexturewithBlitedTextFromSurface(const char* Fonts, int FontSize, GlobalResources* globalresources, const char* FontsPath, int r, int g, int b, int a,
	SDL_Surface* dstSurface, int delX, int delY, TextBlitMode modeX, TextBlitMode modeY)
{
	SDL_Surface* dstSurface1 = SDL_DuplicateSurface(dstSurface);
	SDL_Surface* rscSurface = CreateSurfaceFromText(Fonts, FontSize, globalresources->rend, FontsPath, r, g, b, a);
	SDL_Rect Rect = { 0 , 0 , rscSurface->w , rscSurface->h };
	switch (modeX)
	{
	case ALIGN_LEFT:
		Rect.x = 0 + delX;
		break;
	case ALIGN_CENTER:
		Rect.x = dstSurface1->w / 2 - rscSurface->w / 2 + delX;
		break;
	case ALIGN_RIGHT:
		Rect.x = dstSurface1->w - rscSurface->w + delX;
		break;
	}
	switch (modeY)
	{
	case ALIGN_LEFT:
		Rect.y = 0 + delY;
		break;
	case ALIGN_CENTER:
		Rect.y = dstSurface1->h / 2 - rscSurface->h / 2 + delY;
		break;
	case ALIGN_RIGHT:
		Rect.y = dstSurface1->h - rscSurface->h + delY;
		break;
	}
	SDL_BlitScaled(rscSurface, NULL, dstSurface1, &Rect);
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(globalresources->rend, dstSurface1);
	SDL_FreeSurface(dstSurface1);
	SDL_FreeSurface(rscSurface);
	return Texture;
}

int GetFileNum(GlobalResources* globalresources)
{
	int filenum = 0;
	SDL_RWseek(globalresources->pak, 0, RW_SEEK_SET);
	SDL_RWread(globalresources->pak, &filenum, sizeof(int), 1);
	return filenum;
}

void GetIndex(GlobalResources* globalresources)
{
	SDL_RWseek(globalresources->pak, sizeof(int), RW_SEEK_SET);
	int i;
	for (i = 0; i < globalresources->filenum; i++)
	{
		SDL_RWread(globalresources->pak, globalresources->p_index + i, sizeof(Index), 1);
	}
}

void InitIndex(GlobalResources* globalresources)
{
	globalresources->filenum = GetFileNum(globalresources);
	globalresources->p_index = SDL_calloc(globalresources->filenum, sizeof(Index));
	GetIndex(globalresources);
}

SDL_RWops* GetCertainData(char* filename, GlobalResources* globalresources)
{
	SDL_RWseek(globalresources->pak, 0, RW_SEEK_SET);
	SDL_RWops* tmp = SDL_RWFromFile(TMP_FILE, "wb+");

	int offset = 0;
	int i;
	for (i = 0; i < globalresources->filenum; i++)
	{
		if (strcmp((globalresources->p_index + i)->filename, filename) == 0)
		{
			offset = (globalresources->p_index + i)->offset;
			break;
		}
	}
	SDL_RWseek(globalresources->pak, offset, RW_SEEK_SET);
	unsigned char buf[LEN_MAX];
	int rc;
	while ((rc = SDL_RWread(globalresources->pak, buf, sizeof(unsigned char), LEN_MAX)) != 0)
	{
		SDL_RWwrite(tmp, buf, sizeof(unsigned char), rc);
	}

	return tmp;
}

void ClearTemp(SDL_RWops* rw, char* outfile)
{
	unsigned char buf[LEN_MAX];
	int rc;
	while ((rc = SDL_RWread(rw, buf, sizeof(unsigned char), LEN_MAX)) != 0)
	{
		SDL_RWseek(rw, -rc, RW_SEEK_CUR);
		SDL_RWwrite(rw, "", sizeof(unsigned char), rc);
	}
	SDL_RWclose(rw);
	remove(outfile);
}

void* GetPackedResource(char* respath, int mode, GlobalResources* globalresources)
{
	SDL_RWseek(globalresources->pak, 0, RW_SEEK_SET);
	SDL_RWops* rw = GetCertainData(respath, globalresources);
	SDL_RWseek(rw, 0, RW_SEEK_SET);
	void* res = NULL;
	if (mode == UNPACK_TEXTURE)
	{
		res = IMG_LoadTexture_RW(globalresources->rend, rw, 0);
	}
	else if (mode == UNPACK_CHUNK)
	{
		res = Mix_LoadWAV_RW(rw, 0);
	}
	else if (mode == UNPACK_SURFACE)
	{
		res = IMG_Load_RW(rw, 0);
	}
	ClearTemp(rw, TMP_FILE);
	return res;
}
