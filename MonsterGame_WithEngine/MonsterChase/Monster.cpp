////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Monster.cpp
//
// summary:	Implements the monster class for MonsterChase game.
// Garin Richards
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <random>
#include <time.h>

#include "IGameObjectController.h"
#include "Monster.h"
#include "MonsterController.h"
#include "Player.h"
#include "ZombieController.h"



//constructor
Monster::Monster(std::string& new_name, int x_range_bound, int y_range_bound) :
name_(new_name),
bound_x_(x_range_bound),
bound_y_(y_range_bound)
{
	monster_controller_ = new MonsterController();
	SetRandomPos();

}

Monster::Monster() :
	name_("temp"),
	bound_x_(100),
	bound_y_(100)
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


//assignment
Monster & Monster::operator=(const Monster && i_other)
{

	//std::string name
	name_ = i_other.name_;

	//bound x, bound y ints
	bound_x_ = i_other.bound_x_;
	bound_y_ = i_other.bound_y_;
	
	//int age
	age_ = i_other.age_;

	//controller
	monster_controller_ = i_other.monster_controller_;

	return *this;
}

//move
Monster::Monster(Monster && i_other) :
	bound_x_(i_other.bound_x_),
	bound_y_(i_other.bound_y_),
	age_(i_other.age_),
	name_(i_other.name_),
	monster_controller_(i_other.monster_controller_)
{	
	i_other.name_ = nullptr;
}

//copy
Monster::Monster(const Monster & i_other) :
	bound_x_(i_other.bound_x_),
	bound_y_(i_other.bound_y_),
	age_(i_other.age_),
	name_(i_other.name_),
	monster_controller_(i_other.monster_controller_)
{	}

Monster & Monster::operator=(const Monster & i_other)
{	
	//std::string name
	name_ = i_other.name_;

	//bound x, bound y ints
	bound_x_ = i_other.bound_x_;
	bound_y_ = i_other.bound_y_;

	//int age
	age_ = i_other.age_;

	//controller
	monster_controller_ = i_other.monster_controller_;

	return *this;

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

	/*MonsterTransform mt(rand() % static_cast<int>((bound_x_ + 1)), rand() % static_cast<int((bound_y_ + 1)));*/
	float x = static_cast<float>(rand() % static_cast<int>(bound_x_ + 1));
	float y = static_cast<float>(rand() % static_cast<int>(bound_y_ + 1));
	MonsterTransform mt(x, y);
	monster_controller_->GetMonsterObject()->SetTransform(mt);
}


void Monster::Zombify(Player * target_player) {
	ZombieController * new_zombie_controller = new ZombieController(target_player);
	MonsterObject* mo = monster_controller_->GetMonsterObject();
	new_zombie_controller->SetMonsterObject(mo);
	SetController(new_zombie_controller);
	is_zombie_ = true;
}

///Special Assignment Operators


