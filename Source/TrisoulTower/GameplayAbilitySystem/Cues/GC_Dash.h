#pragma once

#include "CoreMinimal.h"
#include "AkAudioEvent.h"
#include "GameplayCueNotify_Static.h"
#include "GC_Dash.generated.h"

UCLASS()
class TRISOULTOWER_API UGC_Dash : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

	UPROPERTY(EditAnywhere, Category = Audio)
	UAkAudioEvent* DashSoundEffect;
};
