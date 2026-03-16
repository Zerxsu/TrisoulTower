// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioManager.h"

#include "AkGameplayStatics.h"

void UAudioManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UAudioManager::Deinitialize()
{
	Super::Deinitialize();
}

void UAudioManager::SetEvent(UAkAudioEvent* SoundEffect, AActor* ActorSource)
{
	if (SoundEffect)
		UAkGameplayStatics::PostEvent(SoundEffect, ActorSource, 0, FOnAkPostEventCallback(), false);
}

void UAudioManager::SetState(UAkStateValue const* StateName)
{
	UAkGameplayStatics::SetState(StateName);
}

void UAudioManager::SetParameter(UAkRtpc const* ParamName, float Value, int32 InterpSpeed)
{
	UAkGameplayStatics::SetRTPCValue(ParamName, Value, InterpSpeed, nullptr);
}