#include "main.h"
#include "globals.h"
#include "objects.h"

#define BOSS_ATTACKMODE_0 0
#define BOSS_ATTACKMODE_1 1
#define BOSS_ATTACKMODE_2 2
#define BOSS_ATTACKMODE_3 3

#define BOSS_BULLET_TYPE_0 0
#define BOSS_BULLET_TYPE_1 1
#define BOSS_BULLET_TYPE_2 2
#define BOSS_BULLET_TYPE_3 3

typedef struct AttackMode
{
	int mode;
	uint32_t last_modetime;
	int duration;
	uint32_t last_attacktime;
	int attack_interval;
	SDL_bool coexist;
} AttackMode;

typedef struct Boss
{
	double lx, ly;
	int x, y;
	int w, h;
	double angle;
	double speed_x, speed_y;
	int life;
	uint32_t last_movetime;
	int moveduration;
	AttackMode* attackmode[BOSS_ATTACKMODE_AMOUNT_MAX];
	SDL_bool is_automove;
} Boss;

Boss* boss;

typedef struct BossBullet
{
	int bullet_type;
	double lx, ly;
	int x, y;
	int w, h;
	double speed;
	double speed_x, speed_y;
	double acc;
	double acc_x, acc_y;
	double angle;
	uint32_t createtime;
	SDL_bool tracking;
	SDL_bool hide;
	SDL_bool removeable;
	SDL_Texture* tex;
} BossBullet;

BossBullet* bossbullets[BOSS_BULLET_AMOUNT_MAX];

extern void AddBoss();

extern void DestroyBoss(Boss** pp_boss);

extern void ShowBoss(GlobalResources* globalresources);

extern void NewBossAttackMode(uint32_t gametime);

extern void MoveBoss(uint32_t gametime);

extern void DecideBossAttackMode(uint32_t gametime);

extern void BossAttack(uint32_t gametime, GlobalResources* globalresources);

extern void DestroyBossBullet(BossBullet** pp_bossbullet);

extern void DestroyAllBossBullets();

extern void AddBossBullet(int x, int y, int bullet_type, GlobalResources* globalresources, uint32_t gametime);

extern void MoveBossBullet(uint32_t gametime, GlobalResources* globalresources);

extern void IfPlayerShotByBoss(GlobalResources* globalresources);

extern void DrawBossBullets(GlobalResources* globalresources);

extern void IfBossShot(int* p_score, int* p_rage, ObjectsParameters parameters, GlobalResources* globalresources);

extern void ShowBossLife(GlobalResources* globalresources);