#include "main.h"
#include "globals.h"

#define ENEMY_KIND_1 1
#define ENEMY_KIND_2 2
#define ENEMY_KIND_3 3
#define ENEMY_KIND_4 4
#define ENEMY_KIND_5 5

#define INITENEMYMODE_1 1
#define INITENEMYMODE_2 2
#define INITENEMYMODE_3 3
#define INITENEMYMODE_4 4
#define INITENEMYMODE_5 5

#ifndef OBJECTS_H

#define OBJECTS_H

typedef struct Bullet
{
	int ly, lx;
	int x, y;
	int w, h;
	double speed;
	double speed_x, speed_y;
	double acc;
	double acc_x, acc_y;
	double angle;
	SDL_bool is_tracking;
} Bullet;



typedef struct Enemy
{
	int ID;
	int score;
	int rage;
	double lx, ly;
	int x, y;
	int w, h;
	int hp;
	double speed;
	double bsc_speed;
	double ext_speed;
	double delX;
	double last_delX;
	double angle;
	SDL_bool is_aggresive;
	uint32_t last_attack_time;
	uint32_t born_time;
	SDL_Texture* tex;
	int A;
	int cycle;
} Enemy;



typedef struct BlitedTexture
{
	int x, y;
	int w, h;
	int frame;
	int totalframe;
	uint32_t last_refresh_time;
	int frame_width, frame_height;
	int framerate;
	SDL_Rect rect;
	SDL_Texture* tex;
} BlitedTexture;



typedef struct EnemyBullet
{
	double lx, ly;
	int x, y;
	double speed;
	double speed_x, speed_y;
	uint32_t last_shoot_time;
} EnemyBullet;



typedef struct Bomb
{
	double lx, ly;
	int x, y;
	int w, h;
	double speed;
	double speed_x, speed_y;
	double acc;
	double acc_x, acc_y;
	double angle;
} Bomb;



typedef struct ObjectsParameters
{
	const int bulletW;
	const int bulletH;

	const int enemybulletW;
	const int enemybulletH;
} ObjectsParameters;

#endif

Bullet* bullets[BULLET_AMOUNT_MAX];

Enemy* enemies[ENEMY_AMOUNT_MAX];

int EnemyList[ENEMY_AMOUNT_MAX];

BlitedTexture* blitedtextures[BLITEDTEXTURE_AMOUNT_MAX];

EnemyBullet* enemybullets[ENEMYBULLET_AMOUNT_MAX];

Bomb* bombs[BOMB_AMOUNT_MAX];

extern void ShootBullet(int x, int y, ObjectsParameters parameters, GlobalParameters parameters2);

extern void DestroyBullet(Bullet** pp_bullet);

extern void DestroyAllBullets(Bullet* bullets[], int bullet_amount);

extern void DrawBullet(SDL_Renderer* renderer, SDL_Texture* bulletTexture, ObjectsParameters parameters);

extern void MoveBullet(ObjectsParameters parameters);

extern void InitEnemyMode(Uint32 flags);

extern void AddEnemy(int List[], int num_left, uint32_t gametime, GlobalResources* globalresources);

extern void DestroyAllEnemies(Enemy* enemies[], int enemy_amount);

extern void DrawEnemy(GlobalResources* globalresources);

extern void MoveEnemy(GlobalResources* globalresources);

extern void ChangeEnemyRout(uint32_t gametime);

extern void IfShot(int* p_score, int* p_rage, ObjectsParameters parameters, GlobalResources* globalresource);

extern void InitBlitedTexture(GlobalResources* globalresources);

extern void DrawEnemy(GlobalResources* globalresources);

extern void DestroyAllBlitedTexture(BlitedTexture* blitedtextures[], int blitedtexture_amount);

extern void PlayAllBlitedTexture(uint32_t gametime);

extern void ShowAllBlitedTexture(GlobalResources* globalresources);

extern void AddExplosionTexture(int x, int y, int w, int h);

extern void AddEnemyMist(int framerate, uint32_t gametime, uint32_t* p_mistT);

extern void AggresiveEnemyAttack(uint32_t gametime, int time, ObjectsParameters parameters, GlobalResources* globalresources);

extern void MoveEnemyBullet();

extern void DrawEnemyBullets(ObjectsParameters parameters, GlobalResources* globalresources);

extern void DestroyEnemyBullet(EnemyBullet** pp_eneblt);

extern void IfPlayerShot(ObjectsParameters parameters, GlobalResources* globalresources);

extern void DestroyAllEnemyBullets(EnemyBullet* enemybullets[], int blitedtexture_amount);

extern void BulletTracking(int framerate, uint32_t gametime, uint32_t* p_trackT);

extern void AddBomb(int x, int y, struct SkillParameters parameters);

extern void DestroyAllBombs(Bomb* bombs[], int bomb_amount);

extern void DrawBomb(GlobalResources* globalresources);

extern void MoveBomb();

extern void BombExplode();

extern void AddBombMist(int framerate, uint32_t gametime, uint32_t* p_bombmistT);

