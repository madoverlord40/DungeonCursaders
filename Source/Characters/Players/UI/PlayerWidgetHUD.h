// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthPanelUI.h"
#include "ItemInventoryUI.h"
#include "Characters/DataStructures/CombatData.h"
#include "LevelActors/DungeonTreasureChest.h"
#include "PlayerWidgetHUD.generated.h"

class UInventoryComponent;
class UItemDetailsUI;
class UDoorInteractWindow;
class UPlayerAbilityBar;
class UTreasureLootWindow;
class UCombatLogWidget;
class UPlayerCharacterStatsWidget;
class UCombatComponent;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UPlayerWidgetHUD : public UUserWidget
{
	GENERATED_BODY()


public:
	UPlayerWidgetHUD(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

	void UpdatePlayerHealthPanel(const UCombatData* PlayerData);

	void ShowHideEnemyHealthPanel(const UCombatData* PlayerData, ESlateVisibility Visibility);

	void ShowInventoryPanel(bool show, const UInventoryComponent* InventoryComponent);

	void ShowDetailsBox(const FText& TextToShow, const FText& Title, bool useable);

	void ShowDoorInteractWindow(bool CanOpenDoor, bool CanUnlockDoor, const FText& DetailsText, const FText& TitleText);

	void UnlockDoor();

	void OpenDoor();

	UFUNCTION(BlueprintCallable)
	void SetEnableAbilitySlot(FName SlotName, bool enable);

	UFUNCTION(BlueprintCallable)
	void SetEnableAll(bool enable);

	UPlayerAbilityBar* GetAbilityBar() { return AbilityBar; }

	void UpdateExpBar();

	void NotifyAbilityComplete();

	void ShowHideStatsUI(bool show, const UCombatComponent* BattleController);

	void ShowHideChestLootWindow(const TMap<FName, TSubclassOf<UItemObject>>& LootItems, bool enabled);

	void TakeSelectedLoot(const TArray<FName>& KeyNames);

	bool UnEquipItemFromDragDrop(const FInventoryItemInfo* UnequipInfo);

	bool EquipItemFromDragDrop(const FInventoryItemInfo* UnequipInfo);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthPanelUI* HealthPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthPanelUI* EnemyHealthPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCombatLogWidget* CombatLog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerCharacterStatsWidget* CharacterStatsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTreasureLootWindow* TreasureLootWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemInventoryUI* ItemInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemDetailsUI* DetailsWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDoorInteractWindow* DoorWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerAbilityBar* AbilityBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bIsShowingInventory : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bIsShowingDoorWindow : 1;

};
