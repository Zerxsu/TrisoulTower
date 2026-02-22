// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "AIGroupManager.generated.h"


USTRUCT(BlueprintType)
struct FNavPoint
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FVector2D RelativePos = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int priority = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isValid = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseEnemy* AIGuy = nullptr;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRISOULTOWER_API UAIGroupManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIGroupManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	AActor* PlayerActor;

	TArray<ABaseEnemy*> Navigators;

	float StartDist = 48.0f;//Seperation from player
	float Separation = 256.0f;//32.0f;//Seperation between rings

	UPROPERTY(BlueprintReadWrite)
	TMap<FVector2D, FNavPoint> Points;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddPoint(FVector2D at, int prio);

	FNavPoint* GetPoint(FVector2D at);

	UFUNCTION(BlueprintCallable)
	void AssignPoint(ABaseEnemy* ai, int minRing = 0);	

	void TradePoint(FVector2D at);

	FVector2D GetHexPos(int point, int dist = 1);

};
