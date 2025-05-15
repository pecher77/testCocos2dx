#pragma once

#include "Descriptors/CharacterDescriptor.h"
#include "Models/Battlefield/Battlefield.h"
#include "Models/Battlefield/TargetLocator.h"
#include "Models/Weapon/Weapon.h"
#include "cocos2d.h"
#include <string>

class Weapon;
class Character
{
public:
	Character(
		cocos2d::Sprite3D* view,
		CharacterDescriptor* descriptor,
		Weapon* weapon,
		TargetLocator* targetLocator);

	enum class State
	{
		Idle,
		Aiming,
		Shooting,
		Reloading
	};

	std::string _currentAnimation;
	cocos2d::Sprite3D* Sprite3d;
	CharacterDescriptor* descriptor;
	float health;
	float armor;

	bool isAlive() const;
	void update(float deltaTime);
	void runAnimation(std::string name, bool repeat, bool force = false);
	cocos2d::Vec3 getPosition() const;

private:
	Weapon* _weapon;
	Character* _currentTarget;
	TargetLocator* _targetLocator;

	State _state;
	float _time;
};