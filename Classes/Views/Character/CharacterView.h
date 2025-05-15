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
		WeaponDescriptor* weaponDescriptor) :
			model(model),
			texture(texture),
			characterDescriptor(characterDescriptor),
			weaponDescriptor(weaponDescriptor)
	{
	}

	std::string model;
	std::string texture;
	CharacterDescriptor* characterDescriptor;
	WeaponDescriptor* weaponDescriptor;
};