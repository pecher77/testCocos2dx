#pragma once
#include "Models/Character/Character.h"

class IModifier
{
public:
	using Ptr = std::shared_ptr<IModifier>;

	enum class Type
	{
		NONE = 0,
		HEALTH,
		ACCURACY,
		DEXTERITY,
		ARMOR,
		AIM_TIME,

		DAMAGE,
		WEAPON_ACCURACY,
		FIRE_RATE,
		CLIP_SIZE,
		MUZZLE_VELOCITY,
		RELOAD_RIME,

		BULLET_SIZE,
		BULLET_COLOR,

		//временные модификаторы
		TIMED_IMMORTALITY,
		TIMED_ACCURACY,
		TIMED_BULLET_SIZE,
		ANY
	};

	IModifier(float value, Type type);
	virtual ~IModifier() = default;

	virtual void apply(Character::Ptr target);
	virtual void cancel() {};

	Type getType() const { return _type; }

	//для временных модификаторов
	virtual bool update(float delta) { return false; }

	float getValue() const { return _value; }
	void setValue(float val) { _value = val; }

	int getId() const { return _id;  }

	bool getNeedManage() const { return _needManage; }
	void setNeedManage(bool value) { _needManage = value; }

	Character::Ptr getTarget() { return _target; }

protected:

	//инициализация модификатора - расчет модификатора(если нужно)
	virtual void init() {};

protected:
	Character::Ptr _target;
	Type _type;
	int _id;
	float _value;
	//нужно ли держать модифаер в менеджере по каким-то причинам, возможно переиодически его снова использовать
	bool _needManage = false;
};