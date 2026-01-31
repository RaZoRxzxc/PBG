// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/MenuWidget.h"
#include "Widgets/MainMenu/Settings/SettingsWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = true;
	}
	
	if (PlayButton)
		PlayButton->OnClicked.AddDynamic(this, &UMenuWidget::PlayGame);
		PlayBtnText->SetText(FText::FromString("Start game"));
	
	if (SettingsButton)
		SettingsButton->OnClicked.AddDynamic(this, &UMenuWidget::OpenSettings);
		SettingsBtnText->SetText(FText::FromString("Settings"));
	
	if (QuitButton)
		QuitButton->OnClicked.AddDynamic(this, &UMenuWidget::QuitGame);
		QuitBtnText->SetText(FText::FromString("Quit"));
}

void UMenuWidget::PlayGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UMenuWidget::OpenSettings()
{
	if (SettingsWidgetClass)
	{
		SettingsWidget = CreateWidget<USettingsWidget>(GetWorld(), SettingsWidgetClass);
		
		if (SettingsWidget)
		{
			this->RemoveFromParent();
			SettingsWidget->AddToViewport();
		}
	}
}

void UMenuWidget::QuitGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}
