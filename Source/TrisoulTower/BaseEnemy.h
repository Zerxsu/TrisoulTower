// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NavigationSystem.h"
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
class TRISOULTOWER_API ABaseEnemy : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	ETargetType TargetType = ETargetType::Direct;

	FNavigationPath* NavPath;


	// Sets default values for this pawn's properties
	ABaseEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void FindTarget();

	virtual void FindPackTarget();

	virtual FVector GetPackPoint();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual void SetDestination(FVector To, bool path);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual void MakePath();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual TArray<FVector> GetPath();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual void ReachedTarget();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual bool CanSeeTarget();

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	virtual void StartAttack();

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	virtual void EndAttack();

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	virtual void TakeAttack(float damage, bool parry);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UNavigationSystemV1* NavSystem;

	FVector Destination = FVector(0, 0, 0);

	float stunTime = 0.0f;

	bool isAtTarget = false;
	bool isAttacking = false;

	AActor* PlayerActor;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed = 256.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StopDist = 32.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TargetDist = 512.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Health = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	bool IsPackLeader = false;//Is this the pack leader
	ABaseEnemy* PackLeader = nullptr;

public:	
	

};
