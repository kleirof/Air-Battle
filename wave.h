#include "main.h"

typedef struct Wave
{
	int ID;
	int num;
	int fore_interval;
	int enemy_interval;
	int enemy_type;
	SDL_bool is_boss_wave;
} Wave;

extern Wave DecideWave(int wave_num);