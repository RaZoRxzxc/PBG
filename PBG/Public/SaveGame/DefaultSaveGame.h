// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DefaultSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PBG_API UDefaultSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MasterVolume = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MusicVolume = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float SFXVolume = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float VoiceVolume = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity")
	float MouseSensitivity = 1.0f;
};
