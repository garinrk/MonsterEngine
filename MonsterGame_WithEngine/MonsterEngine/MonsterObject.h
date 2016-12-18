#pragma once

#include "MonsterTransform.h"

#include <string>

class MonsterObject
{

public:
	MonsterObject();
	~MonsterObject();
	inline void SetTransform(const MonsterTransform &i_delta);
	inline const MonsterTransform& GetTransform() const;
	inline void ObjectTranslate(const MonsterPoint2D &i_delta);
private:
	MonsterTransform transform_;
	
};

#include "MonsterObject-inl.h"

