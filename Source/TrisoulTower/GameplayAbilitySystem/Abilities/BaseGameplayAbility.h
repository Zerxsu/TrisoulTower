#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

UCLASS()
class TRISOULTOWER_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability UI")
	UTexture2D* AbilityIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability UI")
	FLinearColor AbilityColor = FLinearColor::White;
};
