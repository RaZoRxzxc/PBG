// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Widgets/PlayerWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Public/Characters/BaseCharacter.h"

void UPlayerWidget::SetupCharacter(ABaseCharacter* Character)
{
	Character->OnSprintStateChanged.AddDynamic(this, &UPlayerWidget::OnSprintStateChanged);
	Character->OnSprintMeterUpdated.AddDynamic(this, &UPlayerWidget::OnSprintMeterUpdated);
	Character->OnEquipCameraDelegate.AddDynamic(this, &UPlayerWidget::ShowCameraBox);
	Character->OnCountTimeChangedDelegate.AddDynamic(this, &UPlayerWidget::SetCountUpText);
}

void UPlayerWidget::OnSprintMeterUpdated(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

void UPlayerWidget::OnSprintStateChanged(bool bSprinting)
{
	if (bSprinting && StaminaAnim)
	{
		PlayAnimation(ShowStamina);
		
		if (StaminaBar->GetVisibility() == ESlateVisibility::Collapsed)
		{
			PlayAnimation(StaminaAnim, 0.0f, 0, EUMGSequencePlayMode::Forward, 1.0f, false);
		}
	}
	else
	{
		PlayAnimationReverse(ShowStamina);
		
		if (StaminaBar->GetVisibility() == ESlateVisibility::Visible)
		{
			StopAnimation(StaminaAnim);
		}
	}
}

void UPlayerWidget::SetMicBarValue(float Value)
{
	if (MicVolumeBar)
	{
		MicVolumeBar->SetPercent(Value);
	}
}

void UPlayerWidget::ShowInteractImage(bool bIsInteracting)
{
	if (InteractImage)
	{
		if (bIsInteracting)
		{
			InteractImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InteractImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UPlayerWidget::ShowCameraBox(bool bIsEquip)
{
	if (bIsEquip)
	{
		CameraVerBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CameraVerBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlayerWidget::SetCountUpText(float CountTime)
{
	if (CountText && CountTime >= 0.0f)
	{
		FTimespan TimeSpan = FTimespan::FromSeconds(CountTime);
		FString TimeString = FString::Printf(TEXT("%02d:%02d:%02d"), TimeSpan.GetHours(), TimeSpan.GetMinutes(), TimeSpan.GetSeconds());
		
		CountText->SetText(FText::FromString(TimeString));
	}
}
