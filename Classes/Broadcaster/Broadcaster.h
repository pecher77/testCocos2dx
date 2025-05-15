#pragma once

#include "cocos2d.h"
#include <functional>

class cocos2d::EventListenerCustom;
class cocos2d::EventDispatcher;

class Broadcaster
{
public:
	static Broadcaster& instance();

	void init(cocos2d::EventDispatcher* dispatcher, cocos2d::Scene* scene);

	void subscribeForMessage(const std::string msg, const std::function<void(cocos2d::EventCustom*)> callback);
	void broadcastMessage(const std::string msg);
	void broadcastMessage(cocos2d::EventCustom* event);


protected:
	Broadcaster() = default;
	~Broadcaster() = default;

	Broadcaster(const Broadcaster&) = delete;
	Broadcaster& operator=(const Broadcaster&) = delete;

	cocos2d::EventDispatcher* _eventDispatcher = nullptr;

	cocos2d::Scene* _scene;

	bool _inited = false;
};

