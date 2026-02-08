// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UButton;
class UTextBlock;
class USettingsWidget;

UCLASS()
class PBG_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	void PlayMenuAnim();
	
protected:
	
	class AMainMenuHUD* HUD = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StartGame")
	FName LevelName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* PlayButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PlayBtnText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SettingsButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SettingsBtnText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* QuitButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* QuitBtnText;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* OpenAnim;
	
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void PlayGame();
	
	UFUNCTION()
	void OpenSettings();
	
	UFUNCTION()
	void QuitGame();
	
	UFUNCTION()
	void OnSettingsClosedHandler();
};
