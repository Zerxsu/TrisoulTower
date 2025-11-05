#include "GC_Dash.h"

void UGC_Dash::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	if (!MyTarget) return;
	
	if (EventType == EGameplayCueEvent::OnActive)
	{
		// Spawn VFX & Play audio from Audio manager
	}
	else if (EventType == EGameplayCueEvent::Removed)
	{
		// Add any VFX or audio after the cue ends
	}
}