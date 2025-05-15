#pragma once

#include "ModifierManager.h"
#include "Modifiers/OneTimeModifier.h"
#include "Modifiers/BulletColorModifier.h"
#include <random>
#include "TimedModifier.h"

ModifierManager& ModifierManager::instance()
{
	static ModifierManager* _instance = new ModifierManager();
	return *_instance;
}

void ModifierManager::addModifier(IModifier::Ptr mod)
{
	auto type = getType(mod);
	_modifiers[type].push_back(mod);
}

void ModifierManager::removeModifier(IModifier::Ptr mod)
{
	auto type = getType(mod);
	auto it = std::find_if(_modifiers[type].begin(), _modifiers[type].end(), [&mod](auto&& m) {
		return mod->getId() == m->getId();
	});

	if (it != _modifiers[type].end())
	{
		_modifiers[type].erase(it);
	}
}

ModifierType ModifierManager::getType(IModifier::Ptr mod)
{
	for (auto&& pair : _modifiersTypesTable)
	{
		if (std::any_of(pair.second.begin(), pair.second.end(), [mod](auto&& m) {
				return mod->getType() == m;
			}))
		{ 
			return pair.first;
		}
	}
	return ModifierType::NONE;
}

void ModifierManager::update(float deltaTime)
{
	std::vector<IModifier::Ptr> toDelete;
	for (auto&& pair : _modifiers)
	{
		std::for_each(pair.second.begin(), pair.second.end(), [&toDelete, deltaTime](auto&& mod) {
			if (mod->update(deltaTime))
			{
				toDelete.push_back(mod);
			}
		});
	}

	for (auto&& mod : toDelete)
	{
		removeModifier(mod);
	}
}

std::vector<IModifier::Ptr> ModifierManager::getModifiersByType(ModifierType type)
{
	if (_modifiers.find(type) != _modifiers.end())
	{
		return _modifiers[type];
	}
	
	return std::vector<IModifier::Ptr>();
}

std::vector<IModifier::Ptr> ModifierManager::getModifiersForCharacter(Character::Ptr character)
{
	std::vector<IModifier::Ptr> result;

	for (auto&& pair : _modifiers)
	{
		std::for_each(pair.second.begin(), pair.second.end(), [&result, &character](auto&& mod) {
			if (character->getId() == mod->getTarget()->getId())
			{
				result.push_back(mod);
			}
			});
	}

	return result;
}

cocos2d::Color3B getRandomColor() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dist(0, 255);

	return cocos2d::Color3B(dist(gen), dist(gen), dist(gen));
}

IModifier::Ptr ModifierManager::createModifier(IModifier::Type type)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	//если пытаемся сделать любой рандомный
	if (type == IModifier::Type::ANY)
	{
		std::vector<IModifier::Type> allTypes;
		for (auto&& pair : _modifiersTypesTable)
		{
			allTypes.insert(allTypes.end(), pair.second.begin(), pair.second.end());
			std::random_shuffle(allTypes.begin(), allTypes.end());
			type = allTypes.front();
		}
	}

	IModifier::Ptr mod = nullptr;
	bool needAdd = false;

	switch (type)
	{
	
	case IModifier::Type::DEXTERITY:
	{
		std::uniform_real_distribution<float> dist(0.0f, 0.1f);
		mod = std::make_shared<OneTimeModifier>(dist(gen), type);
		break;
	}
	case IModifier::Type::ACCURACY:
	{
		std::uniform_real_distribution<float> dist(0.2f, 0.3f);
		mod = std::make_shared<OneTimeModifier>(dist(gen), type);
		break;
	}
	case IModifier::Type::WEAPON_ACCURACY:
	{
		std::uniform_real_distribution<float> dist(0.5f, 0.95f);
		mod = std::make_shared<OneTimeModifier>(dist(gen), type);
		break;
	}
	case IModifier::Type::FIRE_RATE:
	{
		std::uniform_real_distribution<float> dist(10.f, 50.f);
		mod = std::make_shared<OneTimeModifier>(dist(gen), type);
		break;
	}
	case IModifier::Type::HEALTH:
	case IModifier::Type::ARMOR:
	{
		std::uniform_real_distribution<float> dist(0.f, 10.f);
		mod = std::make_shared<OneTimeModifier>(dist(gen), type);
		break;
	}
	case IModifier::Type::AIM_TIME:
	{
		std::uniform_real_distribution<float> dist(1.f, 5.f);
		mod = std::make_shared<OneTimeModifier>(dist(gen), type);
		break;
	}
	case IModifier::Type::DAMAGE:
	{
		std::uniform_real_distribution<float> dist(10.f, 50.f);
		mod = std::make_shared<OneTimeModifier>(dist(gen), type);
		break;
	}
	case IModifier::Type::MUZZLE_VELOCITY:
	{
		std::uniform_real_distribution<float> dist(20.f, 70.f);
		mod = std::make_shared<OneTimeModifier>(dist(gen), type);
		break;
	}
	case IModifier::Type::CLIP_SIZE:
	case IModifier::Type::RELOAD_RIME:
	case IModifier::Type::BULLET_SIZE:
	{
		std::uniform_real_distribution<float> dist(1.f, 3.f);
		mod = std::make_shared<OneTimeModifier>(dist(gen), type);
		break;
	}
	case IModifier::Type::BULLET_COLOR:
		mod = std::make_shared<BulletColorModifier>(getRandomColor(), type);
		break;
	case IModifier::Type::TIMED_IMMORTALITY:
	{
		std::uniform_real_distribution<float> dist(0.f, 10.f);
		mod = std::make_shared<TimedModifier>(type, dist(gen));
		needAdd = true;
		break;
	}
	case IModifier::Type::TIMED_ACCURACY:
	case IModifier::Type::TIMED_BULLET_SIZE:
	{
		std::uniform_real_distribution<float> dist(1.f, 5.f);
		mod = std::make_shared<TimedModifier>(dist(gen), type, 20.f);
		needAdd = true;
		break;
	}
	default:
		break;
	}

	if (needAdd || (mod && mod->getNeedManage()))
	{
		addModifier(mod);
	}

	return mod;
}

std::vector<IModifier::Ptr> ModifierManager::createRandomModifiers(ModifierType type, int count)
{
	if (_modifiersTypesTable.find(type) == _modifiersTypesTable.end())
	{
		return std::vector<IModifier::Ptr>();
	}

	std::random_shuffle(_modifiersTypesTable[type].begin(), _modifiersTypesTable[type].end());

	std::vector<IModifier::Ptr> result;
	
	count = count > _modifiersTypesTable[type].size() ? _modifiersTypesTable[type].size() : count;

	for (size_t i = 0; i < count; ++i)
	{
		if (auto mod = createModifier(_modifiersTypesTable[type][i]))
		{
			result.push_back(createModifier(_modifiersTypesTable[type][i]));
		}
	}

	return result;
}

void ModifierManager::clear()
{
	_modifiers.clear();
}
