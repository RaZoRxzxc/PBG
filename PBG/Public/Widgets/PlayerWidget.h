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

	/** Called when the character's sprint meter is updated */
	UFUNCTION()
	void OnSprintMeterUpdated(float Percent);

	/** Called when the character's sprint state changes */
	UFUNCTION()
	void OnSprintStateChanged(bool bSprinting);
	
	UFUNCTION(BlueprintCallable)
	UTextBlock* GetInteractText() { return InteractText; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	USizeBox* InteractBox;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	UProgressBar* StaminaBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (BindWidget))
	UTextBlock* InteractText;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* StaminaAnim;
};
