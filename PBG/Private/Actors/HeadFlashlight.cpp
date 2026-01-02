// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HeadFlashlight.h"

#include "Components/SpotLightComponent.h"
#include "GameFramework/Character.h"

AHeadFlashlight::AHeadFlashlight()
{
	Light = CreateDefaultSubobject<USpotLightComponent>("FlashLight");
	Light->SetupAttachment(Mesh);
}

void AHeadFlashlight::Interact_Implementation(ACharacter* Character)
{
	Super::Interact_Implementation(Character);
	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact flashlight"));
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FlashLight_Socket"));
		Collision->SetCollisionProfileName(TEXT("NoCollision"));
	}
}
