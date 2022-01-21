#include "objects.h"
#include "skill.h"
#include "globals.h"
#include "boss.h"

BlitedTexture mist = { 0, 0, 30, 30, 1, 8, 0, 41, 37, 20 };

BlitedTexture explosion = { 0, 0, 130, 111, 1, 8, 0, 95, 81, 20 };

BlitedTexture bombexplosion = { 0, 0, 400, 400, 1, 8, 0, 101, 103, 10 };

static Bullet* CreateOneBullet(int x, int y, ObjectsParameters parameters, GlobalParameters parameters2);

static void UpdateList(int List[], int num, int list_length);

static Enemy* CreateOneEnemy(int List[], int num_left, uint32_t gametime, GlobalResources* globalresources);

static void DestroyEnemy(Enemy** pp_enemy);

static Enemy DecideEnemy(int flag, GlobalResources* globalresources);

static void AddBlitedTexture(int x, int y, int w, int h, BlitedTexture texturetype);

static BlitedTexture* CreateBlitedTexture(int x, int y, int w, int h, BlitedTexture texturetype);

static void DestroyBlitedTexture(BlitedTexture** pp_bltttr);

static void PlaySingleBlitedTexture(BlitedTexture** p_bltttr, uint32_t gametime);

static void AddEnemyBullet(int x, int y, double speed);

static EnemyBullet* CreateEnemyBullet(int x, int y, double speed);

static int GetClosetEnemy(int x, int y, int w, int h);

static double GetDistance(int x1, int y1, int x2, int y2);

static Bomb* CreateBomb(int x, int y, struct SkillParameters parameters);

static void DestroyBomb(Bomb** pp_bomb);

void AddBombExplosionTexture(int x, int y, int w, int h);

void InitEnemyMode(Uint32 flags)
{
	int List_1[ENEMY_AMOUNT_MAX] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	int List_2[ENEMY_AMOUNT_MAX] = { 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 };

	int List_3[ENEMY_AMOUNT_MAX] = { 3, 3, 3, 3, 3, 2, 2, 2, 2, 2 };

	int List_4[ENEMY_AMOUNT_MAX] = { 1, 1, 1, 1, 1, 4, 4, 4, 4, 4 };

	int List_5[ENEMY_AMOUNT_MAX] = { 5, 5, 5, 5, 2, 2, 2, 3, 3, 3 };

	int List_6[1] = { 6 };

	switch (flags)
	{
	case INITENEMYMODE_1:
		SDL_memcpy(EnemyList, List_1, sizeof(EnemyList));
		break;
	case INITENEMYMODE_2:
		SDL_memcpy(EnemyList, List_2, sizeof(EnemyList));
		break;
	case INITENEMYMODE_3:
		SDL_memcpy(EnemyList, List_3, sizeof(EnemyList));
		break;
	case INITENEMYMODE_4:
		SDL_memcpy(EnemyList, List_4, sizeof(EnemyList));
		break;
	case INITENEMYMODE_5:
		SDL_memcpy(EnemyList, List_5, sizeof(EnemyList));
		break;
	}
}

void ShootBullet(int x, int y, ObjectsParameters parameters, GlobalParameters parameters2)
{

	int i;
	for (i = 0; i < BULLET_AMOUNT_MAX; i++)
	{
		if (bullets[i] == NULL)
		{
			bullets[i] = CreateOneBullet(x, y, parameters, parameters2);
			break;
		}
	}
}

Bullet* CreateOneBullet(int x, int y, ObjectsParameters parameters, GlobalParameters parameters2)
{
	Bullet* blt = SDL_malloc(sizeof(Bullet));
	blt->lx = x + parameters2.playerW / 2 - parameters.bulletW / 2;
	blt->ly = y - parameters.bulletH;
	blt->x = (int)blt->lx;
	blt->y = (int)blt->ly;

	blt->speed = 15;
	blt->speed_x = 0;
	blt->speed_y = -15;

	blt->acc = 10;

	blt->w = parameters.bulletW;
	blt->h = parameters.bulletH;
	blt->angle = 0l;
	if (trackmode == SDL_TRUE)
		blt->is_tracking = SDL_TRUE;
	else
		blt->is_tracking = SDL_FALSE;
	return blt;
}

void DestroyBullet(Bullet** pp_bullet)
{
	SDL_free(*pp_bullet);
	*pp_bullet = NULL;
}

void DestroyAllBullets(Bullet* bullets[], int bullet_amount)
{
	int i;
	for (i = 0; i < bullet_amount; i++)
	{
		if (bullets[i] != NULL)
		{
			DestroyBullet(&bullets[i]);
		}
	}
}

void DrawBullet(SDL_Renderer* renderer, SDL_Texture* bulletTexture, ObjectsParameters parameters)
{

	int i;
	for (i = 0; i < BULLET_AMOUNT_MAX; i++)
	{
		if (bullets[i] != NULL)
		{
			SDL_Rect box_bullet = { bullets[i]->x , bullets[i]->y , parameters.bulletW, parameters.bulletH };
			SDL_RenderCopyEx(renderer, bulletTexture, NULL, &box_bullet, bullets[i]->angle, NULL, SDL_FLIP_NONE);
		}
	}
}

void MoveBullet(ObjectsParameters parameters)
{
	int i;
	for (i = 0; i < BULLET_AMOUNT_MAX; i++)
	{
		if (bullets[i] != NULL)
		{
			if (bullets[i]->is_tracking == SDL_FALSE)
			{
				bullets[i]->ly += bullets[i]->speed_y;
				bullets[i]->x = (int)bullets[i]->lx;
				bullets[i]->y = (int)bullets[i]->ly;
			}
			else if (bullets[i]->is_tracking == SDL_TRUE)
			{
				bullets[i]->lx += bullets[i]->speed_x;
				bullets[i]->ly += bullets[i]->speed_y;
				bullets[i]->x = (int)bullets[i]->lx;
				bullets[i]->y = (int)bullets[i]->ly;
			}
			if (bullets[i]->y <= -parameters.bulletH)
			{
				DestroyBullet(&bullets[i]);
			}
		}
	}
}

void UpdateList(int List[], int num, int list_length)
{
	int i;
	for (i = num; i < list_length - 2; i++)
	{
		List[i] = List[i + 1];
	}
}

void AddEnemy(int List[], int num_left, uint32_t gametime, GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < ENEMY_AMOUNT_MAX; i++)
	{
		if (enemies[i] == NULL)
		{
			enemies[i] = CreateOneEnemy(List, num_left, gametime, globalresources);
			break;
		}
	}
}

Enemy* CreateOneEnemy(int List[], int num_left, uint32_t gametime, GlobalResources* globalresources)
{
	int tmp = rand() % num_left;
	Enemy ene = DecideEnemy(List[tmp], globalresources);

	UpdateList(List, tmp, num_left);

	Enemy* p_enemy = SDL_malloc(sizeof(Enemy));
	SDL_memcpy(p_enemy, &ene, sizeof(struct Enemy));
	p_enemy->lx = rand() % (windowW - p_enemy->w);
	p_enemy->ly = -p_enemy->h;
	p_enemy->speed = p_enemy->bsc_speed * (1 + p_enemy->ext_speed * ((double)(rand() % 201) - 101) / 100l);
	p_enemy->born_time = gametime;
	if (p_enemy->ID == 5)
	{
		p_enemy->A = 65 + rand() % 25;
		p_enemy->cycle = 2000 + rand() % 1500;
	}

	return p_enemy;
}

void DestroyEnemy(Enemy** pp_enemy)
{

	SDL_free(*pp_enemy);
	*pp_enemy = NULL;
}

void DestroyAllEnemies(Enemy* enemies[], int enemy_amount)
{
	int i;
	for (i = 0; i < enemy_amount; i++)
	{
		if (enemies[i] != NULL)
		{
			DestroyEnemy(&enemies[i]);
		}
	}
}

Enemy DecideEnemy(int flag, GlobalResources* globalresources)
{
	Enemy enemy_kind1 = { 1, 1, 3, 0, 0, 0, 0, 80, 70, 1, 0, 5l, 0.4, 0, 0, 0, SDL_FALSE };

	Enemy enemy_kind2 = { 2, 1, 3, 0, 0, 0, 0, 65, 50, 1, 0, 5l, 0.3l, 0, 0, 0, SDL_FALSE };

	Enemy enemy_kind3 = { 3, 1, 3, 0, 0, 0, 0, 40, 30, 1, 0, 2l, 0.5l, 0, 0, 0, SDL_TRUE, 0 };

	Enemy enemy_kind4 = { 4, 2, 3, 0, 0, 0, 0, 75, 25, 2, 0, 7l, 0.1l, 0, 0, 0, SDL_FALSE };

	Enemy enemy_kind5 = { 5, 1, 3, 0, 0, 0, 0, 65, 33, 1, 0, 7l, 0.3l, 0, 0, 0, SDL_FALSE };

	enemy_kind1.tex = globalresources->enemyTexture1;
	enemy_kind2.tex = globalresources->enemyTexture2;
	enemy_kind3.tex = globalresources->enemyTexture3;
	enemy_kind4.tex = globalresources->enemyTexture4;
	enemy_kind5.tex = globalresources->enemyTexture5;

	switch (flag)
	{
	case ENEMY_KIND_1:
		return enemy_kind1;
	case ENEMY_KIND_2:
		return enemy_kind2;
	case ENEMY_KIND_3:
		return enemy_kind3;
	case ENEMY_KIND_4:
		return enemy_kind4;
	case ENEMY_KIND_5:
		return enemy_kind5;
	}
}

void DrawEnemy(GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < ENEMY_AMOUNT_MAX; i++)
	{
		if (enemies[i] != NULL)
		{
			SDL_Rect box_enemy = { enemies[i]->x , enemies[i]->y , enemies[i]->w , enemies[i]->h };
			
			if (enemies[i]->ID == 5)
			{
				SDL_RenderCopyEx(globalresources->rend, enemies[i]->tex, NULL, &box_enemy, enemies[i]->angle, NULL, SDL_FLIP_NONE);
			}
			else
			{
				SDL_RenderCopy(globalresources->rend, enemies[i]->tex, NULL, &box_enemy);
			}
		}
	}
}

void MoveEnemy(GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < ENEMY_AMOUNT_MAX; i++)
	{
		if (enemies[i] != NULL)
		{
			enemies[i]->ly += enemies[i]->speed;
			enemies[i]->x = (int)enemies[i]->lx + enemies[i]->delX;
			enemies[i]->y = (int)enemies[i]->ly;
			if (enemies[i]->y >= windowH)
			{
				DestroyEnemy(&enemies[i]);
				playerlife--;
			}
		}
		if (enemies[i] != NULL)
		{
			if(enemies[i]->x <= globalresources->box_player->x + globalresources->box_player->w && enemies[i]->x + enemies[i]->w >= globalresources->box_player->x
				&& enemies[i]->y <= globalresources->box_player->y + globalresources->box_player->h && enemies[i]->y + enemies[i]->h >= globalresources->box_player->y + globalresources->box_player->h * 0.8)
			{
				Mix_PlayChannel(-1, globalresources->explosionSound, 0);

				int x, y, w, h;
				x = enemies[i]->x;
				y = enemies[i]->y;
				w = enemies[i]->w;
				h = enemies[i]->h;

				int change_w = explosion.w * w / 75;
				int change_h = explosion.h * w / 75;

				DestroyEnemy(&enemies[i]);
				playerlife--;
				AddExplosionTexture(x + w / 2 - change_w / 2, y + h / 2 - change_h / 2, change_w, change_h);
			}
		}
	}
}

void ChangeEnemyRout(uint32_t gametime)
{
	int i;
	for (i = 0; i < ENEMY_AMOUNT_MAX; i++)
	{
		if (enemies[i] != NULL)
		{
			if (enemies[i]->ID == 5)
			{
				enemies[i]->last_delX = enemies[i]->delX;
				enemies[i]->delX = SinVar(gametime, enemies[i]->born_time, 0, enemies[i]->A, enemies[i]->cycle, 0);
				enemies[i]->angle = -SDL_atan2(enemies[i]->delX - enemies[i]->last_delX, enemies[i]->speed) / PI * 180;
			}
		}
	}
}

void IfShot(int* p_score, int* p_rage, ObjectsParameters parameters, GlobalResources* globalresources)
{
	int i, j;
	for (i = 0; i < ENEMY_AMOUNT_MAX; i++)
	{
		for (j = 0; j < BULLET_AMOUNT_MAX; j++)
		{

			if (enemies[i] != NULL && bullets[j] != NULL &&
				enemies[i]->x <= bullets[j]->x + parameters.bulletW && enemies[i]->x + enemies[i]->w >= bullets[j]->x
				&& enemies[i]->y <= bullets[j]->y + parameters.bulletH && enemies[i]->y + enemies[i]->h >= bullets[j]->y)
			{
				Mix_PlayChannel(-1, globalresources->explosionSound, 0);

				if (rage < RAGE_MAX)
				{
					*p_rage += enemies[i]->rage;
				}
				if (rage >= RAGE_MAX)
				{
					*p_rage = RAGE_MAX;
				}
				int x, y, w, h;
				x = enemies[i]->x;
				y = enemies[i]->y;
				w = enemies[i]->w;
				h = enemies[i]->h;

				int change_w = explosion.w * w / 75;
				int change_h = explosion.h * w / 75;

				DestroyBullet(&bullets[j]);

				enemies[i]->hp--;
				if (enemies[i]->hp == 0)
				{
					*p_score += enemies[i]->score;
					DestroyEnemy(&enemies[i]);
					AddExplosionTexture(x + w / 2 - change_w / 2, y + h / 2 - change_h / 2, change_w, change_h);
				}
			}
		}
	}
}

void InitBlitedTexture(GlobalResources* globalresources)
{
	mist.tex = globalresources->mistTexture;
	mist.rect.x = 0;
	mist.rect.y = 0;
	mist.rect.w = mist.frame_width;
	mist.rect.h = mist.frame_height;

	explosion.tex = globalresources->explosionTexture;
	explosion.rect.x = 0;
	explosion.rect.y = 0;
	explosion.rect.w = explosion.frame_width;
	explosion.rect.h = explosion.frame_height;

	bombexplosion.tex = globalresources->bombexplosionTexture;
	bombexplosion.rect.x = 0;
	bombexplosion.rect.y = 0;
	bombexplosion.rect.w = bombexplosion.frame_width;
	bombexplosion.rect.h = bombexplosion.frame_height;
}

void AddBlitedTexture(int x, int y, int w, int h, BlitedTexture texturetype)
{
	int i;
	for (i = 0; i < BLITEDTEXTURE_AMOUNT_MAX; i++)
	{
		if (blitedtextures[i] == NULL)
		{
			blitedtextures[i] = CreateBlitedTexture(x, y, w, h, texturetype);
			break;
		}
	}
}

BlitedTexture* CreateBlitedTexture(int x, int y, int w, int h, BlitedTexture texturetype)
{
	BlitedTexture* bltttr = SDL_malloc(sizeof(BlitedTexture));
	SDL_memcpy(bltttr, &texturetype, sizeof(BlitedTexture));
	bltttr->x = x;
	bltttr->y = y;
	bltttr->w = w;
	bltttr->h = h;
	return bltttr;
}

void DestroyBlitedTexture(BlitedTexture** pp_bltttr)
{
	SDL_free(*pp_bltttr);
	*pp_bltttr = NULL;
}

void DestroyAllBlitedTexture(BlitedTexture* blitedtextures[], int blitedtexture_amount)
{
	int i;
	for (i = 0; i < blitedtexture_amount; i++)
	{
		if (blitedtextures[i] != NULL)
		{
			DestroyBlitedTexture(&blitedtextures[i]);
		}
	}
}

void PlaySingleBlitedTexture(BlitedTexture** pp_bltttr, uint32_t gametime)
{
	if (gametime - (*pp_bltttr)->last_refresh_time >= 1000 / (*pp_bltttr)->framerate)
	{
		if ((*pp_bltttr)->frame < (*pp_bltttr)->totalframe)
		{
			(*pp_bltttr)->frame += 1;
			(*pp_bltttr)->rect.x = ((*pp_bltttr)->frame - 1) * (*pp_bltttr)->rect.w;
			(*pp_bltttr)->last_refresh_time = gametime;
		}
		else
		{
			DestroyBlitedTexture(pp_bltttr);
		}
	}
}

void PlayAllBlitedTexture(uint32_t gametime)
{
	int i;
	for (i = 0; i < BLITEDTEXTURE_AMOUNT_MAX; i++)
	{
		if (blitedtextures[i] != NULL)
		{
			PlaySingleBlitedTexture(&blitedtextures[i], gametime);
		}
	}
}

void ShowAllBlitedTexture(GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < BLITEDTEXTURE_AMOUNT_MAX; i++)
	{
		if (blitedtextures[i] != NULL)
		{
			SDL_Rect dst_rect = { blitedtextures[i]->x, blitedtextures[i]->y, blitedtextures[i]->w, blitedtextures[i]->h };
			SDL_RenderCopy(globalresources->rend, blitedtextures[i]->tex, &(blitedtextures[i]->rect), &dst_rect);
		}
	}
}

void AddEnemyMist(int framerate, uint32_t gametime, uint32_t* p_mistT)
{
	if (gametime - *p_mistT >= 1000 / framerate)
	{
		int i;
		for (i = 0; i < ENEMY_AMOUNT_MAX; i++)
		{
			if (enemies[i] != NULL)
			{
				if (enemies[i]->ID != 5)
				{
					AddBlitedTexture(enemies[i]->x + enemies[i]->w / 2 - mist.w / 2, enemies[i]->y - 20, mist.w, mist.h, mist);
				}
			}
		}
		*p_mistT = gametime;
	}
}

void AddExplosionTexture(int x, int y, int w, int h)
{
	AddBlitedTexture(x, y, w, h, explosion);
}

void AggresiveEnemyAttack(uint32_t gametime, int time, ObjectsParameters parameters, GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < ENEMY_AMOUNT_MAX; i++)
	{
		if (enemies[i] != NULL)
		{
			if (enemies[i]->is_aggresive == SDL_TRUE)
			{
				if (gametime - enemies[i]->last_attack_time >= time)
				{
					Mix_PlayChannel(-1, globalresources->shootSound, 0);
					AddEnemyBullet(enemies[i]->x + enemies[i]->w / 2 - parameters.enemybulletW / 2,
						enemies[i]->y + enemies[i]->h - parameters.enemybulletH / 2,
						10);
					enemies[i]->last_attack_time = gametime;
				}
			}
		}
	}
}

void MoveEnemyBullet()
{
	int i;
	for (i = 0; i < ENEMYBULLET_AMOUNT_MAX; i++)
	{
		if (enemybullets[i] != NULL)
		{
			enemybullets[i]->ly += enemybullets[i]->speed;
			enemybullets[i]->x = (int)enemybullets[i]->lx;
			enemybullets[i]->y = (int)enemybullets[i]->ly;
		}
		if (enemybullets[i] != NULL)
		{
			if (enemybullets[i]->y >= windowH)
			{
				DestroyEnemyBullet(&enemybullets[i]);
			}
		}
	}
}

void DrawEnemyBullets(ObjectsParameters parameters, GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < ENEMYBULLET_AMOUNT_MAX; i++)
	{
		if (enemybullets[i] != NULL)
		{
			SDL_Rect eneblt_rect = { enemybullets[i]->x, enemybullets[i]->y, parameters.enemybulletW, parameters.enemybulletH };
			SDL_RenderCopy(globalresources->rend, globalresources->enemybulletTexture, NULL, &eneblt_rect);
		}
	}
}

void IfPlayerShot(ObjectsParameters parameters, GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < ENEMYBULLET_AMOUNT_MAX; i++)
	{
		if (enemybullets[i] != NULL)
		{
			if (enemybullets[i]->x <= globalresources->box_player->x + globalresources->box_player->w - globalresources->box_player->w * 0.3
				&& enemybullets[i]->x + parameters.enemybulletW >= globalresources->box_player->x + globalresources->box_player->w * 0.3
				&& enemybullets[i]->y <= globalresources->box_player->y + globalresources->box_player->h
				&& enemybullets[i]->y + parameters.enemybulletH >= globalresources->box_player->y + globalresources->box_player->h * 0.8)
			{
				playerlife--;
				DestroyEnemyBullet(&enemybullets[i]);
			}
		}
	}
}

void AddEnemyBullet(int x, int y, double speed)
{
	int i;
	for (i = 0; i < ENEMYBULLET_AMOUNT_MAX; i++)
	{
		if (enemybullets[i] == NULL)
		{
			enemybullets[i] = CreateEnemyBullet(x, y, speed);
			break;
		}
	}
}

EnemyBullet* CreateEnemyBullet(int x, int y, double speed)
{
	EnemyBullet* eneblt = SDL_malloc(sizeof(EnemyBullet));
	eneblt->lx = x;
	eneblt->ly = y;
	eneblt->speed = speed;
	return eneblt;
}

void DestroyEnemyBullet(EnemyBullet** pp_eneblt)
{
	SDL_free(*pp_eneblt);
	*pp_eneblt = NULL;
}

void DestroyAllEnemyBullets(EnemyBullet* enemybullets[], int blitedtexture_amount)
{
	int i;
	for (i = 0; i < blitedtexture_amount; i++)
	{
		if (enemybullets[i] != NULL)
		{
			DestroyEnemyBullet(&enemybullets[i]);
		}
	}
}

int GetClosetEnemy(int x, int y, int w, int h)
{
	int tmp;
	int i;
	double distance;
	double min = -1;
	int ret = -1;
	for (i = 0; i < ENEMY_AMOUNT_MAX; i++)
	{
		if (enemies[i] != NULL)
		{
			distance = GetDistance(x + w / 2, y + h / 2,
				enemies[i]->x + enemies[i]->w / 2, enemies[i]->y + enemies[i]->h / 2);
			if (min == -1)
			{
				min = distance;
				ret = i;
			}
			else
			{
				if (min > distance)
				{
					min = distance;
					ret = i;
				}
			}
		}
	}
	if (boss != NULL)
	{
		distance = GetDistance(x + w / 2, y + h / 2, boss->x + boss->w / 2, boss->y + boss->h / 2);
		if (min > distance)
		{
			ret = ENEMY_AMOUNT_MAX;
		}
	}
	return ret;
}

double GetDistance(int x1, int y1, int x2, int y2)
{
	return sqrt(((double)x1 - x2) * ((double)x1 - x2) + ((double)y1 - y2) * ((double)y1 - y2));
}

void BulletTracking(int framerate, uint32_t gametime, uint32_t* p_trackT)
{
	if (gametime - *p_trackT >= 1000 / framerate || *p_trackT == 0)
	{
		int i;
		for (i = 0; i < BULLET_AMOUNT_MAX; i++)
		{
			if (bullets[i] != NULL)
			{
				if (bullets[i]->is_tracking == SDL_TRUE)
				{
					if (GetClosetEnemy(bullets[i]->x, bullets[i]->y, bullets[i]->w, bullets[i]->h) != -1)
					{
						int dst = GetClosetEnemy(bullets[i]->x, bullets[i]->y, bullets[i]->w, bullets[i]->h);

						if (dst == ENEMY_AMOUNT_MAX)
						{
							bullets[i]->acc_x = bullets[i]->acc * CalcComponentX(bullets[i]->x + bullets[i]->w / 2, bullets[i]->y + bullets[i]->h / 2,
								boss->x + boss->w / 2, boss->y + boss->h / 2);
							bullets[i]->acc_y = bullets[i]->acc * CalcComponentY(bullets[i]->x + bullets[i]->w / 2, bullets[i]->y + bullets[i]->h / 2,
								boss->x + boss->w / 2, boss->y + boss->h / 2);
						}
						else
						{
							bullets[i]->acc_x = bullets[i]->acc * CalcComponentX(bullets[i]->x + bullets[i]->w / 2, bullets[i]->y + bullets[i]->h / 2,
								enemies[dst]->x + enemies[dst]->w / 2, enemies[dst]->y + enemies[dst]->h / 2);
							bullets[i]->acc_y = bullets[i]->acc * CalcComponentY(bullets[i]->x + bullets[i]->w / 2, bullets[i]->y + bullets[i]->h / 2,
								enemies[dst]->x + enemies[dst]->w / 2, enemies[dst]->y + enemies[dst]->h / 2);
						}
						bullets[i]->speed_x += bullets[i]->acc_x;
						bullets[i]->speed_y += bullets[i]->acc_y;

						double vx = bullets[i]->speed_x;
						double vy = bullets[i]->speed_y;

						bullets[i]->speed_x = bullets[i]->speed * CalcComponentX(0, 0, vx, vy);
						bullets[i]->speed_y = bullets[i]->speed * CalcComponentY(0, 0, vx, vy);

						bullets[i]->angle = SDL_atan2(bullets[i]->speed_x, -bullets[i]->speed_y) * 180 / PI;
					}
					else
					{


						bullets[i]->acc_x = 0;
						bullets[i]->acc_y = -bullets[i]->acc;

						if (abs(bullets[i]->speed_x) <= 1e-4 && abs(bullets[i]->speed_y - bullets[i]->speed) <= 1e-4)
						{
							if (bullets[i]->x + bullets[i]->w / 2 <= windowW / 2)
							{
								bullets[i]->acc_x = 15;
								bullets[i]->acc_y = 0;
							}
							else if (bullets[i]->x + bullets[i]->w > windowW / 2)
							{
								bullets[i]->acc_x = -15;
								bullets[i]->acc_y = 0;
							}
						}

						bullets[i]->speed_x += bullets[i]->acc_x;
						bullets[i]->speed_y += bullets[i]->acc_y;

						double vx = bullets[i]->speed_x;
						double vy = bullets[i]->speed_y;

						bullets[i]->speed_x = bullets[i]->speed * CalcComponentX(0, 0, vx, vy);
						bullets[i]->speed_y = bullets[i]->speed * CalcComponentY(0, 0, vx, vy);

						bullets[i]->angle = SDL_atan2(bullets[i]->speed_x, -bullets[i]->speed_y) * 180 / PI;
					}
				}
			}
		}
		*p_trackT = gametime;
	}

}

void AddBomb(int x, int y, SkillParameters parameters)
{

	int i;
	for (i = 0; i < BOMB_AMOUNT_MAX; i++)
	{
		if (bombs[i] == NULL)
		{
			bombs[i] = CreateBomb(x, y, parameters);
			break;
		}
	}
}

Bomb* CreateBomb(int x, int y, SkillParameters parameters)
{
	Bomb* bomb = SDL_malloc(sizeof(Bomb));
	bomb->lx = x;
	bomb->ly = y;
	bomb->x = x;
	bomb->y = y;
	bomb->speed = 25;
	bomb->speed_x = 0;
	bomb->speed_y = -25;
	bomb->w = parameters.bombW;
	bomb->h = parameters.bombH;
	bomb->acc = 8;
	bomb->angle = 0l;
	return bomb;
}

void DestroyBomb(Bomb** pp_bomb)
{
	SDL_free(*pp_bomb);
	*pp_bomb = NULL;
}

void DestroyAllBombs(Bomb* bombs[], int bomb_amount)
{
	int i;
	for (i = 0; i < bomb_amount; i++)
	{
		if (bombs[i] != NULL)
		{
			DestroyBomb(&bombs[i]);
		}
	}
}

void DrawBomb(GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < BOMB_AMOUNT_MAX; i++)
	{
		if (bombs[i] != NULL)
		{
			SDL_Rect bomb_rect = { bombs[i]->x, bombs[i]->y, bombs[i]->w, bombs[i]->h };
			SDL_RenderCopyEx(globalresources->rend, globalresources->bombTexture, NULL, &bomb_rect, bombs[i]->angle, NULL, SDL_FLIP_NONE);
		}
	}
}

void MoveBomb()
{
	int i;
	for (i = 0; i < BOMB_AMOUNT_MAX; i++)
	{
		if (bombs[i] != NULL)
		{
			int dst_x = windowW / 2;
			int dst_y = windowH / 2;

			bombs[i]->acc_x = bombs[i]->acc * CalcComponentX(bombs[i]->x + bombs[i]->w / 2, bombs[i]->y + bombs[i]->h / 2,
				dst_x, dst_y);
			bombs[i]->acc_y = bombs[i]->acc * CalcComponentY(bombs[i]->x + bombs[i]->w / 2, bombs[i]->y + bombs[i]->h / 2,
				dst_x, dst_y);

			bombs[i]->speed_x += bombs[i]->acc_x;
			bombs[i]->speed_y += bombs[i]->acc_y;

			double vx = bombs[i]->speed_x;
			double vy = bombs[i]->speed_y;

			bombs[i]->speed_x = bombs[i]->speed * CalcComponentX(0, 0, vx, vy);
			bombs[i]->speed_y = bombs[i]->speed * CalcComponentY(0, 0, vx, vy);

			bombs[i]->angle = SDL_atan2(bombs[i]->speed_x, -bombs[i]->speed_y) * 180 / PI;

			bombs[i]->lx += bombs[i]->speed_x;
			bombs[i]->ly += bombs[i]->speed_y;
			bombs[i]->x = (int)bombs[i]->lx;
			bombs[i]->y = (int)bombs[i]->ly;
		}
	}
}

void AddBombExplosionTexture(int x, int y, int w, int h)
{
	AddBlitedTexture(x, y, w, h, bombexplosion);
}

void BombExplode()
{
	int i;
	for (i = 0; i < BOMB_AMOUNT_MAX; i++)
	{
		if (bombs[i] != NULL)
		{
			if (bombs[i]->x < windowW / 2 && bombs[i]->x + bombs[i]->w > windowW / 2
				&& bombs[i]->y < windowH / 2 && bombs[i]->y + bombs[i]->h > windowH / 2)
			{
				DestroyBomb(&bombs[i]);

				AddBombExplosionTexture(windowW / 2 - bombexplosion.w / 2, windowH / 2 - bombexplosion.h + 65,
					bombexplosion.w, bombexplosion.h);
				int j;
				for (j = 0; j < ENEMY_AMOUNT_MAX; j++)
				{
					if (enemies[j] != NULL)
					{
						enemies[j]->hp -= 100;
						if (enemies[j]->hp <= 0)
						{
							int x, y, w, h;
							x = enemies[j]->x;
							y = enemies[j]->y;
							w = enemies[j]->w;
							h = enemies[j]->h;
							DestroyEnemy(&enemies[j]);


							int change_w = explosion.w * w / 75;
							int change_h = explosion.h * w / 75;
							AddExplosionTexture(x + w / 2 - change_w / 2, y + h / 2 - change_h / 2, change_w, change_h);
						}
					}
				}
				if (boss != NULL)
				{
					boss->life -= 50;
				}
			}
		}
	}
}

void AddBombMist(int framerate, uint32_t gametime, uint32_t* p_bombmistT)
{
	if (gametime - *p_bombmistT >= 1000 / framerate)
	{
		int i;
		for (i = 0; i < BOMB_AMOUNT_MAX; i++)
		{
			if (bombs[i] != NULL)
			{
				int d = bombs[i]->h / 2 + 12;
				AddBlitedTexture(bombs[i]->x + bombs[i]->w / 2 - d * SDL_sin(bombs[i]->angle / 180 * PI) - mist.w / 2,
					bombs[i]->y + bombs[i]->h / 2 + d * SDL_cos(bombs[i]->angle / 180 * PI) - mist.h / 2, mist.w, mist.h, mist);
			}
		}
		*p_bombmistT = gametime;
	}
}