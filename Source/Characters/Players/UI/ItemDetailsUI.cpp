// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemDetailsUI.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "ItemInventoryUI.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"


void UItemDetailsUI::SetDetailsText(const FText& Text, const FText& Title, bool useable)
{
	if (IsValid(DetailsTextBlock))
	{
		DetailsTextBlock->SetText(Text);

	}
	if (IsValid(TitleTextBlock))
	{
		TitleTextBlock->SetText(Title);
	}

	if (IsValid(UseThisButton))
	{
		UseThisButton->SetIsEnabled(useable);
	}
}

void UItemDetailsUI::DeleteOwningItem()
{
	if (InventoryWindow.IsValid())
	{
		//call down to inventory window we want to delete this..
	}
}

void UItemDetailsUI::UseThisItem()
{
	//call down to inventory window we want to use this..
}
