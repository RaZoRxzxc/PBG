// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractableActor.h"

void AInteractableActor::UseItem_Implementation()
{
	IInteractInterface::UseItem_Implementation();
	
	
}

// Sets default values
AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	Collision->SetupAttachment(Mesh);
}

void AInteractableActor::Interact_Implementation(ACharacter* Character)
{
	IInteractInterface::Interact_Implementation(Character);
	
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

