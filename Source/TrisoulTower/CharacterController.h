#pragma once

#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterController.generated.h"

UCLASS()
class TRISOULTOWER_API ACharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterController();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Variables")
	float DashForce;

	virtual void BeginPlay() override;
	
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
