#include "main.h"
#include "globals.h"
#include "objects.h"

typedef struct Skill
{
	int ID;
	int key;
	int duration;
	int timeleft;
	uint32_t start_time;
	SDL_bool is_using;
} Skill;

Skill skills[SKILL_ANOUNT_MAX];

typedef struct SkillParameters
{
	const int bombW;
	const int bombH;
} SkillParameters;

typedef struct ShortAttack
{
	int x, y;
	int w, h;
	uint32_t createtime;
	int duration;
} ShortAttack;

ShortAttack* shortattacks[SHORTATTACK_AMOUNT_MAX];

extern void ShowRageBar(uint32_t gametime, uint32_t* p_ragestartT, SDL_bool* p_is_ragejustfull, GlobalResources* globalresources);

extern void ShowRageBarWhilePause(GlobalResources* globalresources);

extern void InitSkills();

extern void CalcSkillTimeLeft(uint32_t gametime);

extern void RespondSkills(int ID, uint32_t gametime, SkillParameters parameters, GlobalResources* globalresources);

extern void UseSkills(uint32_t gametime);

extern void AddShortAttack(int x, int y, GlobalResources* globalresources, uint32_t gametime);

extern void DestroyAllShortAttack();

extern void VanishShortAttack(uint32_t gametime);

extern void ShowShortAttack(GlobalResources* globalresources);

extern void ShortAttackTakeEffect(ObjectsParameters parameters);

extern void ShowSkillIcons(GlobalResources* globalresources);