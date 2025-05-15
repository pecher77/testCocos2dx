#pragma once

#include "Descriptors/CharacterDescriptor.h"
#include "Models/Battlefield/Battlefield.h"
#include "Models/Battlefield/TargetLocator.h"
#include "Models/Weapon/Weapon.h"
#include "cocos2d.h"
#include <string>

//Команда держит в себе персонажей и умеет проверять, все ли мертвы
class Team
{
public:
	Team(int id);

	void addCharacter(Character::Ptr character);
	bool isCharacterExistInTeam(int id);
	
	bool isTeamKilled();

	std::vector<Character::Ptr> getAliveCharacters();

	void update(float deltaTime);

	std::vector<Character::Ptr> getCharacters() const { return _characters; }
	Character::Ptr getCharacterById(int id);

	int getId() const { return _id; }

private:
	std::vector<Character::Ptr> _characters;

	int _id = -1;
};