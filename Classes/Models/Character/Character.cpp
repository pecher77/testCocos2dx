#include "Character.h"

#include "Models/Weapon/Weapon.h"
#include "Models/Battlefield/Battlefield.h"
#include "Broadcaster/Broadcaster.h"

USING_NS_CC;

Character::Character(
	int id, 
	int teamId,
	cocos2d::Sprite3D* view,
	CharacterDescriptor* descriptor,
	Weapon* weapon,
	TargetLocator* targetLocator) : _id(id), _teamId(teamId), _sprite3d(view), _descriptor(descriptor), _weapon(weapon)
			, _targetLocator(targetLocator), _health(descriptor->maxHealth)
			, _armor(descriptor->maxArmor) 
{
	_state = State::Idle;

	//создание прогрессбаров, для MVP достаточно иметь их в персонаже, но если будет усложнять и обрастать логикой - вынести в отдельный класс и инитить его
	//жизнь
	_helthBar = Sprite::create("ui/TEX_health_bar.png");
	_helthBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	_helthBar->setScale(2);

	auto color = (teamId % 2) == 0 ? Color3B(0, 153, 0) : Color3B(204, 0, 0);
	_helthBar->setColor(color);
	_helthBar->setPosition(Vec2(0, 200.0f));

	//броня
	_armorBar = Sprite::create("ui/TEX_armor_bar.png");
	_armorBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	_armorBar->setScale(2);
	_armorBar->setColor(Color3B(204, 102, 0));
	_armorBar->setPosition(Vec2(0, 230.0f));

	auto billboard = BillBoard::create();
	billboard->setMode(BillBoard::Mode::VIEW_PLANE_ORIENTED);
	billboard->addChild(_helthBar);
	billboard->addChild(_armorBar);
	billboard->setPosition3D(Vec3(0, 3.0f, 0));

	view->addChild(billboard, 100);
}

bool Character::isAlive() const
{
	return _health > 0 || _armor > 0;
}

cocos2d::Vec3 Character::getPosition() const
{
	return _sprite3d->getPosition3D();
}

void Character::receiveDamage(float damage, int shooterId)
{
	if (damage <= 0)
	{
		return;
	}

	if (_immortality)
	{
		return;
	}

	auto tintTo = TintTo::create(0.2f, 255, 50, 50);
	auto delay = DelayTime::create(0.1f);
	auto tintBack = TintTo::create(0.2f, 255, 255, 255);

	_sprite3d->runAction(Sequence::create(tintTo, delay, tintBack, nullptr));

	if (_armor > 0)
	{
		//остаток уменьшает на здоровье
		if (damage > _armor)
		{
			auto rest = damage - _armor;
			_armor = 0;
			_health -= rest;
			updateHealthProgress();
		}

		_armor -= damage;
		updateArmorProgress();
	}
	else if (_health > 0)
	{
		_health -= damage;
		updateHealthProgress();
	}

	if (_armor <= 0 && _health <= 0)
	{
		onDeath(shooterId);
	}
}

void Character::updateArmorProgress()
{
	//можно было бы избежать дублирования с updateHealthProgress, но лучше оставить 2 метода, т.к. логика должна усложняться
	auto percentage = _armor / _descriptor->maxArmor;
	_armorBar->setScaleX(percentage);
}

void Character::updateHealthProgress()
{
	auto percentage = _health / _descriptor->maxHealth;
	_helthBar->setScaleX(percentage);
}

void Character::applyImmortality()
{
	_sprite3d->setColor(cocos2d::Color3B(0, 0, 0));
	_immortality = true;
}

void Character::cancelImmortality()
{
	auto tintTo = TintTo::create(0.2f, 255, 255, 255);
	_sprite3d->runAction(tintTo);
	_immortality = false;
}

void Character::onDeath(int shooterId)
{
	runAnimation("objects/Death.c3b", false);

	EventCustom event("onCharactersDeath");
	event.setUserData(&_id);
	Broadcaster::instance().broadcastMessage(&event);

	EventCustom event2("onCharacterDidKill");
	event.setUserData(&shooterId);
	Broadcaster::instance().broadcastMessage(&event2);
}

void Character::update(float deltaTime)
{
	if (!isAlive())
		return;

	switch (_state)
	{
		case State::Idle:
			runAnimation("objects/Idle.c3b", true);

			_currentTarget = _targetLocator->getNearestAliveTarget(shared_from_this());
			if (_currentTarget)
			{
				_state = State::Aiming;
				_time = _descriptor->aimTime;

				cocos2d::Vec3 direction = _currentTarget->getPosition() - this->getPosition();
				direction.normalize();
				float rotationY = CC_RADIANS_TO_DEGREES(atan2(direction.x, direction.z));
				cocos2d::Vec3 rotation = cocos2d::Vec3(0, rotationY, 0);
				_sprite3d->setRotation3D(rotation);
			}
			break;
		case State::Aiming:
			
			if (_currentTarget && _currentTarget->isAlive())
			{
				runAnimation("objects/Aiming.c3b", true);

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
			if (_currentTarget && _currentTarget->isAlive())
			{
				if (_weapon->hasAmmo())
				{
					if (_weapon->isReady())
					{
						float random = CCRANDOM_0_1();
						bool hit = random <= _descriptor->accuracy
								   && random <= _weapon->descriptor->accuracy
								   && random >= _currentTarget->_descriptor->dexterity;
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
				if (_currentTarget && _currentTarget->isAlive())
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

	_sprite3d->stopAllActions();

	Animation3D* animation = Animation3D::create(name);
	Animate3D* animate = Animate3D::create(animation);

	if (repeat)
	{
		_sprite3d->runAction(RepeatForever::create(animate));
	}
	else
	{
		_sprite3d->runAction(animate);
	}

	_currentAnimation = name;
}
