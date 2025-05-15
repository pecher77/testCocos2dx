#pragma once

class cocos2d::Color3B;

class WeaponDescriptor
{
public:
	//список инициализации, чтобы изюежать двойного копирования
	WeaponDescriptor(
		float damage_,
		float accuracy_,
		float fireRate_,
		int clipSize_,
		float muzzleVelocity_,
		float reloadTime_,
		cocos2d::Color3B bulletColor_,
		bool needApplyColor_,
		float bulletSizeMultiplier_
		) : damage(damage_), accuracy(accuracy_), fireRate(fireRate_)
			, clipSize(clipSize_), muzzleVelocity(muzzleVelocity_), reloadTime(reloadTime_)
			, bulletColor(bulletColor_), needApplyColor(needApplyColor_), bulletSizeMultiplier(bulletSizeMultiplier_)
	{
	}

	float damage;
	float accuracy;
	float fireRate;
	int clipSize;
	float muzzleVelocity;
	float reloadTime;
	cocos2d::Color3B bulletColor = {255, 255, 255};
	bool needApplyColor;
	float bulletSizeMultiplier = 1.f;
};