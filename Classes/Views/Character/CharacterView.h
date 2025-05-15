#pragma once

#include "Descriptors/CharacterDescriptor.h"
#include "Descriptors/WeaponDescriptor.h"
#include <string>

struct CharacterView
{
	CharacterView(
		const std::string& model,
		const std::string& texture,
		CharacterDescriptor* characterDescriptor,
		WeaponDescriptor* weaponDescriptor,
		const std::string& armorNodeAdress,
		const std::string& helthNodeAdress) :
			model(model),
			texture(texture),
			characterDescriptor(characterDescriptor),
			weaponDescriptor(weaponDescriptor),
			armorBar(armorNodeAdress),
			helthBar(helthNodeAdress)
	{
	}

	std::string model;
	std::string texture;
	CharacterDescriptor* characterDescriptor;
	WeaponDescriptor* weaponDescriptor;
	std::string armorBar;
	std::string helthBar;
};