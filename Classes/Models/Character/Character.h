#pragma once


#include "Descriptors/CharacterDescriptor.h"
#include "cocos2d.h"
#include <string>
#include <memory>

class Weapon;
class Team;
class TargetLocator;
class cocos2d::Node;

//использую std::enable_shared_from_this, т.к. в методе update отправляю this в функцию, которая ожидает shared_ptr, но 
//данный объект уже был ранее обернут в shared_ptr(лежит в Team)
class Character : public std::enable_shared_from_this<Character>
{
public:
	using Ptr = std::shared_ptr<Character>;
	using WeakPtr = std::weak_ptr<Character>;

	Character(
		int id,
		int teamId,
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

	bool isAlive() const;
	void update(float deltaTime);
	void runAnimation(std::string name, bool repeat, bool force = false);
	cocos2d::Vec3 getPosition() const;

	void receiveDamage(float damage, int shooterId);

	int getId() const { return _id; }

	//для доступа модификаторов
	CharacterDescriptor* getCharacterDescriptor() { return _descriptor; }
	float getHealth() const { return _health; }
	void setHealth(float value) { _health = value;  }
	float getArmor() const { return _armor; }
	void setArmor(float value) { _armor = value; }
	Weapon* getWeapon() { return _weapon; }

	void applyImmortality();
	void cancelImmortality();

private:
	void onDeath(int shooterId);
	void updateArmorProgress();
	void updateHealthProgress();

private:
	//пока просто инт для простоты
	int _id = -1;
	int _teamId = -1;

	float _health;
	float _armor;

	std::string _currentAnimation;

	Weapon* _weapon;
	Ptr _currentTarget = nullptr;
	TargetLocator* _targetLocator;

	//добавил обратную зависимость на команду, чтобы оповестить ее при смерти, а не проверять на каждом апдейте в команде все ли мертвы

	cocos2d::Sprite3D* _sprite3d;
	CharacterDescriptor* _descriptor;
	cocos2d::Sprite* _armorBar;
	cocos2d::Sprite* _helthBar;

	bool _immortality = false;

	State _state;
	float _time;
};
