#pragma once
#include "IModifier.h"

IModifier::IModifier(float value, Type type) : _value(value), _type(type)
{
	static int currentId = 0;
	_id = currentId;
	++currentId;
}

void IModifier::apply(Character::Ptr target)
{
	_target = target;
}
