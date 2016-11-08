#pragma once
#include <string>
#include "MonsterTransform.h"
class MonsterObject
{

public:
	MonsterObject(std::string new_object_name);
	~MonsterObject();
	//transform 
	//constructor takes name
	//get component
	//name var
private:
	std::string name_;
	MonsterTransform transform_;
};

