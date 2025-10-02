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
	// Sets default values for this pawn's properties
	ABaseEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	ETargetType TargetType = ETargetType::Direct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed = 256.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StopDist = 32.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UNavigationSystemV1* NavSystem;
	FVector Destination = FVector(0, 0, 0);
	bool isAtTarget = false;

	AActor* PlayerActor;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void FindTarget();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual void SetDestination(FVector To, bool path);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual void MakePath();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual TArray<FVector> GetPath();

	FNavigationPath* NavPath;

};
