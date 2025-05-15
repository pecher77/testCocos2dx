#pragma once

#include "cocos2d.h"
#include "Models/Character/Character.h"

class WeaponDescriptor;
class Weapon
{
public:
	Weapon(WeaponDescriptor* descriptor, cocos2d::Sprite3D* owner, cocos2d::Node* root, int charId);

	WeaponDescriptor* descriptor;

	bool hasAmmo() const;
	bool isReady() const;

	void reload();
	void update(float deltaTime);
	void fire(Character::Ptr character, bool hit);

	int _ammo;
	bool _ready;
	float _time;
	cocos2d::Sprite3D* _owner;
	cocos2d::Node* _root;
	int _characterId;
};
