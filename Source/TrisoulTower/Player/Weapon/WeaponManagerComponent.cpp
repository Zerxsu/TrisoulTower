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

		//temp
		CharacterRef->bIsWeaponEquipped = true;
	}

	// set anim class
	//CharacterRef->GetMesh()->SetAnimInstanceClass(EquippedWeapon->WeaponProperties.AnimClass);

	// grant abilities
	GrantedAbilities = CharacterRef->GrantAbilities(EquippedWeapon->WeaponProperties.AbilitiesToGrant);
}

void UWeaponManagerComponent::UnequipWeapon()
{
	if (!CharacterRef || !EquippedWeapon) return;

	// destroy weapon
	EquippedWeapon->Destroy();
	EquippedWeapon = nullptr;

	//temp
	CharacterRef->bIsWeaponEquipped = false;

	// set anim class
	//CharacterRef->GetMesh()->SetAnimInstanceClass(DefaultAnimClass);
	
	// remove abilities
	CharacterRef->RemoveAbilities(GrantedAbilities);
}

AWeapon_Base* UWeaponManagerComponent::GetEquippedWeapon()
{
	return EquippedWeapon;
}
