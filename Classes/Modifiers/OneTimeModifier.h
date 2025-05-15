#pragma once
#include "IModifier.h"

//модификатор для "одноразовых" модификаций
class OneTimeModifier : public IModifier
{
public:
	using Ptr = std::shared_ptr<OneTimeModifier>;
	using IModifier::IModifier;

	void apply(Character::Ptr target) override;
};