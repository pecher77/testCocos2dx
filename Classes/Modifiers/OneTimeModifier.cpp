#pragma once
#include "OneTimeModifier.h"
#include "Models/Weapon/Weapon.h"
#include "Descriptors/WeaponDescriptor.h"

void OneTimeModifier::apply(Character::Ptr target)
{
	IModifier::apply(target);

	switch (_type)
	{	
	case IModifier::Type::NONE:
		break;
	case IModifier::Type::HEALTH:
		target->getCharacterDescriptor()->maxHealth += _value;
		target->setHealth(target->getHealth() + _value);
		break;
	case IModifier::Type::ACCURACY:
		target->getCharacterDescriptor()->accuracy = _value;
		break;
	case IModifier::Type::DEXTERITY:
		target->getCharacterDescriptor()->dexterity = _value;
		break;
	case IModifier::Type::ARMOR:
		target->getCharacterDescriptor()->maxArmor += _value;
		target->setArmor(target->getArmor() + _value);
		break;
	case IModifier::Type::AIM_TIME:
		target->getCharacterDescriptor()->aimTime = _value;
		break;

	case IModifier::Type::DAMAGE:
		target->getWeapon()->descriptor->damage = _value;
		break;
	case IModifier::Type::WEAPON_ACCURACY:
		target->getWeapon()->descriptor->accuracy = _value;
		break;
	case IModifier::Type::FIRE_RATE:
		target->getWeapon()->descriptor->fireRate = _value;
		break;
	case IModifier::Type::CLIP_SIZE:
		target->getWeapon()->descriptor->clipSize = _value;
		break;
	case IModifier::Type::MUZZLE_VELOCITY:
		target->getWeapon()->descriptor->muzzleVelocity = _value;
		break;
	case IModifier::Type::RELOAD_RIME:
		target->getWeapon()->descriptor->reloadTime = _value;
		break;

	case IModifier::Type::BULLET_SIZE:
		target->getWeapon()->descriptor->bulletSizeMultiplier = _value;
		break;
	default:
		break;
	}
}