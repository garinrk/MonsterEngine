#include "PlayerController.h"
#ifndef PLAYERCONTROLLER_INL_H
#define PLAYERCONTROLLER_INL_H

inline void PlayerController::SetMonsterObject(MonsterObject * new_Object)
{
	monster_object_ = new_Object;
}

inline MonsterObject * PlayerController::GetMonsterObject() const
{
	return monster_object_;
}


#endif // !PLAYERCONTROLLER_INL_H
