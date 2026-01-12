// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Traps/TrapActor.h"

#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ATrapActor::ATrapActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	TrapBoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	TrapBoxCollision->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void ATrapActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup overlap begin functions to collisions
	TrapBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::OnTrapOverlapBegin);
}

void ATrapActor::OnTrapOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		Character->OnDeathRegistered.Broadcast();
	}
}

// Called every frame
void ATrapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

