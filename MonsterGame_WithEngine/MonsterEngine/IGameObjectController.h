#pragma once
#include "MonsterObject.h"

class IGameObjectController
{
public:
	virtual void SetMonsterObject(MonsterObject * i_newObject) = 0;
	virtual void UpdateMonsterObject() = 0;
	virtual MonsterObject * GetMonsterObject() const = 0;
};

