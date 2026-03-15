#include "WeaponManagerComponent.h"
#include "TrisoulTower/GameplayAbilitySystem/Character/CharacterController.h"

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		CharacterRef = Cast<ACharacterController>(Owner);
	}
}

void UWeaponManagerComponent::EquipWeapon(TSubclassOf<AWeapon_Base> WeaponToEquip)
{
	if (!CharacterRef || !WeaponToEquip) return;

	// if the same weapon is equipped, unequip
	if (EquippedWeapon && EquippedWeapon->GetClass() == WeaponToEquip)
	{
		UnequipWeapon();
		return;
	}
	
	// unequips existing weapon
	if (EquippedWeapon)
	{
		UnequipWeapon();
		CurrentEquippedWeaponClass = nullptr;
	}

	// spawn weapon
	FVector SpawnLocation = CharacterRef->GetActorLocation();
	FRotator SpawnRotation = CharacterRef->GetActorRotation();
	FActorSpawnParameters SpawnParameters;
	
	EquippedWeapon = GetWorld()->SpawnActor<AWeapon_Base>(
		WeaponToEquip, SpawnLocation, SpawnRotation, SpawnParameters);

	// attach weapon to socket
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachToComponent(
			CharacterRef->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			EquippedWeapon->WeaponProperties.SocketName
		);
	}

	// set anim class
	CharacterRef->GetMesh()->SetAnimInstanceClass(EquippedWeapon->WeaponProperties.AnimClass);

	// grant abilities
	GrantedAbilities = CharacterRef->GrantAbilities(EquippedWeapon->WeaponProperties.AbilitiesToGrant);

	// grant parry ability
	FGameplayAbilitySpec ParrySpec(EquippedWeapon->WeaponProperties.ParryAbility, 1, 0);
	ParrySpecHandle = CharacterRef->AbilitySystemComponent->GiveAbility(ParrySpec);

	FGameplayAbilitySpec GroundSlamSpec(EquippedWeapon->WeaponProperties.GroundSlamAbility, 1, -1);
	GroundSlamSpecHandle = CharacterRef->AbilitySystemComponent->GiveAbility(GroundSlamSpec);

	// sets current equipped weapon
	CurrentEquippedWeaponClass = WeaponToEquip;
}

TSubclassOf<AWeapon_Base> UWeaponManagerComponent::GetCurrentEquippedWeaponClass()
{
	return CurrentEquippedWeaponClass;
}

void UWeaponManagerComponent::UnequipWeapon()
{
	if (!CharacterRef || !EquippedWeapon) return;

	// destroy weapon
	EquippedWeapon->Destroy();
	EquippedWeapon = nullptr;

	// set anim class
	CharacterRef->GetMesh()->SetAnimInstanceClass(DefaultAnimClass);
	
	// remove abilities
	CharacterRef->RemoveAbilities(GrantedAbilities);

	CharacterRef->AbilitySystemComponent->ClearAbility(ParrySpecHandle);
	CharacterRef->AbilitySystemComponent->ClearAbility(GroundSlamSpecHandle);
}

AWeapon_Base* UWeaponManagerComponent::GetEquippedWeapon()
{
	return EquippedWeapon;
}

FGameplayAbilitySpecHandle UWeaponManagerComponent::GetAbility(int AbilityIndex)
{
	if (GrantedAbilities.IsValidIndex(AbilityIndex))
		return GrantedAbilities[AbilityIndex];
	
	FGameplayAbilitySpecHandle EmptyAbilitySpec;
	return EmptyAbilitySpec;
}
