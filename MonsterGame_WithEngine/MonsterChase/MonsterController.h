#pragma once

#include "IGameObjectController.h"
#include "MonsterEngine.h"

class MonsterObject;

class MonsterController : public IGameObjectController
{
public:
	inline MonsterObject * GetMonsterObject() const override;
	inline void SetMonsterObject(MonsterObject * new_object) override;
	void UpdateMonsterObject() override;
	void UpdateMonsterPosition();
	void TranslateMonsterPosition(MonsterPoint2D& delta);

	MonsterController();
	~MonsterController();

private:
	MonsterObject * monster_object_;
};

#include "MonsterController-inl.h"

