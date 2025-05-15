#include "Modifiers/IModifier.h"
#include <cocos2d.h>

class BulletColorModifier : public IModifier
{
public:
	BulletColorModifier(cocos2d::Color3B color, Type type);

	void apply(Character::Ptr target) override;

private:
	cocos2d::Color3B _color;
};