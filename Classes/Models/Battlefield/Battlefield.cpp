#include "Battlefield.h"
#include "Models/Character/Character.h"
#include "Views/Character/CharacterView.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Battlefield::Battlefield(cocos2d::Node* root, std::map<int, std::vector<cocos2d::Vec3>> map)
{
	_root = root;
	_spawnPositionsByTeam = map;
}

void Battlefield::start(std::vector<CharacterView> prefabs)
{
	_paused = false;
	_charactersByTeam.clear();

	std::vector<CharacterView> availablePrefabs(prefabs);
	std::random_device rd;
	std::mt19937 rng(rd());

	for (auto& positionsPair : _spawnPositionsByTeam)
	{
		std::vector<Vec3>& positions = positionsPair.second;
		std::vector<Character*> characters;
		int i = 0;
		while (i < positions.size() && !availablePrefabs.empty())
		{
			std::uniform_int_distribution<int> uni(0, availablePrefabs.size() - 1);
			int index = uni(rng);
			characters.push_back(createCharacterAt(availablePrefabs[index], *this, positions[i]));
			availablePrefabs.erase(availablePrefabs.begin() + index);
			i++;
		}
		_charactersByTeam[positionsPair.first] = characters;
	}
}

void Battlefield::update(float delta)
{
	if (!_paused)
	{
		for (auto& team : _charactersByTeam)
		{
			for (auto& character : team.second)
			{
				character->update(delta);
			}
		}
	}
}

Character* Battlefield::createCharacterAt(
	const CharacterView& prefab,
	Battlefield& battlefield,
	const cocos2d::Vec3& position)
{
	Sprite3D* sprite3d = Sprite3D::create(prefab.model, prefab.texture);
	Weapon* weapon = new Weapon(prefab.weaponDescriptor, sprite3d, _root);
	Character* character= new Character(sprite3d, prefab.characterDescriptor, weapon, &battlefield);

	_root->addChild(sprite3d);

	sprite3d->setPosition3D(position);
	sprite3d->setScale(0.3);

	return character;
}

Character* Battlefield::getNearestAliveTarget(Character* character)
{
	int team = getTeam(character);
	Character* nearestEnemy = nullptr;
	float nearestDistance = FLT_MAX;
	std::vector<Character*> enemies = team == 1 ? _charactersByTeam[2] : _charactersByTeam[1];
	for (Character* enemy : enemies)
	{
		if (enemy->isAlive())
		{
			float distance = (character->getPosition() - enemy->getPosition()).length();
			if (distance < nearestDistance)
			{
				nearestDistance = distance;
				nearestEnemy = enemy;
			}
		}
	}

	return nearestEnemy;
}

int Battlefield::getTeam(Character* target)
{
	for (auto& charactersPair : _charactersByTeam)
	{
		for (Character* character : charactersPair.second)
		{
			if (character == target)
			{
				return charactersPair.first;
			}
		}
	}
	return 0;
}