// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityButton.h"
#include "Characters/Players/UI/PlayerAbilityBar.h"

UAbilityButton::UAbilityButton()
{
	OnHovered.AddDynamic(this, &UAbilityButton::OnHoveredEvent);
	OnClicked.AddDynamic(this, &UAbilityButton::OnClickedEvent);
}

void UAbilityButton::OnHoveredEvent()
{
}

void UAbilityButton::OnClickedEvent()
{
	if (ParentUI.IsValid())
	{
		ParentUI->OnAbilityButtonClick(AbilitySlotName);
	}
}
