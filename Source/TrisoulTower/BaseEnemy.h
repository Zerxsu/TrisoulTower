// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"//#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
//#include "AIGroupManager.h"
#include "BaseEnemy.generated.h"

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	Direct UMETA(DisplayName = "Direct"),
	Front UMETA(DisplayName = "Front"),
	Near UMETA(DisplayName = "Near"),
	Pack UMETA(DisplayName = "Pack")
};

UCLASS()
class TRISOULTOWER_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	ETargetType TargetType = ETargetType::Direct;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behaviour")
	bool needPoint = false;

	float stunTime = 0.0f;

	FNavigationPath* NavPath;

	USkeletalMeshComponent* Body;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool isMoving = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool isAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool takeHit = false;

	// Sets default values for this pawn's properties
	ABaseEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void FindTarget();

	void FindPackTarget();

	FVector GetPackPoint();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void SetDestination(FVector To, bool path);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void MakePath();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	TArray<FVector> GetPath();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void ReachedTarget();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	bool IsAtTarget();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	bool CanSeeTarget();

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	void StartAttack();

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	void EndAttack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Behaviour")
	void RunAttack();

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	virtual void TakeAttack(float damage, bool parry);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void AssignPoint(FVector2D at, int priotity);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UNavigationSystemV1* NavSystem;

	FVector Destination = FVector(0, 0, 0);

	bool isAtTarget = false;
	bool pointAtPlayer = true;//Assigned point is player adjacent (priority 1)

	UPROPERTY(BlueprintReadOnly)
	AActor* PlayerActor;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed = 64.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StopDist = 32.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TargetDist = 256.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	//float MoveDist = 512.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	bool IsPackLeader = false;//Is this the pack leader
	ABaseEnemy* PackLeader = nullptr;

public:	
	

};
