// Fill out your copyright notice in the Description page of Project Settings.

#include "EquippedItemButtonUI.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Components/Border.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "Characters/Players/Components/InventoryComponent.h"
#include "PlayerCharacterStatsWidget.h"
#include "UsableItemButton.h"

UEquippedItemButtonUI::UEquippedItemButtonUI(const FObjectInitializer& Initializer) : Super(Initializer)
{

}

void UEquippedItemButtonUI::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (IsValid(BorderButton) && IsValid(ItemInfo))
	{
		
		BorderButton->SetBrushColor(FLinearColor(1.5f, 1.5f, 1.5f));
	}
}

void UEquippedItemButtonUI::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	if (IsValid(BorderButton) && IsValid(ItemInfo))
	{

		BorderButton->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f));
	}
}

FReply UEquippedItemButtonUI::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (IsValid(ItemInfo))
	{
		FEventReply reply;

		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

		return reply.NativeReply;
	}
	return FReply::Handled();
}

FReply UEquippedItemButtonUI::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	
	return FReply::Handled();
}

FReply UEquippedItemButtonUI::NativeOnMouseButtonUp(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	return FReply::Handled();
}

void UEquippedItemButtonUI::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());

	UUsableItemButton* DragWidget = CreateWidget<UUsableItemButton>(this, UsableItemClass);
	DragWidget->AddToViewport();

	DragWidget->SetItemInfo(&ItemInfo->InventoryInfo, ParentWidgetUI, true);
	DragWidget->SetRenderScale(FVector2D(0.5f, 0.5f));
	DragWidget->SetPositionInViewport(FVector2D(-150.0f, -150.0f));

	OutOperation->DefaultDragVisual = DragWidget;

	ItemInfo->DragWidget = DragWidget;
	OutOperation->Payload = ItemInfo;

	BorderButton->SetVisibility(ESlateVisibility::Hidden);
}

bool UEquippedItemButtonUI::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	bool success = false;

	if (InOperation)
	{
		if (IsValid(InOperation->Payload))
		{
			UInventoryItemObject* InventoryObject = Cast<UInventoryItemObject>(InOperation->Payload);

			if (IsValid(InventoryObject) && InventoryObject->SelfWidget != this)
			{
				if (IsValid(ItemInfo))
				{
					ItemInfo->InventoryInfo = InventoryObject->InventoryInfo;

				}

				if (InventoryObject->InventoryInfo.Type == ItemType && InventoryObject->InventoryInfo.ArmorSlotType == ArmorSlot)
				{
					UItemInventoryUI* InventoryParent = Cast<UItemInventoryUI>(InventoryObject->DragFromParentWidget);
					UPlayerCharacterStatsWidget* PlayerStatsWidget = Cast<UPlayerCharacterStatsWidget>(InventoryObject->DragFromParentWidget);

					if (IsValid(InventoryParent))
					{
						PlayerUI = &(*InventoryParent->GetParentUI());

						if (IsValid(PlayerUI))
						{
							PlayerUI->UnEquipItemFromDragDrop(&InventoryObject->InventoryInfo);

							if (PlayerUI->EquipItemFromDragDrop(&InventoryObject->InventoryInfo))
							{
								bIsWeapon = InventoryObject->InventoryInfo.Type == ItemTypes::ITEM_EQUIPABLE_WEAPON;
								SetButtonInfo(&InventoryObject->InventoryInfo, ParentWidgetUI, bIsWeapon);
								
								InventoryObject->DragWidget->RemoveFromViewport();
								InventoryParent->RemoveItemFromUI(InventoryObject->InventoryInfo.ButtonIndex);

								UUsableItemButton* DragFromButton = Cast<UUsableItemButton>(InventoryObject->SelfWidget);
								
								if (IsValid(DragFromButton))
								{
									DragFromButton->SetItemInfo(nullptr, nullptr);
									APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController());
									if (IsValid(PlayerController))
									{
										ParentWidgetUI->SetPropertyValues(PlayerController->GetCharacterCombatData());
									}
								}
								success = true;
							}
						}
					}
				}
			}
			else if (InventoryObject->SelfWidget == this)
			{
				InventoryObject->DragWidget->RemoveFromViewport();
				BorderButton->SetVisibility(ESlateVisibility::Visible);
			}

		}
	}
	return success;
}

void UEquippedItemButtonUI::NativeOnDragCancelled(const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	BorderButton->SetVisibility(ESlateVisibility::Visible);

	if (InOperation)
	{
		if (IsValid(InOperation->Payload))
		{
			UInventoryItemObject* ItemObject = Cast<UInventoryItemObject>(InOperation->Payload);

			if (IsValid(ItemObject))
			{
				ItemObject->DragWidget->RemoveFromViewport();
				BorderButton->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UEquippedItemButtonUI::SetButtonInfo(FInventoryItemInfo* Info, UUserWidget* Parent, uint32 CanDelete, bool IsWeapon /*= false*/)
{

	if (Info && IsValid(BorderButton) && !IsValid(ItemInfo))
	{
		ItemInfo = NewObject<UInventoryItemObject>(this, UInventoryItemObject::StaticClass());
	}
	
	if (Info && IsValid(ItemInfo) && IsValid(BorderButton))
	{
		ItemInfo->InventoryInfo = *Info;
		ItemInfo->DragFromParentWidget = Parent;
		ItemInfo->SelfWidget = this;

		PlayerUI = Cast<UPlayerWidgetHUD>(Parent);
		UTexture2D* Texture = Info->ItemIconTexture.Get();
		BorderButton->SetBrushFromTexture(Texture);
		BorderButton->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));

		SetToolTipText(FText::FromName(Info->ItemName));
		BorderButton->SetVisibility(ESlateVisibility::Visible);
		this->SetVisibility(ESlateVisibility::Visible);
	}

	if (Info == nullptr)
	{
		BorderButton->SetBrushFromTexture(nullptr);
		BorderButton->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.0f));
		ItemInfo->ConditionalBeginDestroy();
		ItemInfo = nullptr;
		SetToolTipText(FText());
	}
}
