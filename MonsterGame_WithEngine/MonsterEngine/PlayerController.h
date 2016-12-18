#pragma once

#include "IGameObjectController.h"
#include "MonsterDebug.h"

#include <conio.h>

class PlayerController : public IGameObjectController
{
public:
	inline MonsterObject * GetMonsterObject() const override;
	inline void SetMonsterObject(MonsterObject * i_object) override;
	void UpdateMonsterObject() override;
	void UpdatePlayerPosition();
	void TranslatePosition(MonsterPoint2D& o_translate);
	char GetControllerInput(); //only accounts for keyboard control currently
	inline bool GetGameOverState() const;
	inline void SetGameOverState(bool i_newValue);

	MonsterPoint2D ParseControllerInput(char i_toParse);
	
	PlayerController();
	~PlayerController();
private :
	MonsterObject * monster_object_;
	bool game_over_flag_ = false;
};

#include "PlayerController-inl.h"