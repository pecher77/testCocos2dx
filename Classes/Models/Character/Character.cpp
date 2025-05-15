#include "Character.h"

#include "Models/Battlefield/TargetLocator.h"
#include "Models/Weapon/Weapon.h"
#include "cocos2d.h"

USING_NS_CC;

Character::Character(
	cocos2d::Sprite3D* view,
	CharacterDescriptor* character,
	Weapon* weapon,
	TargetLocator* targetLocator)
{
	_weapon = weapon;
	_targetLocator = targetLocator;
	_state = State::Idle;

	Sprite3d = view;
	descriptor = character;
	health = descriptor->maxHealth;
	armor = descriptor->maxArmor;
}

bool Character::isAlive() const
{
	return health > 0 || armor > 0;
}

cocos2d::Vec3 Character::getPosition() const
{
	return Sprite3d->getPosition3D();
}

void Character::update(float deltaTime)
{
	if (!isAlive())
		return;

	switch (_state)
	{
		case State::Idle:
			runAnimation("objects/Idle.c3b", true);

			_currentTarget = _targetLocator->getNearestAliveTarget(this);
			if (_currentTarget != nullptr)
			{
				_state = State::Aiming;
				_time = descriptor->aimTime;

				cocos2d::Vec3 direction = _currentTarget->getPosition() - this->getPosition();
				direction.normalize();
				float rotationY = CC_RADIANS_TO_DEGREES(atan2(direction.x, direction.z));
				cocos2d::Vec3 rotation = cocos2d::Vec3(0, rotationY, 0);
				Sprite3d->setRotation3D(rotation);
			}
			break;
		case State::Aiming:
			runAnimation("objects/Aiming.c3b", true);

			if (_currentTarget != nullptr && _currentTarget->isAlive())
			{
				if (_time > 0)
				{
					_time -= deltaTime;
				}
				else
				{
					_state = State::Shooting;
					_time = 0;
				}
			}
			else
			{
				_state = State::Idle;
				_time = 0;
			}
			break;
		case State::Shooting:
			if (_currentTarget != nullptr && _currentTarget->isAlive())
			{
				if (_weapon->hasAmmo())
				{
					if (_weapon->isReady())
					{
						float random = CCRANDOM_0_1();
						bool hit = random <= descriptor->accuracy
								   && random <= _weapon->descriptor->accuracy
								   && random >= _currentTarget->descriptor->dexterity;
						_weapon->fire(_currentTarget, hit);
						runAnimation("objects/Shooting.c3b", false, true);
					}
					else
					{
						_weapon->update(deltaTime);
					}
				}
				else
				{
					_state = State::Reloading;
					_time = _weapon->descriptor->reloadTime;
				}
			}
			else
			{
				_state = State::Idle;
			}
			break;
		case State::Reloading:
			runAnimation("objects/Reloading.c3b", false);

			if (_time > 0)
			{
				_time -= deltaTime;
			}
			else
			{
				if (_currentTarget != nullptr && _currentTarget->isAlive())
				{
					_state = State::Shooting;
				}
				else
				{
					_state = State::Idle;
				}

				_weapon->reload();
				_time = 0;
			}
			break;
	}
}

void Character::runAnimation(std::string name, bool repeat, bool force)
{
	if (_currentAnimation == name && !force)
	{
		return;
	}

	Sprite3d->stopAllActions();

	Animation3D* animation = Animation3D::create(name);
	Animate3D* animate = Animate3D::create(animation);

	if (repeat)
	{
		Sprite3d->runAction(RepeatForever::create(animate));
	}
	else
	{
		Sprite3d->runAction(animate);
	}

	_currentAnimation = name;
}
