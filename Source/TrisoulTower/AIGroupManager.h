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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FVector2D, FNavPoint> Points;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddPoint(FVector2D at, int prio);

	FNavPoint* GetPoint(FVector2D at);

	void AssignPoint(ABaseEnemy* ai);	

};
