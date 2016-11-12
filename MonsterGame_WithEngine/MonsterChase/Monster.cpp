////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Monster.cpp
//
// summary:	Implements the monster class for MonsterChase game.
// Garin Richards
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Monster.h"
#include "MonsterController.h"
#include "ZombieController.h"


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
	monster_controller_ = new MonsterController();
	SetRandomPos();
}

Monster::~Monster()
{
	if (monster_controller_)
		delete monster_controller_;
	monster_controller_ = NULL;
}

Monster::Monster(const Monster& other)
{
	age_ = other.age_;
	bound_x_ = other.bound_x_;
	bound_y_ = other.bound_y_;
	name_ = other.name_;

	monster_controller_ = other.monster_controller_;

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


void Monster::Zombify(Player * target_player) {
	ZombieController * new_zombie_controller = new ZombieController(target_player);
	MonsterObject* mo = monster_controller_->GetMonsterObject();
	new_zombie_controller->SetMonsterObject(mo);
	SetController(new_zombie_controller);
	is_zombie_ = true;
}


