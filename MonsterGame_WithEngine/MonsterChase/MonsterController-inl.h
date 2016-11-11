#include "MonsterController.h"
#ifndef MONSTERCONTROLLER_INL_H
#define MONSTERCONTROLLER_INL_H

inline void MonsterController::SetMonsterObject(MonsterObject * new_object) 
{
	monster_object_ = new_object;
}

inline void MonsterController::UpdateMonsterObject()
{
}

inline void MonsterController::UpdateMonsterPosition()
{
}

inline void MonsterController::TranslateMonsterPosition()
{
}

inline char MonsterController::GetMonsterInput()
{
	return 0;
}

inline MonsterObject * MonsterController::GetMonsterObject() const
{
	return monster_object_;
}


#endif // !MONSTERCONTROLLER_INL_H
