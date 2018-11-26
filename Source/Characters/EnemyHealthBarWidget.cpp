// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyHealthBarWidget.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

void UEnemyHealthBarWidget::UpdateInfo(const FName& Name, const uint8 Level, const float HealthPercent)
{
	if (IsValid(LevelText)) {
		FString DisplayText = FString::Printf(TEXT("%s Level: %i"), *Name.ToString(), Level);

		LevelText->SetText(FText::FromString(DisplayText));
	}

	if (IsValid(HealthProgressBar)) {
		HealthProgressBar->SetPercent(HealthPercent);
	}
}

void UEnemyHealthBarWidget::EnableHighlightText(bool enable)
{
	FLinearColor color = enable ? HighlightTextColor : NormalTextColor;

	LevelText->SetColorAndOpacity(FSlateColor(color));


}
