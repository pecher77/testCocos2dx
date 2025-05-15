#pragma once

#include "BulletView.h"

#include <cocos2d.h>

bool BulletView::init(Weapon* weapon, Character* target, bool hit)
{
	_weapon = weapon;
	_target = target;
	_hit = hit;

	this->scheduleUpdate();

	return true;
}

void BulletView::update(float deltaTime)
{
	WeaponDescriptor* weaponDescriptor = _weapon->descriptor;
	cocos2d::Vec3 targetPosition = _target->getPosition() + cocos2d::Vec3(0, 2, 0);
	cocos2d::Vec3 direction = targetPosition - this->getPosition3D();
	direction.normalize();
	cocos2d::Vec3 newPosition = this->getPosition3D() + direction * weaponDescriptor->muzzleVelocity * deltaTime * 10;
	this->setPosition3D(newPosition);
	float distance = targetPosition.distance(this->getPosition3D());
	if (distance < 10)
	{
		if (_hit)
		{
			CharacterDescriptor* targetDescriptor = _target->descriptor;
			float damage = weaponDescriptor->damage;
			if (_target->armor > 0)
			{
				_target->armor -= damage;
			}
			else if (_target->health > 0)
			{
				_target->health -= damage;
			}
			if (_target->armor <= 0 && _target->health <= 0)
			{
				_target->runAnimation("objects/Death.c3b", false);
			}
		}

		this->removeFromParentAndCleanup(true);
	}
}
