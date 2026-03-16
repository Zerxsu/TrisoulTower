// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioManager.generated.h"

class UAkRtpc;
class UAkStateValue;
class UAkAudioEvent;

UCLASS()
class TRISOULTOWER_API UAudioManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// UE standard Init & DeInit
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Plays sound effect
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetEvent(UAkAudioEvent* SoundEffect, AActor* ActorSource);
	
	// Sets states from Wwise project
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetState(UAkStateValue const* StateName);

	// Sets parameters from Wwise project
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetParameter(UAkRtpc const* ParamName, float Value, int32 InterpSpeed);
};
