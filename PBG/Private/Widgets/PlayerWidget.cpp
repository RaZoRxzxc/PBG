// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Widgets/PlayerWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Public/Characters/BaseCharacter.h"

void UPlayerWidget::SetupCharacter(ABaseCharacter* Character)
{
	Character->OnSprintStateChanged.AddDynamic(this, &UPlayerWidget::OnSprintStateChanged);
	Character->OnSprintMeterUpdated.AddDynamic(this, &UPlayerWidget::OnSprintMeterUpdated);
	Character->OnEquipCameraDelegate.AddDynamic(this, &UPlayerWidget::ShowMicBar);
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

void UPlayerWidget::ShowMicBar(bool bIsEquip)
{
	if (bIsEquip)
	{
		MicVolumeBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MicVolumeBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}
