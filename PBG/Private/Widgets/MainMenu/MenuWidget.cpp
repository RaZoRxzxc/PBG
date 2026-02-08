// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/MenuWidget.h"
#include "Widgets/MainMenu/Settings/SettingsWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "HUDs/MainMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::PlayMenuAnim()
{
	if (OpenAnim)
		PlayAnimation(OpenAnim);
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayMenuAnim();
}

void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	HUD = Cast<AMainMenuHUD>( GetWorld()->GetFirstPlayerController()->GetHUD());
	
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
	if (HUD)
	{
		HUD->OpenSettings();
	}
}

void UMenuWidget::QuitGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}

void UMenuWidget::OnSettingsClosedHandler()
{
	AddToViewport();
}
