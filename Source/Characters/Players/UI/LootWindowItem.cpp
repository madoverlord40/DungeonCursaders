// Fill out your copyright notice in the Description page of Project Settings.

#include "LootWindowItem.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Border.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "TreasureLootWindow.h"


void ULootWindowItem::SetItemInfo(const FName& KeyName, UTreasureLootWindow* ParentWindow, const FText& text, UTexture2D* IconImage)
{
	if (IsValid(ParentWindow))
	{
		ParentUI = &(*ParentWindow);

		if (IsValid(ItemText))
		{
			ItemText->SetText(text);
		}

		if (IsValid(ItemIcon))
		{
			ItemIcon->SetBrushFromTexture(IconImage);
		}

		ItemKeyID = KeyName;
	}
}

void ULootWindowItem::SetEnableSelection(bool enable)
{
	bIsSelected = enable;

	if (!bIsSelected)
	{
		if (IsValid(ItemBorder))
		{
			bIsSelected = false;
			ItemBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
			ItemBorder->SetContentColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	else
	{
		if (IsValid(ItemBorder))
		{
			bIsSelected = true;
			ItemBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			ItemBorder->SetContentColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}
}

void ULootWindowItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (IsValid(ItemBorder))
	{
		ItemBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void ULootWindowItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (IsValid(ItemBorder) && !bIsSelected)
	{
		ItemBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
	}
}

FReply ULootWindowItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ParentUI.IsValid())
	{
		ParentUI->SetCurrentSelectedItem(ItemKeyID);
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
