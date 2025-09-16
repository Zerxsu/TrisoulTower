#pragma once

#include "EnhancedInputComponent.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* InputContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* JumpAction;

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);
};
