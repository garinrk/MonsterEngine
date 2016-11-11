#ifndef MONSTEROBJECT_H
#define MONSTEROBJECT_H

#include <string>
#include "MonsterTransform.h"
class MonsterObject
{

public:
	MonsterObject();
	~MonsterObject();
	inline void SetTransform(const MonsterTransform &new_transform);
	inline const MonsterTransform& GetTransform() const;
	inline void ObjectTranslate(const MonsterPoint2D &delta_change);
private:
	MonsterTransform transform_;
	
};

#include "MonsterObject-inl.h"

#endif
