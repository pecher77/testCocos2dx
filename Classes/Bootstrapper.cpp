#include "Bootstrapper.h"

#include "Views/Character/CharacterView.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>

USING_NS_CC;

Scene* Bootstrapper::createScene()
{
	return Bootstrapper::create();
}

bool Bootstrapper::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Node* rootNode = cocos2d::CSLoader::createNode("ScorewarriorCS/res/MainScene.csb");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Camera* camera= Camera::createPerspective(60, (float)visibleSize.width / visibleSize.height, 10, 10000);
	camera->setPosition3D(Vec3(-50, 500, -300));
	camera->lookAt(Vec3(0, 0, 0));
	camera->setCameraMask(1);

	Sprite3D* sprite3d = Sprite3D::create("ScorewarriorCS/res/environment_MESH.c3b", "ScorewarriorCS/res/environment_tex.png");
	sprite3d->setPosition3D(Vec3(0, 0, 0));
	sprite3d->setScale(0.25);

	this->addChild(camera);
	this->addChild(rootNode);
	this->addChild(sprite3d);

	std::map<int, std::vector<cocos2d::Vec3>> spawnPointPositions = getTeamSpawnPoints(rootNode);
	std::vector<CharacterView> characters = getCharacterViews();

	_battlefield = new Battlefield(rootNode, spawnPointPositions);
	_battlefield->start(characters);

	this->scheduleUpdate();

	return true;
}

void Bootstrapper::update(float delta)
{
	_battlefield->update(delta);
}

int Bootstrapper::getTeam(const std::string& name)
{
	std::regex regex("Team(\\d+)");
	std::smatch match;

	if (std::regex_search(name, match, regex) && match.size() > 1)
	{
		return std::stoul(match.str(1));
	}

	throw std::invalid_argument("No team number found in the string");
}

std::map<int, std::vector<cocos2d::Vec3>> Bootstrapper::getTeamSpawnPoints(cocos2d::Node* node)
{
	std::map<int, std::vector<cocos2d::Vec3>> spawnPositionsByTeam;

	for (const auto& child : node->getChildren())
	{
		std::string childName = child->getName();

		if (childName.find("SpawnPoint") == std::string::npos)
		{
			continue;
		}

		int team = getTeam(childName);
		auto& spawnPoints = spawnPositionsByTeam[team];

		spawnPoints.push_back(child->getPosition3D());
	}

	return spawnPositionsByTeam;
}

std::vector<CharacterView> Bootstrapper::getCharacterViews()
{
	std::vector<CharacterView> characterViews;

	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_1.png",
		 new CharacterDescriptor(0.8, 0.1, 100, 200, 2),
		 new WeaponDescriptor(20, 0.5, 1, 2, 20, 4)});
	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_2.png",
		 new CharacterDescriptor(0.7, 0.25, 100, 100, 3.5),
		 new WeaponDescriptor(10, 0.9, 0.4, 10, 60, 5)});
	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_3.png",
		 new CharacterDescriptor(0.95, 0.1, 100, 150, 4),
		 new WeaponDescriptor(12, 0.85, 0.5, 4, 60, 6)});
	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_4.png",
		 new CharacterDescriptor(0.85, 0.2, 100, 100, 2.5),
		 new WeaponDescriptor(6, 0.7, 3, 30, 30, 5)});
	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_5.png",
		 new CharacterDescriptor(0.85, 0.2, 100, 100, 2),
		 new WeaponDescriptor(8, 0.65, 3, 30, 30, 4)});
	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_6.png",
		 new CharacterDescriptor(0.95, 0.15, 100, 100, 3.5),
		 new WeaponDescriptor(15, 0.95, 0.1, 5, 60, 6)});

	return characterViews;
}