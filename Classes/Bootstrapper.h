#pragma once

#include "Models/Battlefield/Battlefield.h"
#include "Views/Character/CharacterView.h"
#include "cocos2d.h"
#include <vector>

class Bootstrapper : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void update(float delta) override;

	CREATE_FUNC(Bootstrapper);

private:
	Battlefield* _battlefield;
	cocos2d::Camera* _camera;

	int getTeam(const std::string& name);
	std::map<int, std::vector<cocos2d::Vec3>> getTeamSpawnPoints(cocos2d::Node* node);
	std::vector<CharacterView> getCharacterViews();
};
