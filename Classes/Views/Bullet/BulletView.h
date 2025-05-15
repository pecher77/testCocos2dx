#include "Models/Character/Character.h"

class BulletView : public cocos2d::Node
{
public:
	bool init(Weapon* weapon, Character* target, bool hit);
	void update(float deltaTime);

	static BulletView* create(Weapon* weapon, Character* target, bool hit)
	{
		BulletView* pRet = new (std::nothrow) BulletView();
		if (pRet && pRet->init(weapon, target, hit))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}

private:
	Weapon* _weapon;
	Character* _target;
	bool _hit;
};