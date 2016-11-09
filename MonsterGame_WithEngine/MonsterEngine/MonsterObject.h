#ifndef MONSTEROBJECT_H
#define MONSTEROBJECT_H

#include <string>
#include "MonsterTransform.h"
class MonsterObject
{

public:
	MonsterObject(std::string new_object_name);
	~MonsterObject();
	inline void SetTransform(const MonsterTransform &new_transform);
	inline const MonsterTransform& GetTransform() const;
	inline void ObjectTranslate(const MonsterPoint2D &delta_change);
private:
	std::string name_;
	MonsterTransform transform_;
	
};

#include "MonsterObject-inl.h"

#endif
