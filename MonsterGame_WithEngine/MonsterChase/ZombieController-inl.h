#include "ZombieController.h"
#ifndef ZOMBIECONTROLLER_INL_H
#define ZOMBIECONTROLLER_INL_H



inline MonsterObject * ZombieController::GetMonsterObject() const 
{
	return monster_object_;
}
inline void ZombieController::SetMonsterObject(MonsterObject * new_object)
{
	monster_object_ = new_object;
}


#endif // !