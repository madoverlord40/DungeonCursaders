// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryButtonUI.h"
#include "ItemInventoryUI.h"



UInventoryButtonUI::UInventoryButtonUI()
{
	OnHovered.AddDynamic(this, &UInventoryButtonUI::OnHoveredEvent);
	OnClicked.AddDynamic(this, &UInventoryButtonUI::OnClickedEvent);

}

void UInventoryButtonUI::OnHoveredEvent()
{
	FString text = ButtonName.ToString();
	text.Append(" Click To See More Details!");

	ToolTipText = FText::FromString(text);
}

void UInventoryButtonUI::OnClickedEvent()
{
	if (ParentUI.IsValid())
	{
		ParentUI->OnButtonClicked(ArrayItemIndex);
	}
}
