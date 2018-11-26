// Fill out your copyright notice in the Description page of Project Settings.

#include "TreasureLootWindow.h"
#include "LootWindowItem.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "LootDetailsWindow.h"
#include "PlayerWidgetHUD.h"


void UTreasureLootWindow::SetCurrentSelectedItem(FName& ID)
{
	ULootWindowItem* SelectedItem = *LootWindowItems.Find(ID);

	if (IsValid(SelectedItem))
	{
		CurrentSelectedID = ID;

		if (IsValid(ItemDetailsButton))
		{
			ItemDetailsButton->SetIsEnabled(true);
		}

		if (IsValid(LastSelectedItem))
		{
			LastSelectedItem->SetEnableSelection(false);
		}

		SelectedItem->SetEnableSelection(true);

		LastSelectedItem = &(*SelectedItem);
	}
}

void UTreasureLootWindow::AddLootItemToWindow(const FName& KeyName, const FText& text, UTexture2D* IconImage, const FString& Details)
{
	ULootWindowItem* WindowItem = CreateWidget<ULootWindowItem>(this, LootWindowItemClass);
	
	if (IsValid(WindowItem))
	{
		ScrollBox->AddChild(WindowItem);

		LootWindowItems.Add(KeyName, WindowItem);
		LootWindowDetails.Add(KeyName, Details);

		WindowItem->SetItemInfo(KeyName, this, text, IconImage);
		
	}
}

void UTreasureLootWindow::SetDetailsWindowPtr(ULootDetailsWindow* Window)
{
	if (Window)
	{
		DetailsWindow = &(*Window);
	}
}

void UTreasureLootWindow::SetParentWindowPtr(UPlayerWidgetHUD* Window)
{
	ParentUI = &(*Window);
}

void UTreasureLootWindow::ShowDetailsForSelectedItem()
{
	if (IsValid(DetailsWindow) && LootWindowDetails.Find(CurrentSelectedID))
	{
		const FString& Details = *LootWindowDetails.Find(CurrentSelectedID);

		DetailsWindow->SetItemDetails(FText::FromString(Details));
		DetailsWindow->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTreasureLootWindow::OnNotifyTakeSelectedItems()
{
	if (IsValid(ParentUI))
	{
		DetailsWindow->SetVisibility(ESlateVisibility::Hidden);
		TArray<FName> KeyNames;

		for (TPair<FName, ULootWindowItem*>& Pair : LootWindowItems)
		{
			ULootWindowItem* LootItem = Pair.Value;
			const FName Key = Pair.Key;
			
			if (LootItem->GetIsItemChecked())
			{
				KeyNames.Add(Key);
				ScrollBox->RemoveChild(LootItem);
			}
		}

		ParentUI->TakeSelectedLoot(KeyNames);		
	}
}

void UTreasureLootWindow::OnNotifyTakeAllItems()
{
	if (IsValid(ParentUI))
	{
		DetailsWindow->SetVisibility(ESlateVisibility::Hidden);
		TArray<FName> KeyNames;

		for (TPair<FName, ULootWindowItem*>& Pair : LootWindowItems)
		{
			const ULootWindowItem* LootItem = Pair.Value;
			const FName Key = Pair.Key;

			KeyNames.Add(Key);			
		}

		ParentUI->TakeSelectedLoot(KeyNames);
		ScrollBox->ClearChildren();
		LootWindowItems.Empty();
	}
}

void UTreasureLootWindow::OnNotifyCloseWindow()
{
	if (IsValid(ParentUI))
	{
		DetailsWindow->SetVisibility(ESlateVisibility::Hidden);
		const TMap<FName, TSubclassOf<UItemObject>> LootItems;
		ParentUI->ShowHideChestLootWindow(LootItems, false);
		ScrollBox->ClearChildren();
		LootWindowItems.Empty();
	}
}
