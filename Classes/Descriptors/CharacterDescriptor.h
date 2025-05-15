#pragma once

class CharacterDescriptor
{
public:
	CharacterDescriptor(
		float accuracy,
		float dexterity,
		float maxHealth,
		float maxArmor,
		float aimTime)
	{
		this->accuracy = accuracy;
		this->dexterity = dexterity;
		this->maxHealth = maxHealth;
		this->maxArmor = maxArmor;
		this->aimTime = aimTime;
	}

	float accuracy;
	float dexterity;
	float maxHealth;
	float maxArmor;
	float aimTime;
};