#include "Battlefield.h"
#include "Models/Weapon/Weapon.h"
#include "ui/CocosGUI.h"
#include "Broadcaster/Broadcaster.h"
#include "cocostudio/CocoStudio.h"
#include "Modifiers/ModifierManager.h"

USING_NS_CC;

Battlefield::Battlefield(cocos2d::Node* root, std::map<int, std::vector<cocos2d::Vec3>> map) : _root(root), _spawnPositionsByTeam(map)
{
	Broadcaster::instance().subscribeForMessage("onCharactersDeath", [this](auto&& event) 
		{
			auto characterId = static_cast<int*>(event->getUserData());
			onCharactersDeath(*characterId);
		});

	Broadcaster::instance().subscribeForMessage("onEnd", [this](auto&& event)
		{
			_paused = true;
		});

	Broadcaster::instance().subscribeForMessage("onCharacterDidKill", [this](auto&& event)
		{
			auto shooter = static_cast<int*>(event->getUserData());
		});
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

		_charactersByTeam[positionsPair.first] = std::make_shared<Team>(positionsPair.first);
		
		int i = 0;
		while (i < positions.size() && !availablePrefabs.empty())
		{
			std::uniform_int_distribution<int> uni(0, availablePrefabs.size() - 1);
			int index = uni(rng);
			_charactersByTeam[positionsPair.first]->addCharacter(createCharacterAt(availablePrefabs[index], *this, positions[i], positionsPair.first));
			availablePrefabs.erase(availablePrefabs.begin() + index);
			i++;
		}
	}

	addModifiersToCharactersOnStart();
}

void Battlefield::restart(std::vector<CharacterView> prefabs)
{
	_root->removeAllChildren();
	_charactersByTeam.clear();
	ModifierManager::instance().clear();
	start(prefabs);
}

void Battlefield::update(float delta)
{
	if (!_paused)
	{
		for (auto& pair : _charactersByTeam)
		{
			pair.second->update(delta);
		}
	}
}

void Battlefield::onCharactersDeath(int id)
{
	int teamId = getTeam(id);
	if (_charactersByTeam[teamId]->isTeamKilled())
	{
		onTeamsDeath(teamId);
	}


	std::string text = "PLayer" + std::to_string(id) + " was killed";

	auto label = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 32);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_root->addChild(label);

	auto delay = DelayTime::create(1.0f);
	auto remove = RemoveSelf::create();  

	label->runAction(Sequence::create(delay, remove, nullptr));
}

void Battlefield::onCharacterDidKill(int id)
{
	int teamId = getTeam(id);
	if (auto&& character = _charactersByTeam[teamId]->getCharacterById(id))
	{
		applyRandomModifier(character);
	}
}


void Battlefield::onTeamsDeath(int teamId)
{
	Broadcaster::instance().broadcastMessage("onTeamsDeath");
}

void Battlefield::applyRandomModifier(Character::Ptr character)
{
	auto mod = ModifierManager::instance().createModifier(IModifier::Type::ANY);
	mod->apply(character);
}

Character::Ptr Battlefield::createCharacterAt(
	const CharacterView& prefab,
	Battlefield& battlefield,
	const cocos2d::Vec3& position,
	int teamId)
{
	static int characterId = -1;
	++characterId;

	Sprite3D* sprite3d = Sprite3D::create(prefab.model, prefab.texture);
	Weapon* weapon = new Weapon(prefab.weaponDescriptor, sprite3d, _root, characterId);

	auto character = std::shared_ptr<Character>(new Character(characterId, teamId, sprite3d, prefab.characterDescriptor, weapon, &battlefield));

	_root->addChild(sprite3d);

	sprite3d->setPosition3D(position);
	sprite3d->setScale(0.3);

	return character;
}

void Battlefield::addModifiersToCharactersOnStart()
{
	for (auto&& pair : _charactersByTeam)
	{
		auto&& characters = pair.second->getCharacters();
		std::for_each(characters.begin(), characters.end(), [](auto&& character)
			{
				//получаем рандомные модификаторы и применяем их
				auto characterModifiers = ModifierManager::instance().createRandomModifiers(ModifierType::CHARACTER, 3);
				auto weaponModifiers = ModifierManager::instance().createRandomModifiers(ModifierType::WEAPON, 2);
				auto bulletModifiers = ModifierManager::instance().createRandomModifiers(ModifierType::BULLET, 1);

				std::for_each(characterModifiers.begin(), characterModifiers.end(), [character](auto&& mod) {
					mod->apply(character);
					});

				std::for_each(weaponModifiers.begin(), weaponModifiers.end(), [character](auto&& mod) {
					mod->apply(character);
					});

				std::for_each(bulletModifiers.begin(), bulletModifiers.end(), [character](auto&& mod) {
					mod->apply(character);
					});
			});
	}
	
}

Character::Ptr Battlefield::getNearestAliveTarget(Character::Ptr character)
{
	int teamId = getTeam(character->getId());
	Character::Ptr nearestEnemy = nullptr;
	float nearestDistance = FLT_MAX;
	//в теории, если команд не 2, то лучше бы собрать всех врагов со всех остальных команд

	std::vector<Character::Ptr> enemies;
	for (auto&& teamPair : _charactersByTeam)
	{
		if (teamPair.first != teamId)
		{
			auto&& characters = teamPair.second->getAliveCharacters();
			enemies.insert(enemies.end(), characters.begin(), characters.end());
		}
	}

	for (auto&& enemy : enemies)
	{
		float distance = (character->getPosition() - enemy->getPosition()).length();
		if (distance < nearestDistance)
		{
			nearestDistance = distance;
			nearestEnemy = enemy;
		}
	}

	return nearestEnemy;
}

int Battlefield::getTeam(int target)
{
	for (auto&& teamPair : _charactersByTeam)
	{
		if (teamPair.second->isCharacterExistInTeam(target))
		{
			return teamPair.second->getId();
		}
	}
	return 0;
}