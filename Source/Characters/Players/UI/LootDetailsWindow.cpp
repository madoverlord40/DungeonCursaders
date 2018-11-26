// Fill out your copyright notice in the Description page of Project Settings.

#include "LootDetailsWindow.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"



void ULootDetailsWindow::SetItemDetails(const FText& Details)
{
	if (IsValid(DetailsTextBox))
	{
		DetailsTextBox->SetText(Details);
	}
}
