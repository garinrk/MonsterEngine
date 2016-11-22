#include "MonsterController.h"



MonsterController::MonsterController()
{
	monster_object_ = new MonsterObject();
}


MonsterController::~MonsterController()
{
	if (monster_object_)
		delete monster_object_;
	monster_object_ = NULL;
}

void MonsterController::UpdateMonsterObject() {
	UpdateMonsterPosition();
}

void MonsterController::UpdateMonsterPosition() {

	float x_movement = static_cast<float>(MonsterEngine::RandomIntInRange_Inclusive(0, 1));
	float y_movement = static_cast<float>(MonsterEngine::RandomIntInRange_Inclusive(0, 1));
	MonsterPoint2D random_movement(x_movement, y_movement);
	TranslateMonsterPosition(random_movement);

}

void MonsterController::TranslateMonsterPosition(MonsterPoint2D& delta) {
	MonsterTransform mt = monster_object_->GetTransform();
	mt.Translate(delta.x(), delta.y());
	monster_object_->SetTransform(mt);
}
