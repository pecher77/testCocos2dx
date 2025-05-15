#include "IModifier.h"

enum class ModifierType
{
	NONE = 0,
	CHARACTER,
	WEAPON,
	BULLET,
	ANY
};

class ModifierManager
{
public:
	static ModifierManager& instance();

	void addModifier(IModifier::Ptr mod);
	void removeModifier(IModifier::Ptr mod);

	ModifierType getType(IModifier::Ptr mod);

	void update(float deltaTime);

	std::vector<IModifier::Ptr> getModifiersByType(ModifierType type);
	std::vector<IModifier::Ptr> getModifiersForCharacter(Character::Ptr character);


	std::vector<IModifier::Ptr> createRandomModifiers(ModifierType type, int count);
	IModifier::Ptr createModifier(IModifier::Type type);

	void clear();

protected:
	ModifierManager() = default;
	~ModifierManager() = default;

	ModifierManager(const ModifierManager&) = delete;
	ModifierManager& operator=(const ModifierManager&) = delete;

private:
	std::map<ModifierType, std::vector<IModifier::Ptr>> _modifiers;

	//таблица соответствий, могла бы храниться где-то в ассете для гибкости добавления
	std::map<ModifierType, std::vector<IModifier::Type>> _modifiersTypesTable = {
		{ModifierType::CHARACTER, {	IModifier::Type::HEALTH,
									IModifier::Type::ACCURACY,
									IModifier::Type::DEXTERITY,
									IModifier::Type::ARMOR,
									IModifier::Type::AIM_TIME,
									IModifier::Type::TIMED_IMMORTALITY,
									IModifier::Type::TIMED_ACCURACY
	}},
		{ModifierType::WEAPON,	{	IModifier::Type::DAMAGE,
									IModifier::Type::WEAPON_ACCURACY,
									IModifier::Type::FIRE_RATE,
									IModifier::Type::CLIP_SIZE,
									IModifier::Type::MUZZLE_VELOCITY,
									IModifier::Type::RELOAD_RIME}},
		{ModifierType::BULLET, {	IModifier::Type::BULLET_SIZE,
									IModifier::Type::BULLET_COLOR,
									IModifier::Type::TIMED_BULLET_SIZE}}
	};
};