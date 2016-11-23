#pragma once

#include "IGameObjectController.h"

class Player;
class MonsterObject;

class ZombieController : public IGameObjectController
{
public:
	inline MonsterObject * GetMonsterObject() const override;
	inline void SetMonsterObject(MonsterObject * new_object) override;
	void UpdateMonsterObject() override;
	void UpdateZombiePosition();
	void TranslateZombiePosition(MonsterPoint2D& delta);
	
	ZombieController();
	ZombieController(Player * target_player);
	~ZombieController();

private:
	MonsterObject * monster_object_;
	Player * player_to_follow_;
	float GetDeltaInXDirection(float my_x, float other_x);
	float GetDeltaInYDirection(float my_y, float other_y);
};
#include "ZombieController-inl.h"