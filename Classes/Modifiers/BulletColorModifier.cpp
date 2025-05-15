#pragma once
#include "BulletColorModifier.h"
#include "Models/Weapon/Weapon.h"
#include "Descriptors/WeaponDescriptor.h"

BulletColorModifier::BulletColorModifier(cocos2d::Color3B color, Type type) : IModifier(0.f, type)
{
}

void BulletColorModifier::apply(Character::Ptr target)
{
	target->getWeapon()->descriptor->bulletColor = _color;
	target->getWeapon()->descriptor->needApplyColor = true;
}