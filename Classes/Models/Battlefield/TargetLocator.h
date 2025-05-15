#pragma once

#include "Models/Character/Character.h"
#include "cocos2d.h"

class Character;
class TargetLocator
{
public:
	virtual Character::Ptr getNearestAliveTarget(Character::Ptr character) = 0;
};