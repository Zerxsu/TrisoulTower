// Fill out your copyright notice in the Description page of Project Settings.

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "DataTracker.h"

// Add default functionality here for any IDataTracker functions that are not pure virtual.

TArray<FString> UDataTracker::LoadData() {
    TArray<FString> FileText = {};

    FString FilePath = FPaths::ProjectDir() + fileName;
    if (!FFileHelper::LoadFileToStringArray(FileText, *FilePath)) {
        SaveData(FileText);
    }


    return FileText;
}


void UDataTracker::SaveData(TArray<FString> SaveText) {
    
    FString newData = "";

    for (const FString& data: SaveText) {
        newData += data;
    }

    FString SavePath = FPaths::ProjectDir() + fileName;
    FFileHelper::SaveStringToFile(newData, *SavePath);

}