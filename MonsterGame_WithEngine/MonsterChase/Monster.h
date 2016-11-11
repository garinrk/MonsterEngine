////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Monster.h
//
// summary:	Declares the monster class
// Garin Richards
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MONSTER_H
#define MONSTER_H

#include <random>
#include <time.h>
#include "MonsterPoint2D.h"
#include "IGameObjectController.h"
#include "Player.h"
class Monster {

	public:
		Monster(std::string new_name, int x_range_bound, int y_range_bound);
		Monster();
		~Monster();
		bool CheckForDeath();
		void SetRandomPos();
		void Update();
		inline void SetName(std::string new_name);
		inline MonsterPoint2D GetPosition() const;
		inline std::string GetName() const;
		inline int GetAge();
		inline void SetController(IGameObjectController * new_controller);
		void Zombify(Player * target_player);
		bool is_zombie_ = false;

	private:
		inline void MakeOlder(); 
		int bound_x_;
		int bound_y_;

		int age_ = 1;
		std::string name_;
		IGameObjectController * monster_controller_;
};

#include "Monster-inl.h"
#endif