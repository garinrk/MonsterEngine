#pragma once

inline void Monster::MakeOlder() {
	age_++;
}

inline MonsterPoint2D Monster::GetPosition() const
{
	return monster_controller_->GetMonsterObject()->GetTransform().GetPosition();
}

inline std::string Monster::GetName() const
{
	return name_;
}

inline void Monster::SetName(std::string new_name) {
	name_ = new_name;
}

inline int Monster::GetAge() {
	return age_;
}

inline void Monster::SetController(IGameObjectController * new_controller) {
	monster_controller_ = new_controller;
}
