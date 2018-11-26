// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemInventoryUI.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Border.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "UsableItemButton.h"
#include "PlayerWidgetHUD.h"

UItemInventoryUI::UItemInventoryUI(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{

}

void UItemInventoryUI::DisplayInventoryItems(TArray<FInventoryItemInfo>& ItemsToShow)
{
	if (ItemsToShow.Num() > 0 && InventoryButtons.Num() >= ItemsToShow.Num())
	{
		InventoryItems.Empty();

		UUsableItemButton* ItemButton = nullptr;

		for (int index = 0; index < InventoryButtons.Num(); index++)
		{
			if (index < ItemsToShow.Num())
			{
				if (ItemsToShow[index].ButtonIndex != 0)
				{
					uint8 ButtonIndex = ItemsToShow[index].ButtonIndex;
					if (InventoryButtons[ButtonIndex]->ButtonIndex == ButtonIndex)
					{
						ItemButton = InventoryButtons[ButtonIndex];
					}
				}
				else if (ItemsToShow[index].ButtonIndex == 0 && index > 0)
				{
					ItemsToShow[index].ButtonIndex = index;
					ItemButton = InventoryButtons[index];
				}

				if (IsValid(ItemButton))
				{
					InventoryItems.Add(ItemsToShow[index]);
					ItemButton->SetItemInfo(&ItemsToShow[index], ParentWindow, true);
				}
			}
			
			
		}
	}
}

void UItemInventoryUI::OnButtonClicked(uint8 ItemIndex)
{
	if (IsValid(ParentWindow))
	{
		ParentWindow->ShowDetailsBox(FText::FromString(InventoryItems[ItemIndex].ItemDescription), FText::FromName(InventoryItems[ItemIndex].ItemName), InventoryItems[ItemIndex].bIsUseable);
	}
}

void UItemInventoryUI::RemoveItemFromUI(const uint8 index)
{
	if (InventoryItems.Num() > index)
	{
		FInventoryItemInfo* ItemInfo = &InventoryItems[index];

		if (ItemInfo)
		{
			UUsableItemButton* ItemButton = InventoryButtons[index];

			if (IsValid(ItemButton))
			{
				InventoryItems.RemoveAt(index);
				ItemButton->SetItemInfo(nullptr, nullptr);
			}
		}
	}
}

void UItemInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

}

void UItemInventoryUI::NativeDestruct()
{
	Super::NativeDestruct();

}

void UItemInventoryUI::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}
