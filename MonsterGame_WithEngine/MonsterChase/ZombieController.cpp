#include "ZombieController.h"



ZombieController::ZombieController(Player * target_player) :
	player_to_follow_(target_player)
{
	monster_object_ = new MonsterObject();
}

ZombieController::ZombieController() {
	monster_object_ = new MonsterObject();
}


ZombieController::~ZombieController()
{
	if (monster_object_)
		delete monster_object_;
	monster_object_ = NULL;
}

void ZombieController::UpdateMonsterObject() 
{
	UpdateZombiePosition();
}
void ZombieController::UpdateZombiePosition()
{
	MonsterPoint2D player_position = player_to_follow_->GetPosition();
	MonsterPoint2D my_position = monster_object_->GetTransform().GetPosition();
	
	float player_x = player_position.x();
	float player_y = player_position.y();
	
	float x_delta = GetDeltaInXDirection(my_position.x(),player_x);
	float y_delta = GetDeltaInYDirection(my_position.y(),player_y);

	MonsterPoint2D delta(x_delta, y_delta);

	TranslateZombiePosition(delta);
}
void ZombieController::TranslateZombiePosition(MonsterPoint2D& delta)
{
	MonsterTransform mt = monster_object_->GetTransform();
	mt.Translate(delta.x(), delta.y());
	monster_object_->SetTransform(mt);
}

float ZombieController::GetDeltaInXDirection(float my_x, float other_x) {
	if (my_x > other_x)
		return -1.0f;
	else if (my_x < other_x)
		return 1.0f;
	else
		return 0.0f;
}
float ZombieController::GetDeltaInYDirection(float my_y, float other_y) {
	if (my_y > other_y)
		return -1.0f;
	else if (my_y < other_y)
		return 1.0f;
	else
		return 0.0f;
}