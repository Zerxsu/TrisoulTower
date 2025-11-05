#include "GE_DashCost.h"

#include "TrisoulTower/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"

UGE_DashCost::UGE_DashCost()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModInfo;
	ModInfo.Attribute = UBasicAttributeSet::GetStaminaAttribute();
	ModInfo.ModifierOp = EGameplayModOp::AddBase;
	ModInfo.ModifierMagnitude = FScalableFloat(-20.f);

	Modifiers.Add(ModInfo);
}
