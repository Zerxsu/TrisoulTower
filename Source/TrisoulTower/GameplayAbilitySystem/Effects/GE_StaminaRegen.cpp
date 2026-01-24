#include "GE_StaminaRegen.h"

#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "TrisoulTower/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"

void UGE_StaminaRegen::PostInitProperties()
{
	Super::PostInitProperties();

	// sets stamina regen frequency
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 0.033333;

	// sets stamina regen amount
	FGameplayModifierInfo ModInfo;
	ModInfo.Attribute = UBasicAttributeSet::GetStaminaAttribute();
	ModInfo.ModifierOp = EGameplayModOp::AddBase;
	ModInfo.ModifierMagnitude = FScalableFloat(0.2);
	Modifiers.Add(ModInfo);

	// sets stamina regen tag
	FGameplayTag StaminaRegenTag = FGameplayTag::RequestGameplayTag(TEXT("Status.Stamina.Regen"));
	UTargetTagsGameplayEffectComponent& TargetTagsComponent = FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer TagChanges = TargetTagsComponent.GetConfiguredTargetTagChanges();
	TagChanges.Added.AddTag(StaminaRegenTag);
	TargetTagsComponent.SetAndApplyTargetTagChanges(TagChanges);
}
