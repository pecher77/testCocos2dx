#pragma once

class WeaponDescriptor
{
public:
	WeaponDescriptor(
		float damage,
		float accuracy,
		float fireRate,
		int clipSize,
		float muzzleVelocity,
		float reloadTime)
	{
		this->damage = damage;
		this->accuracy = accuracy;
		this->fireRate = fireRate;
		this->clipSize = clipSize;
		this->muzzleVelocity = muzzleVelocity;
		this->reloadTime = reloadTime;
	}

	float damage;
	float accuracy;
	float fireRate;
	int clipSize;
	float muzzleVelocity;
	float reloadTime;
};