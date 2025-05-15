#pragma once
#include "TimedModifier.h"
#include "Models/Weapon/Weapon.h"
#include "Descriptors/WeaponDescriptor.h"

TimedModifier::TimedModifier(float value, Type type, float time) : IModifier(value, type), _time(time), _applyed(false)
{
}

TimedModifier::TimedModifier(Type type, float time) : IModifier(0.f, type), _time(time), _applyed(false)
{
}

void TimedModifier::apply(Character::Ptr target)
{
	IModifier::apply(target);
	_applyed = true;

	switch (_type)
	{
	case IModifier::Type::TIMED_IMMORTALITY:
		target->applyImmortality();
		break;
	case IModifier::Type::TIMED_ACCURACY:
		_oldValue = target->getCharacterDescriptor()->accuracy;
		target->getCharacterDescriptor()->accuracy += _value;
		break;
	case IModifier::Type::TIMED_BULLET_SIZE:
		_oldValue = target->getWeapon()->descriptor->bulletSizeMultiplier;
		target->getWeapon()->descriptor->bulletSizeMultiplier = _value;
		break;
	default:
		break;
	}
}

bool TimedModifier::update(float delta)
{
	if (!_applyed)
	{
		return false;
	}

	_time -= delta;
	if (_time < 0)
	{
		cancel();
		return true;
	}

	return false;
}

void TimedModifier::cancel()
{
	switch (_type)
	{
	case IModifier::Type::TIMED_IMMORTALITY:
		_target->cancelImmortality();
		break;
	case IModifier::Type::TIMED_ACCURACY:
		 _target->getCharacterDescriptor()->accuracy = _oldValue;
		break;
	case IModifier::Type::TIMED_BULLET_SIZE:
		_target->getWeapon()->descriptor->bulletSizeMultiplier = _oldValue;
		break;
	default:
		break;
	}
}
