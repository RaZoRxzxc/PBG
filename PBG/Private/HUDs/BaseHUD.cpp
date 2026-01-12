// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/HUDs/BaseHUD.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Characters/BaseCharacter.h"
#include "Public/Widgets/PlayerWidget.h"

void ABaseHUD::ShowInteractImage(bool bIsInteract)
{
	if (PlayerWidget)
	{
		PlayerWidget->ShowInteractImage(bIsInteract);
	}
}


void ABaseHUD::SetMicVolumeValue(float Value)
{
	if (PlayerWidget)
	{
		PlayerWidget->SetMicBarValue(Value);
	}
}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerWidgetClass)
	{
		PlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);
		if (PlayerWidget)
		{
			PlayerWidget->AddToViewport();
			
			// Setup player class for stats
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				PlayerWidget->SetupCharacter(Character);
			}
		}
	}
}
