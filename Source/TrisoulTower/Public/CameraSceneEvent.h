// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Actor.h"
#include "CameraSceneEvent.generated.h"


UENUM(BlueprintType)
enum class EEventType : uint8
{
	SetCamera UMETA(DisplayName = "Set Camera"),
	MoveActor UMETA(DisplayName = "Move Actor"),
	CallEvent UMETA(DisplayName = "Call Event"),
	FadeInOut UMETA(DisplayName = "Fade In/Out"),
	Wait UMETA(DisplayName = "Wait")
};


USTRUCT(BlueprintType)
struct FCamEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEventType Event;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
    float duration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
    bool waitForFinish = true;

	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Event == EEventType::SetCamera", EditConditionHides))
    ACameraActor* Camera;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Event == EEventType::MoveActor || Event == EEventType::CallEvent", EditConditionHides))
    AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Event == EEventType::MoveActor", EditConditionHides))
    FTransform StartTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Event == EEventType::MoveActor", EditConditionHides))
    FTransform EndTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Event == EEventType::CallEvent", EditConditionHides))
    FString EventName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Event == EEventType::FadeInOut || Event == EEventType::Wait", EditConditionHides))
    FColor FadeColour = FColor(0, 0, 0, 255);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Event == EEventType::FadeInOut", EditConditionHides))
    bool fadeIn = false;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRISOULTOWER_API UCameraSceneEvent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraSceneEvent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
