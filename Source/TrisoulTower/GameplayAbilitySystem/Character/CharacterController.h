#pragma once

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "TrisoulTower/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TrisoulTower/Player/Weapon/Weapon_Base.h"
#include "CharacterController.generated.h"

class UWeaponManagerComponent;

UCLASS()
class TRISOULTOWER_API ACharacterController : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterController();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	// GameplayAbilitySystem components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerController|GameplayAbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerController|GameplayAbilitySystem")
	UBasicAttributeSet* BasicAttributeSet;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant);

	UFUNCTION(BlueprintCallable)
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove);

	// only used for debugging rn might keep tho
	UFUNCTION(BlueprintCallable)
	void ResetAttackCombo();

	UPROPERTY(BlueprintReadOnly, Category = "PlayerController|Variables")
	bool bIsDashing;

	// handles light attack combo logic
	UPROPERTY(BlueprintReadWrite, Category = "PlayerController|Variables")
	bool bIsAttacking;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerController|Variables")
	int AttackIndex;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerController|Variables")
	TArray<AActor*> EnemiesHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Variables")
	float SprintSpeed = 700.f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|GameplayAbilitySystem|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;

	// TEMP (remove later when implementing ground slam in C++)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|GameplayAbilitySystem|Abilities")
	TSubclassOf<UGameplayAbility> GroundSlamAbility;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerController")
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerController")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputMappingContext* InputContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* LightAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* UltimateAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* EquipWeapon1Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* EquipWeapon2Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Input")
	UInputAction* EquipWeapon3Action;

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Dash();

	UFUNCTION()
	void LightAttack();

	UFUNCTION()
	void HeavyAttack();

	UFUNCTION()
	void UltimateAttack();

	UFUNCTION()
	void EquipWeapon1();
	
	UFUNCTION()
	void EquipWeapon2();

	UFUNCTION()
	void EquipWeapon3();

	void HandleStaminaChanged(const FOnAttributeChangeData& Data);

private:
	UPROPERTY()
	// original walk speed is cached at the start of the game
	float OriginalWalkSpeed;

	// current walk speed is cached everytime the player is frozen
	float CurrentWalkSpeed;

	UPROPERTY()
	UWeaponManagerComponent* WeaponManager;
	
	UPROPERTY(EditAnywhere, Category = "PlayerController|Weapon")
	TSubclassOf<AWeapon_Base> WeaponSlot1;

	UPROPERTY(EditAnywhere, Category = "PlayerController|Weapon")
	TSubclassOf<AWeapon_Base> WeaponSlot2;

	UPROPERTY(EditAnywhere, Category = "PlayerController|Weapon")
	TSubclassOf<AWeapon_Base> WeaponSlot3;

	UPROPERTY(EditAnywhere, Category = "PlayerController|GameplayAbilitySystem|Effects")
	TSubclassOf<UGameplayEffect> StaminaRegenEffect;

	UPROPERTY(EditAnywhere, Category = "PlayerController|GameplayAbilitySystem|Tags")
	FGameplayTag StaminaRegenTag;

	UFUNCTION()
	void ApplyStaminaRegen();

	UFUNCTION()
	void RemoveStaminaRegen();
};
