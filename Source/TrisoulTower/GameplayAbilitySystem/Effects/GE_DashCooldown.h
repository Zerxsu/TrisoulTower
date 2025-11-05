#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_DashCooldown.generated.h"

UCLASS()
class TRISOULTOWER_API UGE_DashCooldown : public UGameplayEffect
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
};