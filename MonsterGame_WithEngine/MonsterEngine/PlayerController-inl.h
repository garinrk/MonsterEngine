#pragma once
#include "PlayerController.h"


inline void PlayerController::SetMonsterObject(MonsterObject * i_newObject)
{
	monster_object_ = i_newObject;
}

inline MonsterObject * PlayerController::GetMonsterObject() const
{
	return monster_object_;
}

inline bool PlayerController::GetGameOverState() const {
	return game_over_flag_;
}
inline void PlayerController::SetGameOverState(bool i_newValue) {
	game_over_flag_ = i_newValue;
}