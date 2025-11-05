#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BasicAttributeSet.generated.h"

/**
 * 
 */

// helper function for getting and setting
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class TRISOULTOWER_API UBasicAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBasicAttributeSet();
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, MaxStamina);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
