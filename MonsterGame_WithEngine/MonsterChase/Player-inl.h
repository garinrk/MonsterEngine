#pragma once

inline std::string Player::GetName() const{
	return name_;

}
inline void Player::SetName(std::string new_name) {
	name_ = new_name;
}

inline IGameObjectController * Player::GetController() const {
	return player_controller_;
}