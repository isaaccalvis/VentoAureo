#ifndef __HeavyOrangeAirship_H__
#define __HeavyOrangeAirship_H__

#include "Enemy.h"
#include "ModuleEnemies.h"

class HeavyOrangeAirship : public Enemy {
private:
	Animation HeavyOrangeAirship_Animation;
	Animation HeavyOrangeAirship_AnimationDeixantBomba;

	int current_time;
public:
	HeavyOrangeAirship(int x, int y);
	~HeavyOrangeAirship(){}
	void Move();
	void OnCollision(Collider* collider);
	void DrawUp();
};
#endif