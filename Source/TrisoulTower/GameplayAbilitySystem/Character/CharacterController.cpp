#include "CharacterController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TrisoulTower/Player/Weapon/WeaponManagerComponent.h"
#include "TrisoulTower/GameplayAbilitySystem/Abilities/GA_Attack.h"
#include "TrisoulTower/GameplayAbilitySystem/Abilities/GA_Dash.h"

ACharacterController::ACharacterController()
{
	// setup gameplay ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// setup weapon manager component
	WeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));

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
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 15.f;

	// for character mesh rotation
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();

	// setup input
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputContext, 0);
		}
	}

	CurrentWalkSpeed = JogSpeed;
	GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
	bCanMove = true;
	bCanDash = true;

	bCanEquipWeapon = true;

	WeaponManager->DefaultAnimClass = PlayerAnimClass;
}

void ACharacterController::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GrantAbilities(StartingAbilities);
	}
}

void ACharacterController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	// sets player movement speed to jog speed after input is released
	if (GetLastMovementInputVector().IsNearlyZero() && bCanMove)
	{
		if (CurrentWalkSpeed >= JogSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
		}
	}
	
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

void ACharacterController::Dash()
{
	if (!bCanDash) return;
	
	// starts sprint
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	// sets default walk speed to jogging after input release
	CurrentWalkSpeed = SprintSpeed;
	
	if (!GetMovementComponent()->IsFalling())
		AbilitySystemComponent->TryActivateAbilityByClass(UGA_Dash::StaticClass());
}

void ACharacterController::ToggleWalk()
{
	if (CurrentWalkSpeed == WalkSpeed)
	{
		CurrentWalkSpeed = JogSpeed;
	}
	else if (CurrentWalkSpeed == JogSpeed || CurrentWalkSpeed == SprintSpeed)
	{
		CurrentWalkSpeed = WalkSpeed;
	}

	GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
}

void ACharacterController::ParryPressed()
{
	if (!WeaponManager->GetEquippedWeapon())
		return;

	AbilitySystemComponent->AbilityLocalInputPressed(0);
	TSubclassOf<UGameplayAbility> ParryAbility = WeaponManager->GetEquippedWeapon()->WeaponProperties.ParryAbility;
	AbilitySystemComponent->TryActivateAbilityByClass(ParryAbility);
}

void ACharacterController::ParryReleased()
{
	AbilitySystemComponent->AbilityLocalInputReleased(0);
}

void ACharacterController::LightAttack()
{
	if (!WeaponManager->GetEquippedWeapon())
		return;

	// temp implementation for ground slam ability activation
	if (GetCharacterMovement()->IsFalling())
	{
		TSubclassOf<UGameplayAbility> GroundSlamAbility = WeaponManager->GetEquippedWeapon()->WeaponProperties.GroundSlamAbility;
		AbilitySystemComponent->TryActivateAbilityByClass(GroundSlamAbility);
		return;
	}
	
	TSubclassOf<UGameplayAbility> LightAttack = WeaponManager->GetEquippedWeapon()->GetAbility(0);
	AbilitySystemComponent->TryActivateAbilityByClass(LightAttack);
}

void ACharacterController::HeavyAttack()
{
	if (!WeaponManager->GetEquippedWeapon())
		return;
	
	TSubclassOf<UGameplayAbility> HeavyAttack = WeaponManager->GetEquippedWeapon()->GetAbility(1);
	AbilitySystemComponent->TryActivateAbilityByClass(HeavyAttack);
}

void ACharacterController::UltimateAttack()
{
	if (!WeaponManager->GetEquippedWeapon())
		return;
	
	TSubclassOf<UGameplayAbility> UltimateAttack = WeaponManager->GetEquippedWeapon()->GetAbility(2);
	AbilitySystemComponent->TryActivateAbilityByClass(UltimateAttack);
}

void ACharacterController::EquipWeapon1()
{
	HandleWeaponEquip(WeaponSlot1);
}

void ACharacterController::EquipWeapon2()
{
	HandleWeaponEquip(WeaponSlot2);
}

void ACharacterController::EquipWeapon3()
{
	HandleWeaponEquip(WeaponSlot3);
}

void ACharacterController::HandleWeaponEquip(const TSubclassOf<AWeapon_Base>& WeaponToEquip)
{
	if (!WeaponToEquip || !bCanEquipWeapon || GetWorld()->GetTimerManager().IsTimerActive(WeaponEquipCooldown))
		return;
	
	bCanEquipWeapon = false;
	WeaponManager->EquipWeapon(WeaponToEquip);

	// weapon equip cooldown
	GetWorld()->GetTimerManager().SetTimer(
			WeaponEquipCooldown,
			FTimerDelegate::CreateLambda([this]()
			{
				// this executes after the timer
				bCanEquipWeapon = true;
			}),
			.5f,
			false
	);
}

void ACharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// binding actions to their respective method
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterController::Move);
		EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterController::Look);
		
		// binding JumpAction to default CharacterMovementComponent jump function
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// binding abilities
		EnhancedInputComp->BindAction(DashAction, ETriggerEvent::Started, this, &ACharacterController::Dash);
		EnhancedInputComp->BindAction(ParryAction, ETriggerEvent::Started, this, &ACharacterController::ParryPressed);
		EnhancedInputComp->BindAction(ParryAction, ETriggerEvent::Completed, this, &ACharacterController::ParryReleased);
		EnhancedInputComp->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &ACharacterController::LightAttack);
		EnhancedInputComp->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ACharacterController::HeavyAttack);
		EnhancedInputComp->BindAction(UltimateAttackAction, ETriggerEvent::Started, this, &ACharacterController::UltimateAttack);

		// binding weapon slots
		EnhancedInputComp->BindAction(EquipWeapon1Action, ETriggerEvent::Started, this, &ACharacterController::EquipWeapon1);
		EnhancedInputComp->BindAction(EquipWeapon2Action, ETriggerEvent::Started, this, &ACharacterController::EquipWeapon2);
		EnhancedInputComp->BindAction(EquipWeapon3Action, ETriggerEvent::Started, this, &ACharacterController::EquipWeapon3);

		// walk / jog toggle
		EnhancedInputComp->BindAction(ToggleWalkAction, ETriggerEvent::Started, this, &ACharacterController::ToggleWalk);
	}
}

UAbilitySystemComponent* ACharacterController::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

TArray<FGameplayAbilitySpecHandle> ACharacterController::GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant)
{
	if (!AbilitySystemComponent) return TArray<FGameplayAbilitySpecHandle>();

	TArray<FGameplayAbilitySpecHandle> AbilityHandles;
	for (TSubclassOf<UGameplayAbility> Ability : AbilitiesToGrant)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, -1, this));
		AbilityHandles.Add(SpecHandle);
	}
	
	return AbilityHandles;
}

void ACharacterController::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove)
{
	if (!AbilitySystemComponent) return;
	
	for (FGameplayAbilitySpecHandle AbilityHandle : AbilitiesToRemove)
		AbilitySystemComponent->ClearAbility(AbilityHandle);
}

void ACharacterController::ResetAttackCombo()
{
	AttackIndex = 0;
	bIsAttacking = false;
}

void ACharacterController::FreezeMovement(bool CanMove)
{
	if (CanMove)
	{
		bCanMove = true;
		GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
	}
	else
	{
		bCanMove = false;
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}
}

void ACharacterController::LockRotation(bool IsLocked)
{
	if (IsLocked)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}
