#include "Team.h"

#include "Models/Battlefield/TargetLocator.h"
#include "Models/Weapon/Weapon.h"
#include "cocos2d.h"
#include <numeric>

USING_NS_CC;

Team::Team(int id) : _id(id)
{
}

void Team::update(float deltaTime)
{
	for (auto&& ch : _characters)
	{
		ch->update(deltaTime);
	}
}

Character::Ptr Team::getCharacterById(int id)
{
	auto it = std::find_if(_characters.begin(), _characters.end(), [id](auto&& ch) {
		return id == ch->getId();
		});

	if (it != _characters.end())
	{
		return *it;
	}
	return Character::Ptr();
}


void Team::addCharacter(Character::Ptr character)
{
	if (!isCharacterExistInTeam(character->getId()))
	{
		_characters.push_back(character);
	}
	else
	{
		//return Assert(false);
	}
}

bool Team::isCharacterExistInTeam(int id)
{
	return std::any_of(_characters.begin(), _characters.end(), [id](auto&& ch){
		return id == ch->getId();
	});
}

bool Team::isTeamKilled()
{
	return getAliveCharacters().size() == 0;
}

std::vector<Character::Ptr> Team::getAliveCharacters()
{
	std::vector<Character::Ptr> alive;
	std::copy_if(_characters.begin(), _characters.end(), std::back_inserter(alive),[](auto&& ch) 
		{ 
			return ch->isAlive(); 
		});

	return alive;
}

