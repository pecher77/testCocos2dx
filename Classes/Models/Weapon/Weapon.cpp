#include "Weapon.h"

#include "Views/Bullet/BulletView.h"
#include "cocos2d.h"

#include "Descriptors/WeaponDescriptor.h"

USING_NS_CC;

Weapon::Weapon(WeaponDescriptor* weapon, cocos2d::Sprite3D* owner, cocos2d::Node* root, int charId) : descriptor(weapon), _owner(owner)
			, _root(root), _ammo(descriptor->clipSize), _characterId(charId)
{
}

bool Weapon::hasAmmo() const
{
	return _ammo > 0;
}

bool Weapon::isReady() const
{
	return _ready;
}

void Weapon::reload()
{
	_ammo = descriptor->clipSize;
}

void Weapon::update(float deltaTime)
{
	if (!_ready)
	{
		if (_time > 0)
		{
			_time -= deltaTime;
		}
		else
		{
			_ready = true;
		}
	}
}

void Weapon::fire(Character::Ptr character, bool hit)
{
	if (_ammo > 0)
	{
		_ammo -= 1;
		_time = 1.0f / descriptor->fireRate;
		_ready = false;

		Skeleton3D* skeleton = _owner->getSkeleton();
		Bone3D* rightHandBone = skeleton->getBoneByName("RightHand");
		Mat4 worldMat = rightHandBone->getWorldMat();
		cocos2d::Vec3 worldPosition;
		worldMat.transformPoint(&worldPosition);
		Vec3 position = _owner->getPosition3D() + worldPosition * _owner->getScale();
		Sprite3D* view = Sprite3D::create("objects/bullet.c3b", "objects/bullet.png");

		if (descriptor->needApplyColor)
		{
			view->setColor(descriptor->bulletColor);
		}
		
		view->setScale(view->getScale() * descriptor->bulletSizeMultiplier);

		BulletView* bullet = BulletView::create(this, character, hit);

		bullet->addChild(view);
		bullet->setPosition3D(position);
		
		view->setPosition3D(cocos2d::Vec3(0, 0, 0));

		_root->addChild(bullet);
	}
}
