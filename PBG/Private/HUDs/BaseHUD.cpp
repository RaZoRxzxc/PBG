// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/HUDs/BaseHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Characters/BaseCharacter.h"
#include "Public/Widgets/PlayerWidget.h"

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
