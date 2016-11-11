////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Monster.cpp
//
// summary:	Implements the monster class for MonsterChase game.
// Garin Richards
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Monster.h"
#include "MonsterController.h"


//constructor
Monster::Monster(std::string new_name, int x_range_bound, int y_range_bound) :
name_(new_name),
bound_x_(x_range_bound),
bound_y_(y_range_bound)
{
	monster_controller_ = new MonsterController();
	SetRandomPos();

}

Monster::Monster() 
{

}


bool Monster::CheckForDeath() {

	
	int killOrNot = rand() % 2;

	if (killOrNot == 1) {
		return true;
	}
	
	return false;
}

void Monster::Update() {
	monster_controller_->UpdateMonsterObject();
	MakeOlder();
}


void Monster::SetRandomPos() {

	MonsterTransform mt(rand() % (bound_x_ + 1), rand() % (bound_y_ + 1));
	monster_controller_->GetMonsterObject()->SetTransform(mt);
}


