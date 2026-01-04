// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UPlayerWidget;

UCLASS()
class PBG_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	
	void SetInteractText(FText text);
	
	void ShowInteractBlock();
	void HideInteractBlock();
	
	void SetMicVolumeValue(float Value);
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widgets")
	UPlayerWidget* PlayerWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widgets")
	TSubclassOf<UPlayerWidget> PlayerWidgetClass;
};
