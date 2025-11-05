#include "BasicAttributeSet.h"
#include "GameplayEffectExtension.h"

UBasicAttributeSet::UBasicAttributeSet()
{
	// assigning default values
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
}

// function makes sure that the attributes don't go under or over min and max values
void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

// function executes at the end of every gameplay effect
void UBasicAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth());
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(GetStamina());
	}
}
