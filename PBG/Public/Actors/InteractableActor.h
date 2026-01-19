// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "InteractableActor.generated.h"

UCLASS()
class PBG_API AInteractableActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	void Interact_Implementation(ABaseCharacter* Character) override;
	void UseItem_Implementation() override;
	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Base actor mesh 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UBoxComponent* Collision;
	
	bool bIsEquip = false;
public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
