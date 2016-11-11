#ifndef MONSTERCONTROLLER_H
#define MONSTERCONTROLLER_H

#include "IGameObjectController.h"
class MonsterController : public IGameObjectController
{
public:
	inline MonsterObject * GetMonsterObject() const override;
	inline void SetMonsterObject(MonsterObject * new_object) override;
	void UpdateMonsterObject() override;
	void UpdateMonsterPosition();
	void TranslateMonsterPosition();
	char GetMonsterInput();

	MonsterPoint2D ParseMonsterInput(int in);

	MonsterController();
	~MonsterController();

private:
	MonsterObject * monster_object_;
};

#include "MonsterController-inl.h"
#endif // !MONSTERCONTROLLER_H

