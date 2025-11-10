#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/ActorComponent.h"
#include "Weapon_Base.h"
#include "WeaponManagerComponent.generated.h"

class ACharacterController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRISOULTOWER_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<AWeapon_Base> WeaponToEquip);

	UFUNCTION(BlueprintCallable)
	void UnequipWeapon();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ACharacterController* CharacterRef;

	UPROPERTY()
	AWeapon_Base* EquippedWeapon;

	UPROPERTY()
	UClass* DefaultAnimClass;

	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;
};
