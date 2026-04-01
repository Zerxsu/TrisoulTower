// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataTracker.generated.h"

// This class does not need to be modified.
//UINTERFACE(MinimalAPI)

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class TRISOULTOWER_API UDataTracker : public UObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FString fileName = "GameData.txt";

	UFUNCTION(BlueprintCallable, Category = "File")
	virtual TArray<FString> LoadData();

	UFUNCTION(BlueprintCallable, Category = "File")
	virtual void SaveData(TArray<FString> SaveText);

};
