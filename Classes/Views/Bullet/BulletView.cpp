#pragma once

#include "BulletView.h"

#include "Descriptors/WeaponDescriptor.h"
#include "Models/Character/Character.h"
#include "Models/Weapon/Weapon.h"
#include "Broadcaster/Broadcaster.h"
#include <cocos2d.h>
#include "cocostudio/CocoStudio.h"

bool BulletView::init(Weapon* weapon, std::shared_ptr<Character> target, bool hit)
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
			float damage = weaponDescriptor->damage;
			//перенес нанесение урона в персонажа, чтобы инкапсулировать его здоровье
			_target->receiveDamage(damage, _weapon->_characterId);
		}

		this->removeFromParentAndCleanup(true);
	}
}
