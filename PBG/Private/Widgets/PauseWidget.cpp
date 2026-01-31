// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PauseWidget.h"
#include "Widgets/MainMenu/Settings/SettingsWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
    	PC->bShowMouseCursor = true;
    	PC->SetInputMode(FInputModeUIOnly());
    }

	UnpauseGameBtn->OnClicked.AddDynamic(this, &UPauseWidget::UnpauseGame);
	SettingsBtn->OnClicked.AddDynamic(this, &UPauseWidget::OpenSettings);
	BackToMenuBtn->OnClicked.AddDynamic(this, &UPauseWidget::BackToMenu);
}

void UPauseWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeGameOnly());
		
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void UPauseWidget::UnpauseGame()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		RemoveFromParent();
		
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		
		PC->SetInputMode(FInputModeGameAndUI());
		PC->bShowMouseCursor = false;
		
	}
}

void UPauseWidget::OpenSettings()
{
	if (SettingsWidgetClass)
	{
		SettingsWidget = CreateWidget<USettingsWidget>(GetWorld(), SettingsWidgetClass);
		if (SettingsWidget)
		{
			SettingsWidget->AddToViewport();
		}
	}
}

void UPauseWidget::BackToMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}
