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
	UPROPERTY()
	UClass* DefaultAnimClass;
	
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<AWeapon_Base> WeaponToEquip);

	UFUNCTION()
	void UnequipWeapon();

	UFUNCTION()
	AWeapon_Base* GetEquippedWeapon();

	UFUNCTION()
	FGameplayAbilitySpecHandle GetAbility(int AbilityIndex);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	ACharacterController* CharacterRef;

	UPROPERTY()
	AWeapon_Base* EquippedWeapon;

	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;

	FGameplayAbilitySpecHandle ParrySpecHandle;
	FGameplayAbilitySpecHandle GroundSlamSpecHandle;
};
