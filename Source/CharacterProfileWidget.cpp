// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterProfileWidget.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"

void UCharacterProfileWidget::SetNameAndClassInfo(const FName& CharacterName, const FName& CharacterClass, const bool DualWeilding, const bool TwoHanded)
{
	
	if (NameText)
	{
		NameText->SetText(FText::FromName(CharacterName));
	}

	if (ClassText)
	{
		ClassText->SetText(FText::FromName(CharacterClass));
	}

	if (DualWeildingText)
	{
		FName dual = (DualWeilding ? FName("YES") : FName("NO"));
		DualWeildingText->SetText(FText::FromName(dual));
	}

	if (TwoHandedText)
	{
		FName twohand = (TwoHanded ? FName("YES") : FName("NO"));
		TwoHandedText->SetText(FText::FromName(twohand));
	}
}

void UCharacterProfileWidget::SetInternalInfo(const FInternalStats * Internals)
{
	if (Internals)
	{
		FString value;

		if (StrengthText)
		{
			value = FString::SanitizeFloat(Internals->Strength);
			StrengthText->SetText(FText::FromString(value));
		}

		if (VitalityText)
		{
			value = FString::SanitizeFloat(Internals->Vitality);
			VitalityText->SetText(FText::FromString(value));
		}

		if (DexterityText)
		{
			value = FString::SanitizeFloat(Internals->Dexterity);
			DexterityText->SetText(FText::FromString(value));
		}

		if (AgilityText)
		{
			value = FString::SanitizeFloat(Internals->Agility);
			AgilityText->SetText(FText::FromString(value));
		}

		if (MindText)
		{
			value = FString::SanitizeFloat(Internals->Mind);
			MindText->SetText(FText::FromString(value));
		}

		if (IntellectText)
		{
			value = FString::SanitizeFloat(Internals->Intellect);
			IntellectText->SetText(FText::FromString(value));
		}
	}
}

void UCharacterProfileWidget::SetExternalInfo(const FExternalStats * Externals)
{
	if (Externals)
	{
		FString value;

		if (HPText)
		{
			value = FString::SanitizeFloat(Externals->HealthPointsMax);
			HPText->SetText(FText::FromString(value));
		}

		if (AttackText)
		{
			value = FString::SanitizeFloat(Externals->AttackPower);
			AttackText->SetText(FText::FromString(value));
		}

		if (DefenseText)
		{
			value = FString::SanitizeFloat(Externals->DefensePower);
			DefenseText->SetText(FText::FromString(value));
		}

		if (EvasionText)
		{
			value = FString::SanitizeFloat(Externals->Evasion);
			EvasionText->SetText(FText::FromString(value));
		}

		if (MagicPowerText)
		{
			value = FString::SanitizeFloat(Externals->MagicPower);
			MagicPowerText->SetText(FText::FromString(value));
		}

		if (MagicDefenseText)
		{
			value = FString::SanitizeFloat(Externals->MagicDefense);
			MagicDefenseText->SetText(FText::FromString(value));
		}

		if (AttackAccText)
		{
			value = FString::SanitizeFloat(Externals->AttackAcc);
			AttackAccText->SetText(FText::FromString(value));
		}
	}
}

void UCharacterProfileWidget::SetProfileCanvasVisibility(ESlateVisibility Visibility)
{
	ProfileStatsPanel->SetVisibility(Visibility);
}

void UCharacterProfileWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UCharacterProfileWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCharacterProfileWidget::NativeDestruct()
{
	Super::NativeDestruct();

}

void UCharacterProfileWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}
