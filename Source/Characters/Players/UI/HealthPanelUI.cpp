// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPanelUI.h"

void UHealthPanelUI::SetHealthPercent(const float health)
{
	NewHealthValue = health;

}

void UHealthPanelUI::SetStaminaPercent(const float stamina)
{
	NewStaminaValue = stamina;
}

void UHealthPanelUI::SetManaPercent(const float mana)
{
	NewManaValue = mana;
}

void UHealthPanelUI::SetRagePercent(const float rage)
{
	NewRageValue = rage;
}

void UHealthPanelUI::SetCharacterName(const FName & Name)
{
	if (IsValid(NameTextBlock)) {
		FText text = FText::FromName(Name);
		NameTextBlock->SetText(text);
	}
}

void UHealthPanelUI::SetExpValue(const float exp)
{
	if (IsValid(ExpTextBlock)) {
		FText text = FText::FromName(FName(*FString::SanitizeFloat(exp, 0)));
		ExpTextBlock->SetText(text);
	}
}

void UHealthPanelUI::SetLevelValue(const uint8 level)
{
	if (IsValid(LevelTextBlock)) {
		FString level_string = FString::FromInt(level);
		FText text = FText::FromName(FName(*level_string));
		LevelTextBlock->SetText(text);
	}
}


void UHealthPanelUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHealthPanelUI::NativeDestruct()
{
	Super::NativeDestruct();
}

void UHealthPanelUI::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(HealthProgBar) && !FMath::IsNearlyEqual(HealthProgBar->Percent, NewHealthValue)) {
		float value = FMath::FInterpTo(HealthProgBar->Percent, NewHealthValue, InDeltaTime, 1.0f);
		HealthProgBar->SetPercent(value);
	}

	if (IsValid(StaminaProgBar) && !FMath::IsNearlyEqual(StaminaProgBar->Percent, NewStaminaValue)) {
		float value = FMath::FInterpTo(StaminaProgBar->Percent, NewStaminaValue, InDeltaTime, 1.0f);
		StaminaProgBar->SetPercent(value);
	}
	if (IsValid(ManaProgBar) && !FMath::IsNearlyEqual(ManaProgBar->Percent, NewManaValue)) {
		float value = FMath::FInterpTo(ManaProgBar->Percent, NewManaValue, InDeltaTime, 1.0f);
		ManaProgBar->SetPercent(value);
	}
	if (IsValid(RageProgBar) && !FMath::IsNearlyEqual(RageProgBar->Percent, NewRageValue)) {
		float value = FMath::FInterpTo(RageProgBar->Percent, NewRageValue, InDeltaTime, 1.0f);
		RageProgBar->SetPercent(value);
	}
}
