#pragma once

#include "TargetLocator.h"
#include "Views/Character/CharacterView.h"
#include "Models/Team/Team.h"
#include "math/Vec3.h"
#include <map>
#include <vector>

class Battlefield : TargetLocator
{
public:
	Battlefield(cocos2d::Node* root, std::map<int, std::vector<cocos2d::Vec3>> map);

	void update(float delta);
	void start(std::vector<CharacterView> characters);
	void restart(std::vector<CharacterView> prefabs);

private:
	void onCharactersDeath(int id);
	void onCharacterDidKill(int id);
	void onTeamsDeath(int teamId);

	//работа с модификаторами
	void applyRandomModifier(Character::Ptr character);
	void addModifiersToCharactersOnStart();

	Character::Ptr getNearestAliveTarget(Character::Ptr character) override;
	int getTeam(int target);
	Character::Ptr createCharacterAt(
		const CharacterView& prefab,
		Battlefield& battlefield,
		const cocos2d::Vec3& position,
		int teamId);

private:

	bool _paused;
	cocos2d::Node* _root;
	std::map<int, std::vector<cocos2d::Vec3>> _spawnPositionsByTeam;
	//храним команды, которые апдейтят персонажей
	std::map<int, std::shared_ptr<Team>> _charactersByTeam;
};