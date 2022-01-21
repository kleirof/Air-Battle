#define _CRT_SECURE_NO_WARNINGS 1

#pragma execution_character_set("utf-8")

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define BULLET_AMOUNT_MAX 20
#define ENEMY_AMOUNT_MAX 10
#define BLITEDTEXTURE_AMOUNT_MAX 500
#define ENEMYBULLET_AMOUNT_MAX 50
#define RAGE_MAX 100
#define SKILL_ANOUNT_MAX 2
#define BOMB_AMOUNT_MAX 2
#define WAVE_AMOUNT_MAX 5
#define CARD_AMOUNT_MAX 6
#define BOSS_ATTACKMODE_AMOUNT_MAX 3
#define BOSS_BULLET_AMOUNT_MAX 20
#define SHORTATTACK_AMOUNT_MAX 5
#define CHANNEL_AMOUNT 16

#define FRAMERATE 60

#define PI 3.14159265359
#define FONT_PATH "font/font.ttf"

extern const int windowW;
extern const int windowH;

extern int sensitivity;
extern int musicVol;
extern int soundVol;

extern int musicchannel;

extern SDL_bool quit;
extern SDL_bool game;

extern SDL_bool trackmode;

extern int playerlife;
extern int score;
extern int rage;

extern double LinearVar(int t, int fromX, int toX, int fromT, int toT);

extern double SinVar(int t, int fromT, double fromX, double A, double cycle, double h);

extern double CalcComponentX(int x, int y, int dst_x, int dst_y);

extern double CalcComponentY(int x, int y, int dst_x, int dst_y);

extern void Delay(uint32_t begin);

