#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "IGameObjectController.h"
#include <conio.h>
#include "MonsterDebug.h"
class PlayerController : public IGameObjectController
{
public:
	inline MonsterObject * GetMonsterObject() const override;
	inline void SetMonsterObject(MonsterObject * new_Object) override;
	void UpdateMonsterObject() override;
	void UpdatePlayerPosition();
	void TranslatePosition(MonsterPoint2D& translation);
	char GetControllerInput(); //only accounts for keyboard control currently
	inline bool GetGameOverState() const;
	inline void SetGameOverState(bool new_value);

	MonsterPoint2D ParseControllerInput(char in);
	
	PlayerController();
	~PlayerController();
private :
	MonsterObject * monster_object_;
	bool game_over_flag_ = false;
};

#include "PlayerController-inl.h"
#endif