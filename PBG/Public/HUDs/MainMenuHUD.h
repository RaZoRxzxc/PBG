// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class UMenuWidget;
class USettingsWidget;

UCLASS()
class PBG_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void OpenSettings();
	
	UFUNCTION()
	void OpenMainMenu();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void BeginDestroy() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	USettingsWidget* SettingsWidget;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	TSubclassOf<UUserWidget> SettingsWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainMenu)
	UMenuWidget* MainMenu;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainMenu)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
};
