#pragma once

#include "Models/Battlefield/Battlefield.h"
#include "Views/Character/CharacterView.h"
#include "cocos2d.h"
#include <vector>

class Bootstrapper : public cocos2d::Scene
{
public:
	enum class GameState : uint8_t {
		INIT = 0,
		PLAY,
		PAUSE,
		END
	};

	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void update(float delta) override;

	void play();
	void onEnd();
	void showMenu(bool onStart = true);

	CREATE_FUNC(Bootstrapper);

private:
	int getTeam(const std::string& name);
	std::map<int, std::vector<cocos2d::Vec3>> getTeamSpawnPoints(cocos2d::Node* node);
	std::vector<CharacterView> getCharacterViews();
	void tryRemoveMenu();

private:
	Battlefield* _battlefield;
	cocos2d::Camera* _camera;

	GameState _state = GameState::INIT;
	//кеш рут ноды
	Node* _rootNode = nullptr;
	cocos2d::Menu* _menu = nullptr;

	Node* _armorBarPrototype = nullptr;
	Node* _helthBarPrototype = nullptr;

	const std::string& CONTINUE_BTN_IMG_RES = "ui/TEX_button_continue.png";
	const std::string& REPLAY_BTN_IMG_RES = "ui/TEX_button_replay.png";
};
