// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Traps/FloorTrap.h"
#include "Components/BoxComponent.h"

AFloorTrap::AFloorTrap()
{
	TriggerBoxCollision = CreateDefaultSubobject<UBoxComponent>("TriggerBoxCollision");
	TriggerBoxCollision->SetupAttachment(Mesh);
}

void AFloorTrap::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFloorTrap::OnTriggerOverlapBegin);
}

void AFloorTrap::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, -1.0f));
	}
}
