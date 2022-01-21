#include "boss.h"

static Boss* CreateBoss();

static void BossAutoMove();

static void DestroyBossAttackMode(AttackMode** pp_atkmode);

static AttackMode* CreateBossAttackMode(int mode, uint32_t last_modetime, int duration, uint32_t last_attacktime, int attack_interval, SDL_bool coexist);

static void AddBossAttackMode(int mode, uint32_t last_modetime, int duration, uint32_t last_attacktime, int attack_interval, SDL_bool coexist);

BossBullet* CreateBossBullet(int x, int y, int w, int h, double speed, double speed_x, double speed_y, int bullet_type, SDL_bool tracking, uint32_t gametime);

static BossBullet* CreateBossBullet(int x, int y, int w, int h, double speed, double speed_x, double speed_y, int bullet_type, SDL_bool tracking, uint32_t gametime);

static void AddBossBullet(int x, int y, int bullet_type, GlobalResources* globalresources, uint32_t gametime);

Boss* CreateBoss()
{
	Boss* boss = SDL_malloc(sizeof(Boss));
	
	boss->w = 200;
	boss->h = 200;
	boss->lx = windowW / 2 - boss->w / 2;
	boss->ly = 50;
	boss->x = (int)boss->lx;
	boss->y = (int)boss->ly;
	boss->speed_x = 0;
	boss->speed_y = 0;
	boss->is_automove = SDL_TRUE;

	boss->life = 200;

	boss->attackmode[0] = SDL_malloc(sizeof(AttackMode));
	boss->attackmode[0]->mode = BOSS_ATTACKMODE_0;
	boss->attackmode[0]->duration = 5000;

	boss->attackmode[0]->last_attacktime = 0;
	boss->attackmode[0]->attack_interval = 1000;
	boss->attackmode[0]->coexist = SDL_FALSE;

	int i;
	for (i = 1; i < BOSS_ATTACKMODE_AMOUNT_MAX; i++)
	{
		boss->attackmode[i] = NULL;
	}

	return boss;
}

void AddBoss()
{
	if (boss == NULL)
	{
		boss = CreateBoss();
	}
}

void DestroyBoss(Boss** pp_boss)
{
	if (*pp_boss != NULL)
	{
		int i;
		for (i = 0; i < BOSS_ATTACKMODE_AMOUNT_MAX; i++)
		{
			if ((*pp_boss)->attackmode[i] != NULL)
			{
				SDL_free((*pp_boss)->attackmode[i]);
				(*pp_boss)->attackmode[i] = NULL;
			}
		}
		SDL_free(*pp_boss);
		*pp_boss = NULL;
	}
}

void BossAutoMove()
{
	if (boss != NULL)
	{
		double speed = (rand() % 150 + 100) / (double)100;
		enum move_direction
		{
			move_left,
			move_right,
			no_move
		};
		int tmp = rand() % 3;
		if (tmp == move_left)
		{
			boss->speed_x = -speed;
		}
		else 
		if (tmp == move_right)
		{
			boss->speed_x = speed;
		}
		else if (tmp == no_move)
		{
			boss->speed_x = 0;
		}
		boss->moveduration = rand() % 2000 + 1000;
	}
}

void MoveBoss(uint32_t gametime)
{
	if (boss != NULL)
	{
		if (boss->is_automove == SDL_TRUE)
		{
			if (gametime - boss->last_movetime >= boss->moveduration)
			{
				BossAutoMove();
				boss->last_movetime = gametime;
			}
			if (boss->lx < 0)
			{
				BossAutoMove();
				boss->last_movetime = gametime;
				boss->lx = 0;
			}
			if (boss->lx + boss->w > windowW)
			{
				BossAutoMove();
				boss->last_movetime = gametime;
				boss->lx = windowW - boss->w;
			}
			boss->lx += boss->speed_x;
		}
		if (boss->life <= 100 && boss->life > 50)
		{
			boss->w = 125;
			boss->h = 125;
		}
		else if (boss->life <= 50)
		{
			boss->w = 75;
			boss->h = 75;
		}
	}
}

void ShowBoss(GlobalResources* globalresources)
{
	if (boss != NULL)
	{
		boss->x = (int)boss->lx;
		boss->y = (int)boss->ly;
		SDL_Rect bossRect = { boss->x, boss->y, boss->w, boss->h };
		SDL_RenderCopy(globalresources->rend, globalresources->bossTexture, NULL, &bossRect);
	}
}

void DestroyBossAttackMode(AttackMode** pp_atkmode)
{
	SDL_free(*pp_atkmode);
	*pp_atkmode = NULL;
}

AttackMode* CreateBossAttackMode(int mode, uint32_t last_modetime, int duration, uint32_t last_attacktime, int attack_interval, SDL_bool coexist)
{
	AttackMode* p_attackmode = SDL_malloc(sizeof(AttackMode));
	p_attackmode->mode = mode;
	p_attackmode->last_modetime = last_modetime;
	p_attackmode->duration = duration;
	p_attackmode->coexist = coexist;
	p_attackmode->attack_interval = attack_interval;
	return p_attackmode;
}

void AddBossAttackMode(int mode, uint32_t last_modetime, int duration, uint32_t last_attacktime, int attack_interval, SDL_bool coexist)
{
	int i;
	SDL_bool no_same = SDL_TRUE;
	for (i = 0; i < BOSS_ATTACKMODE_AMOUNT_MAX; i++)
	{
		if (boss->attackmode[i] != NULL)
		{
			if (boss->attackmode[i]->mode == mode)
			{
				no_same = SDL_FALSE;
				break;
			}
		}
	}
	if (no_same)
	{
		for (i = 0; i < BOSS_ATTACKMODE_AMOUNT_MAX; i++)
		{
			if (boss->attackmode[i] == NULL)
			{
				boss->attackmode[i] = CreateBossAttackMode(mode, last_modetime, duration, last_attacktime, attack_interval, coexist);
				break;
			}
		}
	}
}

void NewBossAttackMode(uint32_t gametime)
{
	if (boss != NULL)
	{
		if (gametime - boss->attackmode[0]->last_modetime >= boss->attackmode[0]->duration)
		{
			DecideBossAttackMode(gametime);
			boss->attackmode[0]->last_modetime = gametime;
		}
		int i;
		for (i = 1; i < BOSS_ATTACKMODE_AMOUNT_MAX; i++)
		{
			if (boss->attackmode[i] != NULL)
			{
				if (gametime - boss->attackmode[i]->last_modetime >= boss->attackmode[i]->duration)
				{

					int j;
					for (j = 0; j < BOSS_BULLET_AMOUNT_MAX; j++)
					{
						if (bossbullets[j] != NULL)
						{
							if (bossbullets[j]->bullet_type == boss->attackmode[i]->mode)
							{
								DestroyBossBullet(&bossbullets[j]);
							}
						}
					}
					DestroyBossAttackMode(&boss->attackmode[i]);
				}
			}
			
		}
	}
}

void DecideBossAttackMode(uint32_t gametime)
{
	if (boss != NULL)
	{
		int tmp = rand() % BOSS_ATTACKMODE_AMOUNT_MAX;
		if (tmp == BOSS_ATTACKMODE_0)
		{
			boss->attackmode[0]->mode = BOSS_ATTACKMODE_0;
			boss->attackmode[0]->duration = 5000;

			boss->attackmode[0]->last_attacktime = 0;
			boss->attackmode[0]->attack_interval = 1000;
			boss->attackmode[0]->coexist = SDL_FALSE;
		}
		else if (tmp == BOSS_ATTACKMODE_1)
		{
			AddBossAttackMode(BOSS_ATTACKMODE_1, gametime, 5000, 0, 6000, SDL_TRUE);
		}
		else if (tmp == BOSS_ATTACKMODE_2)
		{
			boss->attackmode[0]->mode = BOSS_ATTACKMODE_2;
			boss->attackmode[0]->duration = 3000;

			boss->attackmode[0]->last_attacktime = 0;
			boss->attackmode[0]->attack_interval = 900;
			boss->attackmode[0]->coexist = SDL_FALSE;
		}
	}
}

void BossAttack(uint32_t gametime, GlobalResources* globalresources)
{
	if (boss != NULL)
	{
		int i;
		for (i = 0; i < BOSS_ATTACKMODE_AMOUNT_MAX; i++)
		{
			if (boss->attackmode[i] != NULL)
			{
				if (boss->attackmode[i]->mode == BOSS_ATTACKMODE_0)
				{
					if (gametime - boss->attackmode[i]->last_attacktime >= boss->attackmode[i]->attack_interval)
					{
						Mix_PlayChannel(-1, globalresources->shootSound, 0);
						AddBossBullet(boss->x, boss->y, BOSS_BULLET_TYPE_0, globalresources, gametime);
						boss->attackmode[i]->last_attacktime = gametime;
					}
				}
				if (boss->attackmode[i]->mode == BOSS_ATTACKMODE_1)
				{
					if (gametime - boss->attackmode[i]->last_attacktime >= boss->attackmode[i]->attack_interval)
					{
						Mix_PlayChannel(-1, globalresources->laserSound, 0);
						AddBossBullet(boss->x, boss->y, BOSS_BULLET_TYPE_1, globalresources, gametime);
						boss->attackmode[i]->last_attacktime = gametime;
					}
				}
				if (boss->attackmode[i]->mode == BOSS_ATTACKMODE_2)
				{
					if (gametime - boss->attackmode[i]->last_attacktime >= boss->attackmode[i]->attack_interval)
					{
						Mix_PlayChannel(-1, globalresources->shootSound, 0);
						AddBossBullet(boss->x, boss->y, BOSS_BULLET_TYPE_2, globalresources, gametime);
						boss->attackmode[i]->last_attacktime = gametime;
					}
				}
			}
		}
	}
}

BossBullet* CreateBossBullet(int x, int y, int w, int h, double speed, double speed_x, double speed_y, int bullet_type, SDL_bool tracking, uint32_t gametime)
{
	if (boss != NULL)
	{
		BossBullet* p_bossbullet = SDL_malloc(sizeof(BossBullet));
		p_bossbullet->bullet_type = bullet_type;
		p_bossbullet->createtime = gametime;
		p_bossbullet->w = w;
		p_bossbullet->h = h;
		p_bossbullet->lx = x + boss->w / 2 - p_bossbullet->w / 2;
		p_bossbullet->ly = y + boss->h - p_bossbullet->h / 2;
		p_bossbullet->x = x - p_bossbullet->w / 2;
		p_bossbullet->y = y + boss->h - p_bossbullet->h / 2;
		p_bossbullet->speed = speed;
		p_bossbullet->speed_x = speed_x;
		p_bossbullet->speed_y = speed_y;
		p_bossbullet->tracking = tracking;

		return p_bossbullet;
	}
	
}

void DestroyBossBullet(BossBullet** pp_bossbullet)
{
	SDL_free(*pp_bossbullet);
	*pp_bossbullet = NULL;
}

void DestroyAllBossBullets()
{
	int i;
	for (i = 0; i < BOSS_BULLET_AMOUNT_MAX; i++)
	{
		if (bossbullets[i] != NULL)
		{
			DestroyBossBullet(&bossbullets[i]);
		}
	}
}

void AddBossBullet(int x, int y, int bullet_type, GlobalResources* globalresources, uint32_t gametime)
{
	int i;
	for (i = 0; i < BOSS_BULLET_AMOUNT_MAX; i++)
	{
		if (bossbullets[i] == NULL)
		{
			if (bullet_type == BOSS_BULLET_TYPE_0)
			{
				int j;
				double tmpangle = PI / 4;
				for (j = 0; j < 3; j++)
				{
					bossbullets[i + j] = CreateBossBullet(x, y, 10, 37, 9, 9 * SDL_cos(tmpangle), 9 * SDL_sin(tmpangle), bullet_type, SDL_TRUE, gametime);
					bossbullets[i + j]->tex = globalresources->bossbullet1Texture;
					tmpangle += PI / 4;
					bossbullets[i + j]->removeable = SDL_TRUE;
				}
				break;
			}
			else if (bullet_type == BOSS_BULLET_TYPE_1)
			{
				bossbullets[i] = CreateBossBullet(x, y + 950 / 2, 55, 950, 0, 0, 0, bullet_type, SDL_FALSE, gametime);
				bossbullets[i]->hide = SDL_FALSE;
				bossbullets[i]->tex = globalresources->bossbullet2Texture;
				bossbullets[i]->removeable = SDL_FALSE;
				bossbullets[i]->angle = 0l;
				break;
			}
			else if (bullet_type == BOSS_BULLET_TYPE_2)
			{
				bossbullets[i] = CreateBossBullet(x, y, 186, 51, 5, 0, 5, bullet_type, SDL_FALSE, gametime);
				bossbullets[i]->tex = globalresources->bossbullet3Texture;
				bossbullets[i]->removeable = SDL_TRUE;
				bossbullets[i]->angle = 0l;
				break;
			}
		}
	}
}

void MoveBossBullet(uint32_t gametime, GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < BOSS_BULLET_AMOUNT_MAX; i++)
	{
		if (bossbullets[i] != NULL)
		{
			if (bossbullets[i]->bullet_type == BOSS_BULLET_TYPE_1)
			{
				if (boss != NULL)
				{
					bossbullets[i]->lx = boss->x + boss->w / 2 - bossbullets[i]->w / 2;
				}
				else
				{
					DestroyBossBullet(&bossbullets[i]);
				}

			}
		}
		if (bossbullets[i] != NULL)
		{
			if (bossbullets[i]->bullet_type == BOSS_BULLET_TYPE_0)
			{
				if (gametime - bossbullets[i]->createtime < 600)
				{
					int src_x = bossbullets[i]->x + bossbullets[i]->w / 2;
					int src_y = bossbullets[i]->y + bossbullets[i]->h / 2;

					int dst_x = globalresources->box_player->x + globalresources->box_player->w / 2;
					int dst_y = globalresources->box_player->y + globalresources->box_player->h / 2;

					double d = sqrt((src_x - dst_x) * (src_x - dst_x) + (src_y - dst_y) * (src_y - dst_y));

					bossbullets[i]->acc = 150 / pow(4, (d / 100));

					bossbullets[i]->acc_x = bossbullets[i]->acc * CalcComponentX(src_x, src_y, dst_x, dst_y);
					bossbullets[i]->acc_y = bossbullets[i]->acc * CalcComponentY(src_x, src_y, dst_x, dst_y);

					bossbullets[i]->speed_x += bossbullets[i]->acc_x;
					bossbullets[i]->speed_y += bossbullets[i]->acc_y;

					double vx = bossbullets[i]->speed_x;
					double vy = bossbullets[i]->speed_y;

					bossbullets[i]->speed_x = bossbullets[i]->speed * CalcComponentX(0, 0, vx, vy);
					bossbullets[i]->speed_y = bossbullets[i]->speed * CalcComponentY(0, 0, vx, vy);

					bossbullets[i]->angle = SDL_atan2(-bossbullets[i]->speed_x, bossbullets[i]->speed_y) * 180 / PI;
				}
			}


			bossbullets[i]->lx += bossbullets[i]->speed_x;
			bossbullets[i]->ly += bossbullets[i]->speed_y;

			bossbullets[i]->x = (int)bossbullets[i]->lx;
			bossbullets[i]->y = (int)bossbullets[i]->ly;
		}
		if (bossbullets[i] != NULL)
		{
			if (bossbullets[i]->y >= windowH)
			{
				DestroyBossBullet(&bossbullets[i]);
			}
		}
	}
}

void IfPlayerShotByBoss(GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < BOSS_BULLET_AMOUNT_MAX; i++)
	{
		if (bossbullets[i] != NULL)
		{
			if (bossbullets[i]->bullet_type == BOSS_BULLET_TYPE_0 || bossbullets[i]->bullet_type == BOSS_BULLET_TYPE_2)
			{
				if (bossbullets[i]->x <= globalresources->box_player->x + globalresources->box_player->w - globalresources->box_player->w * 0.3
					&& bossbullets[i]->x + bossbullets[i]->w >= globalresources->box_player->x + globalresources->box_player->w * 0.3
					&& bossbullets[i]->y <= globalresources->box_player->y + globalresources->box_player->h
					&& bossbullets[i]->y + bossbullets[i]->h >= globalresources->box_player->y + globalresources->box_player->h * 0.8)
				{
					playerlife--;
					DestroyBossBullet(&bossbullets[i]);
				}
			}
		}
		if (bossbullets[i] != NULL)
		{
			if (bossbullets[i]->bullet_type == BOSS_BULLET_TYPE_1)
			{
				if (bossbullets[i]->x <= globalresources->box_player->x + globalresources->box_player->w - globalresources->box_player->w * 0.3
					&& bossbullets[i]->x + bossbullets[i]->w >= globalresources->box_player->x + globalresources->box_player->w * 0.3
					&& bossbullets[i]->y <= globalresources->box_player->y + globalresources->box_player->h
					&& bossbullets[i]->y + bossbullets[i]->h >= globalresources->box_player->y + globalresources->box_player->h * 0.8)
				{
					if (bossbullets[i]->hide != SDL_TRUE)
					{
						playerlife--;
						bossbullets[i]->hide = SDL_TRUE;
					}
				}
				else
				{
					bossbullets[i]->hide = SDL_FALSE;
				}
			}

		}
	}
}

void DrawBossBullets(GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < BOSS_BULLET_AMOUNT_MAX; i++)
	{
		if (bossbullets[i] != NULL)
		{
			SDL_Rect bossblt_rect = { bossbullets[i]->x, bossbullets[i]->y, bossbullets[i]->w, bossbullets[i]->h };
			SDL_RenderCopyEx(globalresources->rend, bossbullets[i]->tex, NULL, &bossblt_rect, bossbullets[i]->angle, NULL, SDL_FLIP_NONE);
		}
	}
}

void IfBossShot(int* p_score, int* p_rage, ObjectsParameters parameters, GlobalResources* globalresources)
{
	int i, j;
	for (i = 0; i < ENEMY_AMOUNT_MAX; i++)
	{
		for (j = 0; j < BULLET_AMOUNT_MAX; j++)
		{
			if (boss != NULL && bullets[j] != NULL)
			{
				if (boss != NULL && bullets[j] != NULL &&
					boss->x <= bullets[j]->x + parameters.bulletW && boss->x + boss->w >= bullets[j]->x
					&& boss->y <= bullets[j]->y + parameters.bulletH && boss->y + boss->h >= bullets[j]->y)
				{
					boss->life--;
					DestroyBullet(&bullets[j]);
					if (boss->life <= 0)
					{
						int x, y, w, h;
						x = boss->x;
						y = boss->y;
						w = boss->w;
						h = boss->h;
						DestroyBoss(&boss);
						Mix_PlayChannel(-1, globalresources->explosionSound, 0);

						extern BlitedTexture explosion;

						int change_w = explosion.w * w / 75;
						int change_h = explosion.h * w / 75;
						AddExplosionTexture(x + w / 2 - change_w / 2, y + h / 2 - change_h / 2, change_w, change_h);

						*p_score += 10;
						*p_rage += 50;
						if (*p_rage > 100)
						{
							*p_rage = 100;
						}
					}
				}
			

			}
		}
	}
}

void ShowBossLife(GlobalResources* globalresources)
{
	if (boss != NULL)
	{
		const int bosslifemax = 200;

		const int bosslifebar_imageW = 167;
		const int bosslifebar_imageH = 8;

		const int bosslifebarW = 450;
		const int bosslifebarH = 18;

		int full_iamgeW;
		int fullW;

		full_iamgeW = (int)((double)boss->life / bosslifemax * bosslifebar_imageW);
		fullW = (int)((double)boss->life / bosslifemax * bosslifebarW);

		{
			globalresources->bosslifebar_emptyRect->x = windowW / 2 - bosslifebarW / 2;
			globalresources->bosslifebar_emptyRect->y = 65;
			globalresources->bosslifebar_emptyRect->w = bosslifebarW;
			globalresources->bosslifebar_emptyRect->h = bosslifebarH;
		}
		{
			globalresources->bosslifebar_fullRect->x = windowW / 2 - bosslifebarW / 2;
			globalresources->bosslifebar_fullRect->y = 65;
			globalresources->bosslifebar_fullRect->w = fullW;
			globalresources->bosslifebar_fullRect->h = bosslifebarH;
		}
		{
			globalresources->bosslifebar_fullCut->x = 0;
			globalresources->bosslifebar_fullCut->y = 0;
			globalresources->bosslifebar_fullCut->w = full_iamgeW;
			globalresources->bosslifebar_fullCut->h = bosslifebar_imageH;
		}

		SDL_RenderCopy(globalresources->rend, globalresources->bosslifebar_emptyTexture, NULL, globalresources->bosslifebar_emptyRect);
		SDL_RenderCopy(globalresources->rend, globalresources->bosslifebar_fullTexture, globalresources->bosslifebar_fullCut, globalresources->bosslifebar_fullRect);
	}
}