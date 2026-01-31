// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton;
class USettingsWidget;

UCLASS()
class PBG_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UnpauseGame();
	
protected:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void OpenSettings();
	
	UFUNCTION()
	void BackToMenu();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	USettingsWidget* SettingsWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TSubclassOf<USettingsWidget> SettingsWidgetClass;
	
private:
	
	UPROPERTY(meta = (BindWidget))
	UButton* UnpauseGameBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BackToMenuBtn;
};
