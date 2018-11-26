// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerWidgetHUD.h"
#include "Characters/Players/Components/InventoryComponent.h"
#include "Characters/DataStructures/ItemObject.h"
#include "ItemDetailsUI.h"
#include "DoorInteractWindow.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "PlayerAbilityBar.h"
#include "TreasureLootWindow.h"
#include "PlayerCharacterStatsWidget.h"
#include "Characters/Players/UI/TreasureLootWindow.h"

UPlayerWidgetHUD::UPlayerWidgetHUD(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerWidgetHUD::UpdatePlayerHealthPanel(const UCombatData* PlayerData)
{
	if (IsValid(PlayerData) && IsValid(HealthPanel)) {
		HealthPanel->SetHealthPercent(PlayerData->GetExternalStats()->HealthPoints / PlayerData->GetExternalStats()->HealthPointsMax);
		HealthPanel->SetManaPercent(PlayerData->GetExternalStats()->ManaPoints / PlayerData->GetExternalStats()->ManaPointsMax);
		HealthPanel->SetRagePercent(PlayerData->GetExternalStats()->FuryPoints / PlayerData->GetExternalStats()->FuryPointsMax);
		HealthPanel->SetStaminaPercent(PlayerData->GetExternalStats()->StaminaPoints / PlayerData->GetExternalStats()->StaminaPointsMax);
		HealthPanel->SetCharacterName(PlayerData->GetCharacterName());
		HealthPanel->SetLevelValue(PlayerData->GetCharacterLevel());
		HealthPanel->SetExpValue(PlayerData->GetCharacterEXP());
	}
}

void UPlayerWidgetHUD::ShowHideEnemyHealthPanel(const UCombatData* PlayerData, ESlateVisibility Visibility)
{
	if (IsValid(EnemyHealthPanel)) {
		if (IsValid(PlayerData))
		{
			EnemyHealthPanel->SetHealthPercent(PlayerData->GetExternalStats()->HealthPoints / PlayerData->GetExternalStats()->HealthPointsMax);
			EnemyHealthPanel->SetManaPercent(PlayerData->GetExternalStats()->ManaPoints / PlayerData->GetExternalStats()->ManaPointsMax);
			EnemyHealthPanel->SetRagePercent(PlayerData->GetExternalStats()->FuryPoints / PlayerData->GetExternalStats()->FuryPointsMax);
			EnemyHealthPanel->SetStaminaPercent(PlayerData->GetExternalStats()->StaminaPoints / PlayerData->GetExternalStats()->StaminaPointsMax);
			EnemyHealthPanel->SetCharacterName(PlayerData->GetCharacterName());
			EnemyHealthPanel->SetLevelValue(PlayerData->GetCharacterLevel());
			EnemyHealthPanel->SetExpValue(PlayerData->GetCharacterEXP());
		}

		EnemyHealthPanel->SetVisibility(Visibility);
	}
}

void UPlayerWidgetHUD::ShowInventoryPanel(bool show, const UInventoryComponent* InventoryComponent)
{
	if (IsValid(ItemInventory))
	{
		if (show && IsValid(InventoryComponent) && show)
		{
			const TArray<FName>& ItemKeys = InventoryComponent->GetItemKeyNames();
			TArray<FInventoryItemInfo> Items;

			for (int index = 0; index < ItemKeys.Num(); index++)
			{
				const UItemObject* ItemObject = InventoryComponent->GetItemByName(ItemKeys[index]);

				if (IsValid(ItemObject) && ItemObject->IsValidLowLevel())
				{
					FInventoryItemInfo info;

					info.ItemName = ItemObject->ItemName;
					info.InventoryKeyName = ItemKeys[index];
					info.ItemDescription = *ItemObject->GetDescriptionString();
					info.bIsUseable = (ItemObject->GetItemType() == ItemTypes::ITEM_CONSUMABLE);
					info.ItemIconTexture = ItemObject->InventoryIconTexture;
					info.InventoryKeyName = ItemKeys[index];
					info.ItemClass = ItemObject->GetClass();
					info.StackCount = 1;
					info.Type = ItemObject->GetItemType();
					Items.Add(info);
				}
			}
			ItemInventory->SetParentUI(this);
			ItemInventory->DisplayInventoryItems(Items);
			bIsShowingInventory = true;
			ItemInventory->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			bIsShowingInventory = false;
			ItemInventory->SetVisibility(ESlateVisibility::Hidden);
		}
		
	}
}

void UPlayerWidgetHUD::ShowDetailsBox(const FText& TextToShow, const FText& Title, bool useable)
{
	if (IsValid(DetailsWindow))
	{
		DetailsWindow->InventoryWindow = ItemInventory;
		DetailsWindow->SetDetailsText(TextToShow, Title, useable);
		DetailsWindow->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerWidgetHUD::ShowDoorInteractWindow(bool CanOpenDoor, bool CanUnlockDoor, const FText& DetailsText, const FText& TitleText)
{
	if (IsValid(DoorWindow))
	{
		if (bIsShowingDoorWindow)
		{
			DoorWindow->ParentUI = this;
			
			DoorWindow->SetVisibility(ESlateVisibility::Hidden);
			bIsShowingDoorWindow = false;

			TWeakObjectPtr<APlayerControllerBase> PlayerController = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController());
			if (PlayerController.IsValid())
			{
				PlayerController->CancelDoorInteractUI();
			}
		}
		else
		{
			DoorWindow->SetWindowTexts(TitleText, DetailsText);
			DoorWindow->SetButtonStatuses(CanUnlockDoor, CanOpenDoor);
			DoorWindow->ParentUI = this;
			DoorWindow->SetVisibility(ESlateVisibility::Visible);
			bIsShowingDoorWindow = true;
		}
	}
}

void UPlayerWidgetHUD::UnlockDoor()
{
	TWeakObjectPtr<APlayerController> controller = GetWorld()->GetFirstPlayerController();

	if (controller.IsValid())
	{
		TWeakObjectPtr<APlayerControllerBase> PlayerController = Cast<APlayerControllerBase>(controller.Get());
		if (PlayerController.IsValid())
		{
			PlayerController->UnlockSelectedDoor();
			DoorWindow->SetButtonStatuses(false, true);
		}
	}
}

void UPlayerWidgetHUD::OpenDoor()
{
	TWeakObjectPtr<APlayerController> controller = GetWorld()->GetFirstPlayerController();

	if (controller.IsValid())
	{
		TWeakObjectPtr<APlayerControllerBase> PlayerController = Cast<APlayerControllerBase>(controller.Get());
		if (PlayerController.IsValid())
		{
			PlayerController->OpenSelectedDoor();
			//toggle it back off to hidden
			ShowDoorInteractWindow(false, false, FText(), FText());

		}
	}
}

void UPlayerWidgetHUD::SetEnableAbilitySlot(FName SlotName, bool enable)
{
	if (IsValid(AbilityBar))
	{
		AbilityBar->SetEnableAbilitySlot(SlotName, enable);
	}
}

void UPlayerWidgetHUD::SetEnableAll(bool enable)
{
	if (IsValid(AbilityBar))
	{
		AbilityBar->SetEnableAllSlots(enable);
	}
}

void UPlayerWidgetHUD::UpdateExpBar()
{
	if (IsValid(AbilityBar))
	{
		TWeakObjectPtr<APlayerController> controller = GetWorld()->GetFirstPlayerController();

		if (controller.IsValid())
		{
			TWeakObjectPtr<APlayerControllerBase> PlayerController = Cast<APlayerControllerBase>(controller.Get());
			if (PlayerController.IsValid())
			{
				const float percent = PlayerController->GetCharacterCombatData()->GetExpPercent();
				AbilityBar->UpdateExpBar(percent);
			}
		}
	}
}

void UPlayerWidgetHUD::NotifyAbilityComplete()
{
	if (IsValid(AbilityBar))
	{
		AbilityBar->NotifyAbilityComplete();
	}
}

void UPlayerWidgetHUD::ShowHideStatsUI(bool show, const UCombatComponent* BattleController)
{
	if (IsValid(CharacterStatsWidget) && show)
	{
		CharacterStatsWidget->SetPropertyValues(BattleController->GetCharacterCombatData());
		CharacterStatsWidget->SetRightWeaponSlot(BattleController->GetRightEquippedWeapon());
		CharacterStatsWidget->SetLeftWeaponSlot(BattleController->GetLeftEquippedWeapon());
		const UEquipmentComponent* EquipmentComp = BattleController->GetEquipmentComponent();

		if (IsValid(EquipmentComp))
		{
			const TMap<ArmorSlotTypes, UEquippedArmorObject*>& EquippedArmors = EquipmentComp->GetEquippedArmors();

			for (const TPair<ArmorSlotTypes, UEquippedArmorObject*>& Pairs : EquippedArmors)
			{
				if (IsValid(Pairs.Value))
				{
					CharacterStatsWidget->SetArmorSlot(Pairs.Key, Pairs.Value);
				}
			}

		}
		
	}

	ESlateVisibility Visible = show ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	CharacterStatsWidget->SetVisibility(Visible);
}

void UPlayerWidgetHUD::ShowHideChestLootWindow(const TMap<FName, TSubclassOf<UItemObject>>& LootItems, bool enabled)
{
	if (IsValid(TreasureLootWindow) && enabled)
	{
		for (const TPair<FName, TSubclassOf<UItemObject>>& Pairs : LootItems)
		{
			const TSubclassOf<UItemObject> ItemClass = Pairs.Value;

			if (IsValid(ItemClass))
			{
				const UItemObject* ItemCDO = Cast<UItemObject>(ItemClass->GetDefaultObject());

				if (IsValid(ItemCDO))
				{
					TreasureLootWindow->AddLootItemToWindow(Pairs.Key, FText::FromName(ItemCDO->ItemName), ItemCDO->InventoryIconTexture, ItemCDO->GetDescriptionString());
				}
			}
		}
		
	}

	if (enabled)
	{
		TreasureLootWindow->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TreasureLootWindow->SetVisibility(ESlateVisibility::Hidden);
		TWeakObjectPtr<APlayerController> controller = GetWorld()->GetFirstPlayerController();

		if (controller.IsValid())
		{
			TWeakObjectPtr<APlayerControllerBase> PlayerController = Cast<APlayerControllerBase>(controller.Get());
			if (PlayerController.IsValid())
			{
				PlayerController->ExitInteratingMode();
			}
		}
	}
}

void UPlayerWidgetHUD::TakeSelectedLoot(const TArray<FName>& KeyNames)
{
	TWeakObjectPtr<APlayerController> controller = GetWorld()->GetFirstPlayerController();

	if (controller.IsValid())
	{
		TWeakObjectPtr<APlayerControllerBase> PlayerController = Cast<APlayerControllerBase>(controller.Get());
		if (PlayerController.IsValid())
		{
			PlayerController->TakeSelectedLoot(KeyNames);
		}

	}
}

bool UPlayerWidgetHUD::UnEquipItemFromDragDrop(const FInventoryItemInfo* UnequipInfo)
{
	TWeakObjectPtr<APlayerController> controller = GetWorld()->GetFirstPlayerController();

	if (controller.IsValid())
	{
		TWeakObjectPtr<APlayerControllerBase> PlayerController = Cast<APlayerControllerBase>(controller.Get());
		if (PlayerController.IsValid())
		{
			return PlayerController->UnEquipItem(UnequipInfo);
		}

	}
	return false;
}

bool UPlayerWidgetHUD::EquipItemFromDragDrop(const FInventoryItemInfo* UnequipInfo)
{
	TWeakObjectPtr<APlayerController> controller = GetWorld()->GetFirstPlayerController();

	if (controller.IsValid())
	{
		TWeakObjectPtr<APlayerControllerBase> PlayerController = Cast<APlayerControllerBase>(controller.Get());
		if (PlayerController.IsValid())
		{
			return PlayerController->EquipItem(UnequipInfo);
		}

	}

	return false;
}

void UPlayerWidgetHUD::NativeConstruct()
{
	Super::NativeConstruct();

}

void UPlayerWidgetHUD::NativeDestruct()
{
	Super::NativeDestruct();

}

void UPlayerWidgetHUD::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}
