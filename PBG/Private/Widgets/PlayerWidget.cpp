// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Widgets/PlayerWidget.h"
#include "Components/ProgressBar.h"
#include "Public/Characters/BaseCharacter.h"

void UPlayerWidget::SetupCharacter(ABaseCharacter* Character)
{
	Character->OnSprintStateChanged.AddDynamic(this, &UPlayerWidget::OnSprintStateChanged);
	Character->OnSprintMeterUpdated.AddDynamic(this, &UPlayerWidget::OnSprintMeterUpdated);
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
		PlayAnimation(StaminaAnim, 0.0f, 0, EUMGSequencePlayMode::Forward, 1.0f, false);
	}
	else
	{
		StopAnimation(StaminaAnim);
	}
}
