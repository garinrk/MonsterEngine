#include "MonsterController.h"
#ifndef MONSTERCONTROLLER_INL_H
#define MONSTERCONTROLLER_INL_H

inline void MonsterController::SetMonsterObject(MonsterObject * new_object) 
{
	monster_object_ = new_object;
}


inline MonsterObject * MonsterController::GetMonsterObject() const
{
	return monster_object_;
}


#endif // !MONSTERCONTROLLER_INL_H
