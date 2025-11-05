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

UCLASS()
class TRISOULTOWER_API ACharacterController : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterController();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	// GameplayAbilitySystem components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameplayAbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameplayAbilitySystem")
	UBasicAttributeSet* BasicAttributeSet;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
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

private:
	float OriginalWalkSpeed;
};
