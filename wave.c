#include "wave.h"
#include "objects.h"

static Wave* ProbabilityDistribution(Wave* wave[], int p[], int num);

Wave DecideWave(int wave_num)
{
	Wave wave_1 = { 1, ENEMY_AMOUNT_MAX, 1200, 800, INITENEMYMODE_1, SDL_FALSE };

	Wave wave_2 = { 2, ENEMY_AMOUNT_MAX, 1200, 750, INITENEMYMODE_2, SDL_FALSE };

	Wave wave_3 = { 3, ENEMY_AMOUNT_MAX, 1200, 1200, INITENEMYMODE_3, SDL_FALSE };

	Wave wave_4 = { 4, ENEMY_AMOUNT_MAX, 1200, 1500, INITENEMYMODE_4, SDL_FALSE };

	Wave wave_5 = { 5, ENEMY_AMOUNT_MAX, 1200, 1500, INITENEMYMODE_5, SDL_FALSE };

	Wave wave_boss = { 6, 1, 2000, 0, 0, SDL_TRUE };

	if (wave_num % 50 == 0)
	{
		return wave_boss;
	}
	else
	{
		if (wave_num <= 10)
		{
			Wave* wave[WAVE_AMOUNT_MAX] = { &wave_1, &wave_2 };
			int p[WAVE_AMOUNT_MAX] = { 3, 2 };
			int wave_num = 2;
			return *ProbabilityDistribution(wave, p, wave_num);
		}
		else if (wave_num > 10 && wave_num <= 20)
		{
			Wave* wave[WAVE_AMOUNT_MAX] = { &wave_1, &wave_2, &wave_3, &wave_4 };
			int p[WAVE_AMOUNT_MAX] = { 2, 2, 3, 1 };
			int wave_num = 4;
			return *ProbabilityDistribution(wave, p, wave_num);
		}
		else if (wave_num > 20 && wave_num <= 30)
		{
			Wave* wave[WAVE_AMOUNT_MAX] = { &wave_1, &wave_2, &wave_3, &wave_4 };
			int p[WAVE_AMOUNT_MAX] = { 1, 2, 4, 4 };
			int wave_num = 4;
			return *ProbabilityDistribution(wave, p, wave_num);
		}
		else if (wave_num > 30 && wave_num <= 40)
		{
			Wave* wave[WAVE_AMOUNT_MAX] = { &wave_1, &wave_2, &wave_3, &wave_4, &wave_5 };
			int p[WAVE_AMOUNT_MAX] = { 1, 2, 3, 4, 6 };
			int wave_num = 5;
			return *ProbabilityDistribution(wave, p, wave_num);
		}
		else
		{
			Wave* wave[WAVE_AMOUNT_MAX] = { &wave_1, &wave_2, &wave_3, &wave_4, &wave_5 };
			int p[WAVE_AMOUNT_MAX] = { 1, 2, 3, 4, 10 };
			int wave_num = 5;
			return *ProbabilityDistribution(wave, p, wave_num);
		}
	}
}

Wave* ProbabilityDistribution(Wave* wave[], int p[], int num)
{
	int psum = 0;
	int i;
	for (i = 0; i < num; i++)
	{
		psum += p[i];
	}
	int tmp = rand() % psum;
	int count = 0;
	int min = 0;
	int max = p[0];
	for (i = 0; i < num; i++)
	{
		if (tmp >= min && tmp < max)
		{
			break;
		}
		else
		{
			count++;
			min += p[i];
			max += p[i + 1];
		}
	}
	return wave[count];
}