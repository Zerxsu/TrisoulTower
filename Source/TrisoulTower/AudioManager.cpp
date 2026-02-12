// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioManager.h"

#include "AkGameplayStatics.h"

void UAudioManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UAudioManager::Deinitialize()
{
	StopMusic();
	Super::Deinitialize();
}

void UAudioManager::PlaySFX(UAkAudioEvent* SoundEffect, AActor* ActorSource)
{
	if (SoundEffect)
	{
		UAkGameplayStatics::PostEvent(SoundEffect, ActorSource, 0, FOnAkPostEventCallback(), false);
	}
}

void UAudioManager::PlayMusic(UAkAudioEvent* Music)
{
	if (Music)
	{
		StopMusic();
		UAkGameplayStatics::PostEvent(Music, nullptr, 0, FOnAkPostEventCallback(), false);
	}
}

void UAudioManager::StopMusic()
{
	// no function to stop music in Wwise documentation
	// Will most likely have to stop music using a parameter in Wwise project
}

void UAudioManager::SetState(UAkStateValue const* StateName)
{
	UAkGameplayStatics::SetState(StateName);
}

void UAudioManager::SetParameter(UAkRtpc const* ParamName, float Value, int32 InterpSpeed)
{
	UAkGameplayStatics::SetRTPCValue(ParamName, Value, InterpSpeed, nullptr);
}