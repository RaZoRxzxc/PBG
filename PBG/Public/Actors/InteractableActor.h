// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "InteractableActor.generated.h"

UCLASS()
class PBG_API AInteractableActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	void Interact_Implementation() override;
	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Base actor mesh 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	// Name for interact widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FText ActorName;
	
public:	
	
	UFUNCTION(BlueprintCallable)
	FText GetActorName() const { return ActorName; }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
