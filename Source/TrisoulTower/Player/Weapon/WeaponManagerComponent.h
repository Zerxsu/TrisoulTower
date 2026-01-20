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
	UFUNCTION()
	void EquipWeapon(TSubclassOf<AWeapon_Base> WeaponToEquip);

	UFUNCTION()
	void UnequipWeapon();

	UFUNCTION()
	AWeapon_Base* GetEquippedWeapon();

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
