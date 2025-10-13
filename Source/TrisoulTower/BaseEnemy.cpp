// Fill out your copyright notice in the Description page of Project Settings.


#include "Containers/UnrealString.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BaseEnemy.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	NavSystem->RegisterNavigationInvoker(this);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Target")), FoundActors);
	if (FoundActors.Num() > 0) PlayerActor = FoundActors[0];

	Destination = GetActorLocation();
	//MakePath();
	FindTarget();
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (stunTime > 0) stunTime -= DeltaTime;

	if (!isAttacking && stunTime <= 0) FindTarget();

	if (!isAtTarget && stunTime <= 0) {//If path is unfinished, //Removed NavPath check because it kept crashing

		if (NavPath->GetLength() <= TargetDist && CanSeeTarget()) {
			ReachedTarget();
		}
		else 
		{
			MakePath();//Recalculate path to target

			FVector Dir;//Direction to next point
			Dir = NavPath->GetPathPointLocation(1).Position - GetActorLocation();
			Dir.Normalize();
			SetActorLocation(GetActorLocation() + Dir * Speed * DeltaTime);//Move actor towards next point
		}
		
	}

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//return;
}

void ABaseEnemy::FindTarget()
{

	if (TargetType == ETargetType::Direct || TargetType == ETargetType::Near) {
		SetDestination(PlayerActor->GetActorLocation(), true);
		
	} else if (TargetType == ETargetType::Front)
	{
		FVector Dist = PlayerActor->GetActorLocation();
		Dist += PlayerActor->GetActorForwardVector() * TargetDist;
		SetDestination(Dist, true);
	}

}

/** 
 * Set the pathing destination of this actor
 * @param To New destination
 * @param path Recalculate a new path
 */
void ABaseEnemy::SetDestination(FVector To, bool path = true)
{
	Destination = To;
	isAtTarget = false;
	if (path) MakePath();
}

//Generate a path to the actor's destination
void ABaseEnemy::MakePath()
{
    UNavigationPath* NavP = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), Destination, this);

	if (NavP && NavP->IsValid() && NavP->GetPath()) {
		FNavPathSharedPtr NewPath = NavP->GetPath();
		const TArray<FNavPathPoint>& Points = NewPath->GetPathPoints();
		NavPath = NewPath.Get();
	}
}

TArray<FVector> ABaseEnemy::GetPath(){

	TArray<FVector> points;

	if (NavPath) {
		for (int i = 0; i < NavPath->GetPathPoints().Num(); i++) {
			points.Add(NavPath->GetPathPoints()[i].Location);
		}
	} else {
		FString DebugMessage = FString::Printf(TEXT("No NavPath"));
		GEngine->AddOnScreenDebugMessage(
			-1,            // A unique key. -1 means a new entry each time.
			5.0f,          // How long the message will display (in seconds).
			FColor::Green, // The color of the debug text.
			DebugMessage   // The FString containing the message to print.
		);
	}

	return points;
}

void ABaseEnemy::ReachedTarget() {
	isAtTarget = true;

	FString DebugMessage = FString::Printf(TEXT("At Target"));
	GEngine->AddOnScreenDebugMessage(
		-1,            // A unique key. -1 means a new entry each time.
		5.0f,          // How long the message will display (in seconds).
		FColor::Green, // The color of the debug text.
		DebugMessage   // The FString containing the message to print.
	);

	//Check of target is at the right position
	StartAttack();
}

bool ABaseEnemy::CanSeeTarget() {

	if (TargetType != ETargetType::Near) return true;

	//bool hitSomething = 
	FHitResult hit;
	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(
		hit,
		GetActorLocation(),
		Destination,
		ECC_WorldStatic,
		traceParams
	);

	DrawDebugLine(
        GetWorld(),
        GetActorLocation(),
        hit.ImpactPoint,
        FColor::Red, // Color of the debug line
        false,       // Persistent?
        5.f          // Life span of the line
    );

	if (hit.ImpactPoint == Destination || hit.GetActor() == PlayerActor) {

		FString DebugMessage = FString::Printf(TEXT("Sees Player"));
		GEngine->AddOnScreenDebugMessage(
			-1,            // A unique key. -1 means a new entry each time.
			5.0f,          // How long the message will display (in seconds).
			FColor::Green, // The color of the debug text.
			DebugMessage   // The FString containing the message to print.
		);

		return true;
	}

	return false;
}

void ABaseEnemy::StartAttack() {
	isAttacking = true;

	FString DebugMessage = FString::Printf(TEXT("StartAttack"));
	GEngine->AddOnScreenDebugMessage(
		-1,            // A unique key. -1 means a new entry each time.
		5.0f,          // How long the message will display (in seconds).
		FColor::Green, // The color of the debug text.
		DebugMessage   // The FString containing the message to print.
	);

	//Wait for attack animation to happen
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ABaseEnemy::EndAttack, 2.0f, false);
}

void ABaseEnemy::EndAttack() {
	isAttacking = false;

	FString DebugMessage = FString::Printf(TEXT("EndAttack"));
	GEngine->AddOnScreenDebugMessage(
		-1,            // A unique key. -1 means a new entry each time.
		5.0f,          // How long the message will display (in seconds).
		FColor::Green, // The color of the debug text.
		DebugMessage   // The FString containing the message to print.
	);

}

void ABaseEnemy::TakeDamage(float damage, bool parry) {

	if (parry) {
		stunTime += 2.5f;
		isAttacking = false;
		isAtTarget = false;
	}

	Health -= damage;
	if (Health <= 0) {

		//Die

		FString DebugMessage = FString::Printf(TEXT("Dead"));
		GEngine->AddOnScreenDebugMessage(
			-1,            // A unique key. -1 means a new entry each time.
			5.0f,          // How long the message will display (in seconds).
			FColor::Red, // The color of the debug text.
			DebugMessage   // The FString containing the message to print.
		);

	}

}