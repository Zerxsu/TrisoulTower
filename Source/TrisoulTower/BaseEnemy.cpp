// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemy.h"
#include "Containers/UnrealString.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"



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

	Body = FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInst = Body->GetAnimInstance();

	Destination = GetActorLocation();
	
	FindTarget();
	MakePath();

	if (TargetType == ETargetType::Direct) needPoint = true;
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Disable) {
		stunTime = 1.0f;//This is a workaround to dead enemies being coordinated
		return;
	}

	if (stunTime > 0) stunTime -= DeltaTime;

	if (!isAttacking && stunTime <= 0) {
		FindTarget();
		ReactTimer -= FMath::FRand() * DeltaTime;
	}

	if (!isAtTarget && stunTime <= 0 && NavPath != nullptr) {//If path is unfinished
		
		//if (NavPath->GetLength() <= TargetDist) bool m = true;
		//if (CanSeeTarget()) bool m = false;
		
		//if ((NavPath->GetLength() <= TargetDist && CanSeeTarget()) || IsAtTarget()) {
		if (IsAtTarget()) {
			ReachedTarget();
			isMoving = false;
		}
		else 
		{

			if (TargetType == ETargetType::Direct && FVector::Distance(GetActorLocation(), PlayerActor->GetActorLocation()) <= TargetDist * 2) {
				ReachedTarget();
				StartAttack();
				return;
			}

			if (ReactTimer > 0) {
				isMoving = false;
				return;
			}

			MakePath();//Recalculate path to target
			isMoving = true;

			FVector Dir;//Direction to next point

			if (!NavPath) return;
			
			Dir = NavPath->GetPathPointLocation(1).Position - GetActorLocation();
			Dir.Normalize();
			//SetActorLocation(GetActorLocation() + Dir * Speed * DeltaTime);//Move actor towards next point
			AddMovementInput(Dir, Speed / 100.0f);

			FRotator Rot = GetActorRotation();
			SetActorRotation(FRotator(Rot.Pitch, Dir.Rotation().Yaw, Rot.Roll));

		}
		
	}

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//return;
}

void ABaseEnemy::DebugPoint(FVector at, FColor col) {

	DrawDebugSphere(
		GetWorld(),
		at,
		50.0f,
		12,
		col,
		false,
		0,
		SDPG_World,
		1.0f
	);

}

//Set target point from AIGroupManager
void ABaseEnemy::AssignPoint(FVector2D at, int prio)
{
	if (Disable) return;

	needPoint = false;
	if (PlayerActor == nullptr) return;
	if (isAttacking) return;

	FVector new_at = FVector(at.X, at.Y, 0);

	SetDestination(PlayerActor->GetActorLocation() + new_at, true);

	if (TargetType == ETargetType::Direct) pointAtPlayer = prio <= 1;  

}

void ABaseEnemy::FindTarget()
{

	if (TargetType == ETargetType::Near) {//TargetType == ETargetType::Direct || 

		FVector Separation = (GetActorLocation() - PlayerActor->GetActorLocation()).GetSafeNormal();
		//DebugPoint(PlayerActor->GetActorLocation() + Separation, FColor::Green);
		Separation *= TargetDist;
		Separation.Z = GetActorLocation().Z;
		//DebugPoint(PlayerActor->GetActorLocation() + Separation, FColor::Blue);
		SetDestination(PlayerActor->GetActorLocation() + Separation, true);

		/*
		if (FVector::Distance(GetActorLocation(), PlayerActor->GetActorLocation()) <= TargetDist * 0.75) {
			FVector Separation = (PlayerActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			Separation *= TargetDist;
			Separation.Z = 0;
			SetDestination(PlayerActor->GetActorLocation() + Separation, true);
		} else {
			SetDestination(PlayerActor->GetActorLocation(), true);
		}
		*/
		
	} else if (TargetType == ETargetType::Front)
	{
		FVector Dist = PlayerActor->GetActorLocation();
		Dist += PlayerActor->GetActorForwardVector() * TargetDist;
		Dist.Z = PlayerActor->GetActorLocation().Z;
		SetDestination(Dist, true);
	} else if (TargetType == ETargetType::Pack) {
		FindPackTarget();
	}

}

//Find target location based on pack navigation
void ABaseEnemy::FindPackTarget()
{

	if (FVector::Distance(GetActorLocation(), PlayerActor->GetActorLocation()) <= TargetDist) {
		SetDestination(PlayerActor->GetActorLocation(), true);
	} else {

		TArray<AActor*> OverlappingActors;
		
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

		UClass* ThisClass = ABaseEnemy::StaticClass();

		TArray<AActor*> Ignore;
		Ignore.Add(this);

		bool Found = UKismetSystemLibrary::SphereOverlapActors(
			GetWorld(),
			GetActorLocation(),
			512.0f,
			ObjectTypes,
			ThisClass,
			Ignore,
			OverlappingActors
		);

		IsPackLeader = true;
		if (Found) {
			for (AActor* Actor: OverlappingActors) {
				ABaseEnemy* Other = Cast<ABaseEnemy>(Actor);
				if (Other->IsPackLeader) {
					IsPackLeader = false;
					PackLeader = Other;
					break;
				}
			}

			if (IsPackLeader) SetDestination(PlayerActor->GetActorLocation(), true);
			else SetDestination(PackLeader->GetPackPoint(), true);

		} else {
			PackLeader = this;
			SetDestination(PlayerActor->GetActorLocation(), true);
		}

		
	}

}

//Get the target move position based on pack  navigation
FVector ABaseEnemy::GetPackPoint() {

	if (NavPath == nullptr) {
		return GetActorLocation();
	} else if (NavPath->GetLength() <= TargetDist * 2.0f || NavPath->GetPathPoints().Num() <= 1) {
		return PlayerActor->GetActorLocation();
	} else {
		return NavPath->GetPathPointLocation(1).Position;
	}

}

/** 
 * Set the pathing destination of this actor
 * @param To New destination
 * @param path Recalculate a new path
 */
void ABaseEnemy::SetDestination(FVector To, bool path = true)
{
	
	//DebugPoint(To, FColor::Red);
	
	Destination = To;
	isAtTarget = false;
	if (path) MakePath();
}

//Generate a path to the actor's destination
void ABaseEnemy::MakePath()
{
    UNavigationPath* NavP = UNavigationSystemV1::FindPathToLocationSynchronously(
		GetWorld(), 
		GetActorLocation(), 
		Destination, 
		this
	);

	if (NavP && NavP->IsValid() && NavP->GetPath()) {
		FNavPathSharedPtr NewPath = NavP->GetPath();
		const TArray<FNavPathPoint>& Points = NewPath->GetPathPoints();
		NavPath = NewPath.Get();
	}
	else if (TargetType == ETargetType::Direct) needPoint = true;//If a path can't be made to desired point, request a new one
}

TArray<FVector> ABaseEnemy::GetPath(){

	TArray<FVector> points;

	if (NavPath->GetPathPoints().Num() > 0) {
		for (int i = 0; i < NavPath->GetPathPoints().Num(); i++) {
			points.Add(NavPath->GetPathPoints()[i].Location);
		}
	} else {
		FString DebugMessage = FString::Printf(TEXT("No NavPath"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
	}

	return points;
}

bool ABaseEnemy::IsAtTarget() {
	if (TargetType == ETargetType::Near && FVector::Distance(GetActorLocation(), PlayerActor->GetActorLocation()) <= TargetDist * 0.75) return false;
	return FVector::Distance(GetActorLocation(), PlayerActor->GetActorLocation()) <= TargetDist;
}

void ABaseEnemy::ReachedTarget() {
	isAtTarget = true;
	ReactTimer = ReactTime;

	//FString DebugMessage = FString::Printf(TEXT("At Target"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);

	FRotator LookAtPlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerActor->GetActorLocation());
	LookAtPlayer.Pitch = GetActorRotation().Pitch;
	LookAtPlayer.Roll = GetActorRotation().Roll;
	SetActorRotation(LookAtPlayer);


	//FColor aer = FColor::Cyan;
	//if (pointAtPlayer) aer = FColor::Magenta;

	/*
	DrawDebugSphere(
		GetWorld(),
		Destination,
		32.0f,
		8,
		aer,
		false,
		5.0f,
		0, // Depth priority
		2.0f
	);
	*/

	//Check of target is at the right position
	if (!isAttacking && (pointAtPlayer || IsAtTarget())) StartAttack();
	//else if (TargetType == ETargetType::Direct) needPoint = true;
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
        FColor::Red,
        false,
        5.f 
    );

	if (hit.ImpactPoint == Destination || hit.GetActor() == PlayerActor) {

		//FString DebugMessage = FString::Printf(TEXT("Sees Player"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);

		return true;
	}

	return false;
}

void ABaseEnemy::StartAttack() {
	isAttacking = true;

	//FString DebugMessage = FString::Printf(TEXT("StartAttack"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);

}

void ABaseEnemy::EndAttack() {
	isAttacking = false;

	if (IsAtTarget() && TargetType == ETargetType::Direct) needPoint = true;

	//FString DebugMessage = FString::Printf(TEXT("EndAttack"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, DebugMessage);

}


void ABaseEnemy::RunAttack_Implementation() {
	FString DebugMessage = FString::Printf(TEXT("Strike"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, DebugMessage);
}

void ABaseEnemy::TakeAttack(float damage, bool parry) {

	RunDamage();

	if (parry || Vulnerable) {
		Vulnerable = false;
		stunTime += 2.5f;
		isAttacking = false;
		isAtTarget = false;
		takeHit = true;
		if (TargetType == ETargetType::Direct) needPoint = true;
	}

	Health -= damage;
	if (Health <= 0) {

		isDead = true;

		FString DebugMessage = FString::Printf(TEXT("Dead"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DebugMessage);
		//Destroy();
	}

}

void ABaseEnemy::RunDamage_Implementation() {
	FString DebugMessage = FString::Printf(TEXT("Damage"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, DebugMessage);
}