#ifndef PLAYER_H
#define PLAYER_H

#include "IGameObjectController.h"
#include "PlayerController.h"
class Player
{
public:
	Player();
	Player(std::string player_name);
	~Player();
	Player(const Player& other);

	void SetPosition(MonsterPoint2D& pos);
	MonsterPoint2D GetPosition() const;
	void Update();
	//inline PlayerController GetPlayerController() const;
	inline std::string GetName() const;
	inline void SetName(std::string new_name);
	inline IGameObjectController * GetController() const;
private:
	IGameObjectController * player_controller_;
	std::string name_;
	
};

#include "Player-inl.h"
#endif // !PLAYER_H

