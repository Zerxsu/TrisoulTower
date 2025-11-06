#include "GC_Dash.h"
#include "NiagaraFunctionLibrary.h"
#include "AudioManager.generated.h"

void UGC_Dash::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	if (!MyTarget) return;
	
	if (EventType == EGameplayCueEvent::OnActive)
	{
		// Spawn VFX & Play audio from Audio manager
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(MyTarget->GetWorld(), DashVFX, MyTarget->GetActorLocation());

		UAudioManager* AudioManager = 
		
	}
	else if (EventType == EGameplayCueEvent::Removed)
	{
		// Add any VFX or audio after the cue ends
	}
}