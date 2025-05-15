#include "Broadcaster.h"

USING_NS_CC;

Broadcaster& Broadcaster::instance()
{
	static Broadcaster* _instance = new Broadcaster();
	return *_instance;
}

void Broadcaster::init(cocos2d::EventDispatcher* dispatcher, cocos2d::Scene* scene)
{
	_eventDispatcher = dispatcher;
	_scene = scene;
	_inited = true;
}

void Broadcaster::subscribeForMessage(const std::string msg, const std::function<void(cocos2d::EventCustom*)> callback)
{
	if (!_inited)
	{
		//error
		return;
	}
	auto listener = EventListenerCustom::create(msg, callback);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _scene);
}

void Broadcaster::broadcastMessage(const std::string msg)
{
	cocos2d::EventCustom event(msg);
	_eventDispatcher->dispatchEvent(&event);
}

void Broadcaster::broadcastMessage(cocos2d::EventCustom* event)
{
	_eventDispatcher->dispatchEvent(event);
}
