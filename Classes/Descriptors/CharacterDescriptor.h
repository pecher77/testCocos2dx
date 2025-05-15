#pragma once

class CharacterDescriptor
{
public:
	//список инициализации, чтобы изюежать двойного копирования
	CharacterDescriptor(
		float accuracy_,
		float dexterity_,
		float maxHealth_,
		float maxArmor_,
		float aimTime_) : accuracy(accuracy_), dexterity(dexterity_), maxHealth(maxHealth_), maxArmor(maxArmor_), aimTime(aimTime_)
	{
	}

	float accuracy;
	float dexterity;
	float maxHealth;
	float maxArmor;
	float aimTime;
};