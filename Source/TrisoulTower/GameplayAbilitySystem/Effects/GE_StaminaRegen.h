#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_StaminaRegen.generated.h"

UCLASS()
class TRISOULTOWER_API UGE_StaminaRegen : public UGameplayEffect
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
};
