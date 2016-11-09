#include "MonsterObject.h"



MonsterObject::MonsterObject(std::string new_object_name) :
	name_(new_object_name),
	transform_(0,0)
{
}


MonsterObject::~MonsterObject()
{
}
