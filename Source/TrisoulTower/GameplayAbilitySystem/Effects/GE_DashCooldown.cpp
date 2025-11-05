#include "GE_DashCooldown.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

void UGE_DashCooldown::PostInitProperties()
{
	Super::PostInitProperties();

	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(0.2f));

	FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(TEXT("Cooldown.Dash"));

	UTargetTagsGameplayEffectComponent& TargetTagsComponent = FindOrAddComponent<UTargetTagsGameplayEffectComponent>();

	FInheritedTagContainer TagChanges = TargetTagsComponent.GetConfiguredTargetTagChanges();

	TagChanges.Added.AddTag(CooldownTag);

	TargetTagsComponent.SetAndApplyTargetTagChanges(TagChanges);
}