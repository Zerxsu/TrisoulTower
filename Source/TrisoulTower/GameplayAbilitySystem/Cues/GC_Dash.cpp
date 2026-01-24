#include "GC_Dash.h"
#include "AudioManager.generated.h"

#include "TrisoulTower/AudioManager.h"

void UGC_Dash::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	if (!MyTarget) return;
	
	if (EventType == EGameplayCueEvent::OnActive)
	{
		// Spawn VFX & Play audio from Audio manager

		UAudioManager* AudioManager = GetWorld()->GetGameInstance()->GetSubsystem<UAudioManager>();

		AudioManager->PlayMusic(DashSoundEffect);
	}
	else if (EventType == EGameplayCueEvent::Removed)
	{
		// Add any VFX or audio after the cue ends
	}
}
