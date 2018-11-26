// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableItemButton.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UsableItemsBar.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "Characters/Players/Components/InventoryComponent.h"
#include "PlayerCharacterStatsWidget.h"
#include "EquippedItemButtonUI.h"

UUsableItemButton::UUsableItemButton(const FObjectInitializer& Initializer) : Super(Initializer)
{
	
}

void UUsableItemButton::NativeOnMouseEnter(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	if (IsValid(ButtonBorder) && IsValid(ItemInfo))
	{
		ButtonBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UUsableItemButton::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	if (IsValid(ButtonBorder) && IsValid(NormalImage) && IsValid(ItemInfo))
	{
		ButtonBorder->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
	}
}

FReply UUsableItemButton::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	return FReply::Handled();
}

FReply UUsableItemButton::NativeOnPreviewMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	if (IsValid(ItemInfo))
	{
		FEventReply reply;

		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

		return reply.NativeReply;
	}

	return FReply::Handled();
}

FReply UUsableItemButton::NativeOnMouseButtonUp(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	UItemInventoryUI* InventoryUI = Cast<UItemInventoryUI>(ParentWidgetUI);

	if (IsValid(InventoryUI) && IsValid(ItemInfo))
	{
		//ItemInfo->InventoryInfo.Type
		InventoryUI->OnButtonClicked(ItemInfo->InventoryInfo.ButtonIndex);
	}
	else if (IsValid(ItemInfo))
	{
		APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController());
		if (IsValid(PlayerController))
		{
			if (PlayerController->UseInventoryItem(ItemInfo->InventoryInfo))
			{
				uint8 count = PlayerController->GetInventoryComponent()->GetStackCount(ItemInfo->InventoryInfo.InventoryKeyName, ItemInfo->InventoryInfo.Type);
				if (count == 0)
				{
					SetItemInfo(nullptr, nullptr);
				}
			}
		}
	}
	return FReply::Handled();
}

void UUsableItemButton::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(DragOpClass);

	UUsableItemButton* DragWidget = CreateWidget<UUsableItemButton>(this, DragWidgetClass);
	DragWidget->AddToViewport();

	DragWidget->SetRenderScale(FVector2D(0.5f, 0.5f));
	DragWidget->SetItemInfo(&ItemInfo->InventoryInfo, ParentWidgetUI, true);
	DragWidget->SetPositionInViewport(FVector2D(-150.0f, -150.0f));	

	OutOperation->DefaultDragVisual = DragWidget;

	ItemInfo->DragWidget = DragWidget;
	OutOperation->Payload = ItemInfo;

	ButtonBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UUsableItemButton::NativeOnDragEnter(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	if (InOperation)
	{
		
	}
}

void UUsableItemButton::NativeOnDragLeave(const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	if (InOperation)
	{
		
	}
}

bool UUsableItemButton::NativeOnDragOver(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	return true;
}

bool UUsableItemButton::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	if (InOperation)
	{
		UInventoryItemObject* InventoryObject = Cast<UInventoryItemObject>(InOperation->Payload);

		if (IsValid(InventoryObject))
		{
			if (IsValid(ItemInfo))
			{
				ItemInfo->InventoryInfo = InventoryObject->InventoryInfo;
				ItemInfo->InventoryInfo.ButtonIndex = ButtonIndex;
			}
			else
			{
				InventoryObject->InventoryInfo.ButtonIndex = ButtonIndex;
				SetItemInfo(&InventoryObject->InventoryInfo, ParentWidgetUI);
			}

			InventoryObject->DragWidget->RemoveFromViewport();

			UPlayerCharacterStatsWidget* PlayerStatsWidget = Cast<UPlayerCharacterStatsWidget>(InventoryObject->DragFromParentWidget);
			if (IsValid(PlayerStatsWidget))
			{
				if (InventoryObject->SelfWidget != this)
				{
					UEquippedItemButtonUI* OldButton = Cast<UEquippedItemButtonUI>(InventoryObject->SelfWidget);
					if (IsValid(OldButton))
					{
						APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController());
						if (IsValid(PlayerController))
						{
							PlayerController->UnEquipItem(&InventoryObject->InventoryInfo);
		
							FName InventoryName = ItemInfo->InventoryInfo.InventoryKeyName;

							if (InventoryName.Compare(NAME_None) == 0)
							{
								InventoryName = InventoryObject->InventoryInfo.ItemName;
							}
							if (PlayerController->AddLootableItemToInventory(InventoryName, InventoryObject->InventoryInfo.ItemClass))
							{
								OldButton->SetButtonInfo(nullptr, nullptr, false);
								
							}

						}
					}
				}
			}
			else
			{
				UItemInventoryUI* InventoryParent = Cast<UItemInventoryUI>(ParentWidgetUI);
				UUsableItemsBar* UsableItemsParent = Cast<UUsableItemsBar>(ParentWidgetUI);

				if (IsValid(InventoryParent))
				{
					if (InventoryObject->SelfWidget != this)
					{
						UUsableItemButton* OldButton = Cast<UUsableItemButton>(InventoryObject->SelfWidget);
						if (IsValid(OldButton))
						{
							OldButton->SetItemInfo(nullptr, nullptr);
						}

						APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController());
						if (IsValid(PlayerController))
						{
							PlayerController->UpdateInventoryItemButtonIndex(ItemInfo->InventoryInfo.InventoryKeyName, ButtonIndex);

						}
					}
				}
				else if (IsValid(UsableItemsParent))
				{
					ButtonBorder->SetVisibility(ESlateVisibility::Visible);
					ButtonBorder->SetBrushFromTexture(NormalImage);
					ButtonBorder->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
				}
			}
		}
			
	}
	return false;
}

void UUsableItemButton::NativeOnDragCancelled(const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	if (InOperation)
	{
		ButtonBorder->SetVisibility(ESlateVisibility::Visible);
		ButtonBorder->SetBrushFromTexture(NormalImage);
	}
}

void UUsableItemButton::SetItemInfo(const FInventoryItemInfo* Info, UUserWidget* Parent, uint32 CanDelete)
{
	
	if (Info && IsValid(ButtonBorder))
	{
		ItemInfo = NewObject<UInventoryItemObject>(this, UInventoryItemObject::StaticClass());
		ItemInfo->InventoryInfo = *Info;
		ItemInfo->bCanDeleteOnDrop = CanDelete;
		ItemInfo->DragFromParentWidget = Parent;
		ItemInfo->SelfWidget = this;
		
		NormalImage = ItemInfo->InventoryInfo.ItemIconTexture.Get();
		
		ButtonBorder->SetBrushFromTexture(NormalImage);
		ButtonBorder->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));

		SetToolTipText(FText::FromName(Info->ItemName));
		ButtonBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else if(IsValid(ItemInfo) && IsValid(ButtonBorder))
	{
		ButtonBorder->SetBrushFromTexture(UnAssignedImage);
		ButtonBorder->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.0f));
		ItemInfo->ConditionalBeginDestroy();
		ItemInfo = nullptr;
		SetToolTipText(FText());
	}
}
