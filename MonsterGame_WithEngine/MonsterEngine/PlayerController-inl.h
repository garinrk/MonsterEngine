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

inline bool PlayerController::GetGameOverState() const {
	return game_over_flag_;
}
inline void PlayerController::SetGameOverState(bool new_value) {
	game_over_flag_ = new_value;
}


#endif // !PLAYERCONTROLLER_INL_H
