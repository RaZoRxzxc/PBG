// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActor.h"
#include "HeadFlashlight.generated.h"

/**
 * 
 */
UCLASS()
class PBG_API AHeadFlashlight : public AInteractableActor
{
	GENERATED_BODY()
	
	AHeadFlashlight();
public:
	
	void Interact_Implementation(ACharacter* Character) override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light")
	class USpotLightComponent* Light;
};
