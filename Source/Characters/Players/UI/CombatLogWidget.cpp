// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatLogWidget.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"



void UCombatLogWidget::SetText(const FText& NewText)
{
	if (IsValid(RichText) && IsValid(ScrollBox))
	{
		RichText->SetText(NewText);
		ScrollBox->ScrollToEnd();
	}
}
