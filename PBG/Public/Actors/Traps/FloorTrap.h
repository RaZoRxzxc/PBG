// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Traps/TrapActor.h"
#include "FloorTrap.generated.h"

/**
 * 
 */
UCLASS()
class PBG_API AFloorTrap : public ATrapActor
{
	GENERATED_BODY()
	
	AFloorTrap();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision");
	UBoxComponent* TriggerBoxCollision;
	
	UFUNCTION()
	virtual void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
