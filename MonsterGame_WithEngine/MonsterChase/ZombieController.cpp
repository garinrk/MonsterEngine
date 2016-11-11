#include "ZombieController.h"



ZombieController::ZombieController(Player * target_player) :
	player_to_follow_(target_player)
{
	monster_object_ = new MonsterObject();
}


ZombieController::~ZombieController()
{
}

void ZombieController::UpdateMonsterObject() 
{
	UpdateZombiePosition();
}
void ZombieController::UpdateZombiePosition()
{
	MonsterPoint2D player_position = player_to_follow_->GetPosition();
	MonsterPoint2D my_position = monster_object_->GetTransform().GetPosition();
	
	double player_x = player_position.x();
	double player_y = player_position.y();
	
	double x_delta = GetDeltaInXDirection(my_position.x(),player_x);
	double y_delta = GetDeltaInYDirection(my_position.y(),player_y);

	MonsterPoint2D delta(x_delta, y_delta);

	TranslateZombiePosition(delta);
}
void ZombieController::TranslateZombiePosition(MonsterPoint2D& delta)
{
	MonsterTransform mt = monster_object_->GetTransform();
	mt.Translate(delta.x(), delta.y());
	monster_object_->SetTransform(mt);
}

double ZombieController::GetDeltaInXDirection(double my_x, double other_x) {
	if (my_x > other_x)
		return -1;
	else if (my_x < other_x)
		return 1;
	else
		return 0;
}
double ZombieController::GetDeltaInYDirection(double my_y, double other_y) {
	if (my_y > other_y)
		return -1;
	else if (my_y < other_y)
		return 1;
	else
		return 0;
}