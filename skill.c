#include "skill.h"
#include "objects.h"
#include "boss.h"

static ShortAttack* CreateShortAttack(int x, int y, int w, int h, uint32_t gametime);

static void DestroyShortAttack(ShortAttack** pp_shtatk);

void ShowRageBar(uint32_t gametime, uint32_t* p_ragestartT, SDL_bool* p_is_ragejustfull, GlobalResources* globalresources)
{
	const int rage_imageW = 6;
	const int rage_imageH = 120;

	const int ragebarW = 6 * 2;
	const int ragebarH = 120 * 2;

	int full_iamgeH;
	int fullH;

	full_iamgeH = (int)((double)rage / RAGE_MAX * rage_imageH);
	fullH = (int)((double)rage / RAGE_MAX * ragebarH);
	{
		globalresources->ragebar_empetyRect->x = 10;
		globalresources->ragebar_empetyRect->y = 65;
		globalresources->ragebar_empetyRect->w = ragebarW;
		globalresources->ragebar_empetyRect->h = ragebarH;
	}
	{
		globalresources->ragebar_fullRect->x = 10;
		globalresources->ragebar_fullRect->y = 65 + (ragebarH - fullH);
		globalresources->ragebar_fullRect->w = ragebarW;
		globalresources->ragebar_fullRect->h = fullH;
	}
	{
		globalresources->ragebar_fullCut->x = 0;
		globalresources->ragebar_fullCut->y = rage_imageH - full_iamgeH;
		globalresources->ragebar_fullCut->w = rage_imageW;
		globalresources->ragebar_fullCut->h = rage_imageH;
	}

	SDL_RenderCopy(globalresources->rend, globalresources->ragebar_empetyTexture, NULL, globalresources->ragebar_empetyRect);
	SDL_RenderCopy(globalresources->rend, globalresources->ragebar_fullTexture, globalresources->ragebar_fullCut, globalresources->ragebar_fullRect);

	if (rage == RAGE_MAX)
	{
		if (*p_is_ragejustfull == SDL_FALSE)
		{
			*p_ragestartT = gametime;
		}
		SDL_SetTextureAlphaMod(globalresources->ragebar_lightTexture, SinVar(gametime, *p_ragestartT, 254, 127, 1500, 127));
		SDL_RenderCopy(globalresources->rend, globalresources->ragebar_lightTexture, NULL, globalresources->ragebar_empetyRect);
		*p_is_ragejustfull = SDL_TRUE;
	}
	else
	{
		*p_is_ragejustfull = SDL_FALSE;
	}
}

void ShowRageBarWhilePause(GlobalResources* globalresources)
{
	SDL_RenderCopy(globalresources->rend, globalresources->ragebar_empetyTexture, NULL, globalresources->ragebar_empetyRect);
	SDL_RenderCopy(globalresources->rend, globalresources->ragebar_fullTexture, globalresources->ragebar_fullCut, globalresources->ragebar_fullRect);
	if (rage == RAGE_MAX)
	{
		SDL_RenderCopy(globalresources->rend, globalresources->ragebar_lightTexture, NULL, globalresources->ragebar_empetyRect);
	}
}

void InitSkills()
{
	Skill skill_1 = { 1, "1", 3000l, 3000l };

	Skill skill_2 = { 2, "2", 0, 0 };

	skills[0] = skill_1;
	skills[1] = skill_2;
}

void CalcSkillTimeLeft(uint32_t gametime)
{
	int i;
	for (i = 0; i < SKILL_ANOUNT_MAX; i++)
	{
		if (skills[i].is_using == SDL_TRUE)
		{
			skills[i].timeleft = skills[i].duration - (gametime - skills[i].start_time);
		}
		if (skills[i].timeleft <= 0)
		{
			skills[i].is_using = SDL_FALSE;
		}
	}
}

void RespondSkills(int ID, uint32_t gametime, SkillParameters parameters, GlobalResources* globalresources)
{
	if (rage == RAGE_MAX)
	{
		if (ID == 1)
		{
			skills[0].timeleft = skills[0].duration;
			if (skills[0].is_using == SDL_FALSE)
			{
				skills[0].start_time = gametime;
			}
			skills[0].is_using = SDL_TRUE;

			rage = 0;
		}
		else if (ID == 2)
		{
			Mix_PlayChannel(-1, globalresources->bombexplosionSound, 0);
			AddBomb(globalresources->box_player->x + globalresources->box_player->w / 2 - parameters.bombW / 2, globalresources->box_player->y, parameters);
			rage = 0;
		}
	}
}

void UseSkills(uint32_t gametime)
{
	if (skills[0].is_using == SDL_TRUE)
	{
		if (trackmode == SDL_FALSE)
		{
			trackmode = SDL_TRUE;
		}
	}
	else
	{
		if (trackmode == SDL_TRUE)
		{
			trackmode = SDL_FALSE;
		}
	}
}

ShortAttack* CreateShortAttack(int x, int y, int w, int h, uint32_t gametime)
{
	ShortAttack* p_shtatk = SDL_malloc(sizeof(ShortAttack));
	p_shtatk->x = x;
	p_shtatk->y = y;
	p_shtatk->w = w;
	p_shtatk->h = h;
	p_shtatk->createtime = gametime;
	p_shtatk->duration = 500;
	return p_shtatk;
}

void AddShortAttack(int x, int y, GlobalResources* globalresources, uint32_t gametime)
{

	int i;
	for (i = 0; i < SHORTATTACK_AMOUNT_MAX; i++)
	{
		if (shortattacks[i] == NULL)
		{
			int w = 120;
			int h = 60;
			shortattacks[i] = CreateShortAttack(globalresources->box_player->x + globalresources->box_player->w / 2 - w / 2,
				globalresources->box_player->y + globalresources->box_player->h / 2 - 1.5 * h, w, h, gametime);
			break;
		}
	}
}

void DestroyShortAttack(ShortAttack** pp_shtatk)
{
	SDL_free(*pp_shtatk);
	*pp_shtatk = NULL;
}

void VanishShortAttack(uint32_t gametime)
{
	int i;
	for (i = 0; i < SHORTATTACK_AMOUNT_MAX; i++)
	{
		if (shortattacks[i] != NULL)
		{
			if (gametime - shortattacks[i]->createtime >= shortattacks[i]->duration)
			{
				DestroyShortAttack(&shortattacks[i]);
			}
		}
	}
}

void DestroyAllShortAttack()
{
	int i;
	for (i = 0; i < SHORTATTACK_AMOUNT_MAX; i++)
	{
		if (shortattacks[i] == NULL)
		{
			DestroyShortAttack(&shortattacks[i]);
		}
	}
}

void ShowShortAttack(GlobalResources* globalresources)
{
	int i;
	for (i = 0; i < SHORTATTACK_AMOUNT_MAX; i++)
	{
		if (shortattacks[i] != NULL)
		{
			SDL_Rect shortattack_rect = { shortattacks[i]->x, shortattacks[i]->y, shortattacks[i]->w, shortattacks[i]->h };
			SDL_RenderCopy(globalresources->rend, globalresources->shortattackTexture, NULL, &shortattack_rect);
		}
	}
}

void ShortAttackTakeEffect(ObjectsParameters parameters)
{
	int i;
	for (i = 0; i < SHORTATTACK_AMOUNT_MAX; i++)
	{
		if (shortattacks[i] != NULL)
		{
			int j;
			for (j = 0; j < ENEMYBULLET_AMOUNT_MAX; j++)
			{
				if (enemybullets[j] != NULL)
				{
					if (enemybullets[j]->x <= shortattacks[i]->x + shortattacks[i]->w
						&& enemybullets[j]->x + parameters.enemybulletW >= shortattacks[i]->x
						&& enemybullets[j]->y <= shortattacks[i]->y + shortattacks[i]->h
						&& enemybullets[j]->y + parameters.enemybulletH >= shortattacks[i]->y)
					{
						DestroyEnemyBullet(&enemybullets[j]);
					}
				}
			}
			for (j = 0; j < BOSS_BULLET_AMOUNT_MAX; j++)
			{
				if (bossbullets[j] != NULL)
				{
					if (bossbullets[j]->removeable == SDL_TRUE)
					{
						if (bossbullets[j]->x <= shortattacks[i]->x + shortattacks[i]->w
							&& bossbullets[j]->x + bossbullets[j]->w >= shortattacks[i]->x
							&& bossbullets[j]->y <= shortattacks[i]->y + shortattacks[i]->h
							&& bossbullets[j]->y + bossbullets[j]->h >= shortattacks[i]->y)
						{
							DestroyBossBullet(&bossbullets[j]);
						}
					}
				}
			}
		}
	}
}

void ShowSkillIcons(GlobalResources* globalresources)
{
	SDL_Rect skill1Rect = { 5, 320, 40, 40 };
	SDL_Rect skill2Rext = { 5, 370, 40, 40 };

	SDL_RenderCopy(globalresources->rend, globalresources->skill1Texture, NULL, &skill1Rect);
	SDL_RenderCopy(globalresources->rend, globalresources->skill2Texture, NULL, &skill2Rext);
}