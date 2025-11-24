#include "GA_Dash.h"

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "TrisoulTower/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"
#include "TrisoulTower/GameplayAbilitySystem/Character/CharacterController.h"
#include "TrisoulTower/GameplayAbilitySystem/Effects/GE_DashCooldown.h"
#include "TrisoulTower/GameplayAbilitySystem/Effects/GE_DashCost.h"

UGA_Dash::UGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// sets the gameplay cost and cooldown classes
	//CostGameplayEffectClass = UGE_DashCost::StaticClass();
	CooldownGameplayEffectClass = UGE_DashCooldown::StaticClass();
	
	DashCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Dash.Active"));
}

void UGA_Dash::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	SetDashing(true);
	
	UAbilityTask_ApplyRootMotionConstantForce* DashAbilityTask =
		UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this,
			FName("DashAbilityTask"),
			GetDashDirection(),
			2500.f,
			0.18f,
			false,
			nullptr,
			ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity,
			FVector::ZeroVector,
			GetMaxSpeed(),
			true);

	DashAbilityTask->OnFinish.AddDynamic(this, &UGA_Dash::OnDashFinished);
	DashAbilityTask->ReadyForActivation();

	// handle gameplay cue
	FGameplayCueParameters Params;
	Params.Instigator = ActorInfo->AvatarActor.Get();
	ActorInfo->AbilitySystemComponent->AddGameplayCue(DashCueTag, Params);
}

void UGA_Dash::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	ActorInfo->AbilitySystemComponent->RemoveGameplayCue(DashCueTag);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FVector UGA_Dash::GetDashDirection() const
{
	AActor* Player = GetAvatarActorFromActorInfo();

	const APawn* PlayerPawn = Cast<APawn>(Player);
	if (PlayerPawn)
	{
		const FVector LastInput = PlayerPawn->GetLastMovementInputVector();
		if (!LastInput.IsNearlyZero())
		{
			return LastInput;
		}
	}
	return Player->GetActorForwardVector();
}

float UGA_Dash::GetMaxSpeed() const
{
	AActor* Player = GetAvatarActorFromActorInfo();
	const APawn* PlayerPawn = Cast<APawn>(Player);

	if (PlayerPawn)
	{
		return PlayerPawn->GetMovementComponent()->GetMaxSpeed();
	}

	// hard coded return
	return 700.f;
}

void UGA_Dash::OnDashFinished()
{
	SetDashing(false);
	
	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_Dash::SetDashing(bool bIsDashing)
{
	AActor* Player = GetAvatarActorFromActorInfo();
	ACharacterController* PlayerController = Cast<ACharacterController>(Player);

	if (PlayerController)
	{
		PlayerController->bIsDashing = bIsDashing;
	}
}
