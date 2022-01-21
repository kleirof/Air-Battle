#include "main.h"
#include "globals.h"

#define CURSOR_HAND 1
#define CURSOR_ARROW 2

#define LEN_MAX 1024
#define MAX_PATH 260

#define PAK_FILE "./contents.pak"
#define TMP_FILE "./tmp"

#define UNPACK_TEXTURE 0
#define UNPACK_CHUNK 1
#define UNPACK_SURFACE 2

#ifndef RESOURCE_H

#define RESOURCE_H

typedef enum TextBlitMode
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
} TextBlitMode;

#endif

extern SDL_Texture* CreateTextureFromText(const char* Fonts, int FontSize, SDL_Renderer* Render, const char* FontsPath, int r, int g, int b, int a);

extern SDL_Surface* CreateSurfaceFromText(const char* Fonts, int FontSize, SDL_Renderer* Render, const char* FontsPath, int r, int g, int b, int a);

extern SDL_Texture* CreateTexturewithBlitedText(const char* Fonts, int FontSize, GlobalResources* globalresources, const char* FontsPath, int r, int g, int b, int a,
	const char* FileName, int delX, int delY, TextBlitMode modeX, TextBlitMode modeY);

extern SDL_Texture* CreateTexturewithBlitedTextFromSurface(const char* Fonts, int FontSize, GlobalResources* globalresources, const char* FontsPath, int r, int g, int b, int a,
	SDL_Surface* dstSurface, int delX, int delY, TextBlitMode modeX, TextBlitMode modeY);

extern void InitIndex(GlobalResources* globalresources);

extern void* GetPackedResource(char* respath, int mode, GlobalResources* globalresources);

extern void ClearTemp(SDL_RWops* rw, char* outfile);