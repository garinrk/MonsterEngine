#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "IGameObjectController.h"
class PlayerController : IGameObjectController
{
public:
	inline void SetMonsterObject(MonsterObject * new_Object) override;
	//virtual void UpdateMonsterObject() = 0;
	void UpdateMonsterObject() override;
	PlayerController();
	~PlayerController();
private :
	MonsterObject * monster_object_;
};

#include "PlayerController-inl.h"
#endif