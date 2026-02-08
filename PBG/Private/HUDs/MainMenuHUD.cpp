// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/MainMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/MainMenu/MenuWidget.h"
#include "Widgets/MainMenu/Settings/SettingsWidget.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	MainMenu = CreateWidget<UMenuWidget>(GetWorld(), MainMenuWidgetClass);
	if (MainMenu && !MainMenu->IsInViewport())
	{
		MainMenu->AddToViewport();
	}
	
	SettingsWidget = CreateWidget<USettingsWidget>(GetWorld(), SettingsWidgetClass);
	if (SettingsWidget && !SettingsWidget->IsInViewport())
	{
		SettingsWidget->AddToViewport();
		SettingsWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void AMainMenuHUD::BeginDestroy()
{
	Super::BeginDestroy();
	
	SettingsWidget->RemoveFromParent();
	MainMenu->RemoveFromParent();
}

void AMainMenuHUD::OpenSettings()
{
	if (SettingsWidget->GetVisibility() == ESlateVisibility::Hidden)
	{
		MainMenu->SetVisibility(ESlateVisibility::Hidden);
		SettingsWidget->SetVisibility(ESlateVisibility::Visible);
		SettingsWidget->PlaySettingsAnim();
	}
}

void AMainMenuHUD::OpenMainMenu()
{
	if (SettingsWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		SettingsWidget->SetVisibility(ESlateVisibility::Hidden);
		MainMenu->SetVisibility(ESlateVisibility::Visible);
		MainMenu->PlayMenuAnim();
	}
}