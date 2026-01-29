// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class ABaseCharacter;
class UProgressBar;
class UTextBlock;
class USizeBox;

UCLASS()
class PBG_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Sets up delegate listeners for the passed character */
	void SetupCharacter(ABaseCharacter* Character);

protected:
	/** Called when the character's sprint meter is updated */
	UFUNCTION()
	void OnSprintMeterUpdated(float Percent);

	/** Called when the character's sprint state changes */
	UFUNCTION()
	void OnSprintStateChanged(bool bSprinting);
	
	UFUNCTION()
	void ShowCameraBox(bool bIsEquip);
	
	UFUNCTION()
	void SetCountUpText(float CountTime);
public:
	UFUNCTION()
	void SetMicBarValue(float Value);
	
	UFUNCTION()
	void ShowInteractImage(bool bIsInteracting);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	UProgressBar* MicVolumeBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	UProgressBar* StaminaBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	class UImage* InteractImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	UTextBlock* CountText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	class UVerticalBox* CameraVerBox;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* StaminaAnim;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* ShowStamina;
};
