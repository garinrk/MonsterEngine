#include "Player.h"
#include "PlayerController.h"

Player::Player(std::string player_name):
	name_(player_name)
{
	player_controller_ = new PlayerController();
}

Player::Player() {
	player_controller_ = new PlayerController();
}

Player::~Player()
{
	if (player_controller_)
		delete player_controller_;
	player_controller_ = NULL;
}

Player::Player(const Player & other)
{
	player_controller_ = other.player_controller_;
	name_ = other.name_;
}

void Player::SetPosition(MonsterPoint2D & pos)
{
	MonsterTransform mt = player_controller_->GetMonsterObject()->GetTransform();
	mt.SetPosition(pos);
	player_controller_->GetMonsterObject()->SetTransform(mt);
}

MonsterPoint2D Player::GetPosition() const
{
	return player_controller_->GetMonsterObject()->GetTransform().GetPosition();
}

void Player::Update()
{
	player_controller_->UpdateMonsterObject();
}
