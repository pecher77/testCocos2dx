#include "Bootstrapper.h"

#include "Views/Character/CharacterView.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "Broadcaster/Broadcaster.h"
#include "Modifiers/ModifierManager.h"

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

	_rootNode = cocos2d::CSLoader::createNode("ScorewarriorCS/res/MainScene.csb");
	if (!_rootNode)
	{
		return false;
	}

	Broadcaster::instance().init(getEventDispatcher(), this);
	Broadcaster::instance().subscribeForMessage("onTeamsDeath", [this](auto&&) {
		onEnd();
		});

	Size visibleSize = Director::getInstance()->getVisibleSize();
	_camera = Camera::createPerspective(60, (float)visibleSize.width / visibleSize.height, 10, 10000);
	_camera->setPosition3D(Vec3(-50, 500, -300));
	_camera->lookAt(Vec3(0, 0, 0));
	_camera->setCameraMask(1);

	Sprite3D* sprite3d = Sprite3D::create("ScorewarriorCS/res/environment_MESH.c3b", "ScorewarriorCS/res/environment_tex.png");
	sprite3d->setPosition3D(Vec3(0, 0, 0));
	sprite3d->setScale(0.25);

	this->addChild(_camera);
	this->addChild(_rootNode);
	this->addChild(sprite3d);
	this->scheduleUpdate();

	showMenu();

	return true;
}

void Bootstrapper::tryRemoveMenu()
{
	if (_menu) {
		_menu->removeFromParent();
		_menu = nullptr;
	}
}

void Bootstrapper::play()
{
	tryRemoveMenu();

	_state = GameState::PLAY;

	std::map<int, std::vector<cocos2d::Vec3>> spawnPointPositions = getTeamSpawnPoints(_rootNode);
	std::vector<CharacterView> characters = getCharacterViews();

	if (!_battlefield)
	{
		_battlefield = new Battlefield(_rootNode, spawnPointPositions);
		_battlefield->start(characters);
	}
	else
	{
		_battlefield->restart(characters);
	}
}

void Bootstrapper::onEnd()
{
	_state == GameState::END;
	
	Broadcaster::instance().broadcastMessage("onEnd");

	showMenu(false);
}

void Bootstrapper::showMenu(bool onStart /*= true*/)
{
	tryRemoveMenu();

	if (onStart)
	{
		_menu = cocos2d::Menu::createWithItem(MenuItemImage::create(CONTINUE_BTN_IMG_RES, CONTINUE_BTN_IMG_RES, CC_CALLBACK_0(Bootstrapper::play, this)));
		this->addChild(_menu, 1);
		return;
	}
	
	_menu = cocos2d::Menu::createWithItem(MenuItemImage::create(REPLAY_BTN_IMG_RES, REPLAY_BTN_IMG_RES, CC_CALLBACK_0(Bootstrapper::play, this)));
	this->addChild(_menu, 1);
}

void Bootstrapper::update(float delta)
{
	if (_battlefield && _state == GameState::PLAY)
	{
		_battlefield->update(delta);
	}

	ModifierManager::instance().update(delta);
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
		 new CharacterDescriptor(0.8, 0.1, 100, 20, 2),
		 new WeaponDescriptor(20, 0.5, 1, 2, 20, 4, {}, false, 1.f),
		"ScorewarriorCS/res/BarsNode.csb",
		""});
	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_2.png",
		 new CharacterDescriptor(0.7, 0.25, 100, 30, 3.5),
		 new WeaponDescriptor(10, 0.9, 0.4, 10, 60, 5, {}, false, 1.f),
		"ScorewarriorCS/res/BarsNode.csb",
		""});
	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_3.png",
		 new CharacterDescriptor(0.95, 0.1, 80, 20, 4),
		 new WeaponDescriptor(12, 0.85, 0.5, 4, 60, 6, {}, false, 1.f),
		"ScorewarriorCS/res/BarsNode.csb",
		""});
	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_4.png",
		 new CharacterDescriptor(0.85, 0.2, 90, 30, 2.5),
		 new WeaponDescriptor(6, 0.7, 3, 30, 30, 5, {}, false, 1.f),
		"ScorewarriorCS/res/BarsNode.csb",
		""});
	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_5.png",
		 new CharacterDescriptor(0.85, 0.2, 100, 20, 2),
		 new WeaponDescriptor(8, 0.65, 3, 30, 30, 4, {}, false, 1.f),
		"ScorewarriorCS/res/BarsNode.csb",
		""});
	characterViews.push_back(
		{"objects/Soldier.c3b",
		 "objects/Soldier_tex_6.png",
		 new CharacterDescriptor(0.95, 0.15, 70, 30, 3.5),
		 new WeaponDescriptor(15, 0.95, 0.1, 5, 60, 6, {}, false, 1.f),
		"ScorewarriorCS/res/BarsNode.csb",
		""});

	return characterViews;
}