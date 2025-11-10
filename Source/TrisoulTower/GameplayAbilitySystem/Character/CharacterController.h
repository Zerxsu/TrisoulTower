#pragma once

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "TrisoulTower/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UFUNCTION(BlueprintCallable, Category = "PlayerController|GameplayAbilitySystem")
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant);

	UFUNCTION(BlueprintCallable, Category = "PlayerController|GameplayAbilitySystem")
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|GameplayAbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Variables")
	float SprintMultiplier = 1.5f;

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void StartSprint();

	UFUNCTION()
	void StopSprint();

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

private:
	float OriginalWalkSpeed;

	UPROPERTY()
	UWeaponManagerComponent* WeaponManager;
};
