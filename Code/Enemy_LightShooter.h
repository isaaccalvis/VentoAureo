#ifndef __ENEMY_REDBIRD_H__
#define __ENEMY_REDBIRD_H__

#include "Enemy.h"
#include "ModuleEnemies.h"

class Enemy_LightShooter : public Enemy{
private:
	bool going_down = true;
	bool going_up = false;
	int original_y = 0;
	Animation lightShooterAnim;

public:
	Enemy_LightShooter(int x, int y);
	void Move();
};
#endif