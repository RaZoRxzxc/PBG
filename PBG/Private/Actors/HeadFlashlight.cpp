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
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FlashLight_Socket"));
		Collision->SetCollisionProfileName(TEXT("NoCollision"));
		bIsEquip = true;
	}
}

void AHeadFlashlight::UseItem_Implementation()
{
	Super::UseItem_Implementation();
	
	if (bIsEquip)
	{
		if (!bLightOn)
		{
			LightOn();
		}
		else
		{
			LightOff();
		}
	}
}

void AHeadFlashlight::LightOn()
{
	Light->SetIntensity(5000);
	bLightOn = true;
}

void AHeadFlashlight::LightOff()
{
	Light->SetIntensity(0);
	bLightOn = false;
}
