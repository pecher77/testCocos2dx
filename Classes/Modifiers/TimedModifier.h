#pragma once
#include "IModifier.h"

//модификатор действующий, определенное время, после него прекращает действие
class TimedModifier : public IModifier
{
public:
	TimedModifier(float value, Type type, float time);
	TimedModifier(Type type, float time);

	void apply(Character::Ptr target) override;
	bool update(float delta) override;
	void cancel() override;

protected:
	float _time;
	bool _applyed = false;
	float _oldValue;
};