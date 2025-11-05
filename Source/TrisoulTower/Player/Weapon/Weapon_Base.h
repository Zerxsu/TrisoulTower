#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "Weapon_Base.generated.h"

USTRUCT()
struct FWeapon
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UGameplayAbility*> Abilities;
};

UCLASS()
class TRISOULTOWER_API AWeapon_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon_Base();

};
