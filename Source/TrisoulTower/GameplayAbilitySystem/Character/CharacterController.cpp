#include "CharacterController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TrisoulTower/GameplayAbilitySystem/Abilities/GA_Dash.h"

ACharacterController::ACharacterController()
{
	// setup gameplay ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// add basic attribute set
	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));
	
	// sets default mesh transformations
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// camera setup
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	SpringArmComp->bUsePawnControlRotation = true;

	// for character mesh rotation
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();

	// Setup input
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputContext, 0);
		}
	}

	// Cache max walk speed value
	OriginalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// TEMP: giving player dash ability for testing purposes
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(UGA_Dash::StaticClass(), 1));
}

void ACharacterController::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ACharacterController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	if (Controller)
	{
		// gets the camera orientation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// sets movement direction based on camera rotation
		const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// applies the result
		AddMovementInput(DirectionX, MovementValue.X);
		AddMovementInput(DirectionY, MovementValue.Y);
	}
}

void ACharacterController::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookValue.X);
		AddControllerPitchInput(LookValue.Y);
	}
}

void ACharacterController::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed *= SprintMultiplier;
}

void ACharacterController::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
}

void ACharacterController::Dash()
{
	AbilitySystemComponent->TryActivateAbilityByClass(UGA_Dash::StaticClass());
}

void ACharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Binding actions to their respective method
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterController::Move);
		EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterController::Look);
		
		EnhancedInputComp->BindAction(SprintAction, ETriggerEvent::Started,this, &ACharacterController::StartSprint);
		EnhancedInputComp->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACharacterController::StopSprint);

		EnhancedInputComp->BindAction(DashAction, ETriggerEvent::Started, this, &ACharacterController::Dash);
		
		// Binding JumpAction to default CharacterMovementComponent jump function
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
}

UAbilitySystemComponent* ACharacterController::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
