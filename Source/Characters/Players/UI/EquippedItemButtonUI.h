// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/DataStructures/LegendaryWeaponObject.h"
#include "Characters/DataStructures/LegendaryArmorObject.h"
#include "Characters/DataStructures/InventoryItemObject.h"
#include "EquippedItemButtonUI.generated.h"

class UBorder;
class UPlayerWidgetHUD;
class UPlayerCharacterStatsWidget;
class UUsableItemButton;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UEquippedItemButtonUI : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UEquippedItemButtonUI(const FObjectInitializer& Initializer = FObjectInitializer::Get());

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void SetButtonInfo(FInventoryItemInfo* Info, UUserWidget* Parent, uint32 CanDelete, bool IsWeapon = false);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBorder* BorderButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerCharacterStatsWidget* ParentWidgetUI;

	UPROPERTY()
	UPlayerWidgetHUD* PlayerUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDragDropOperation> DragOpClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUsableItemButton> UsableItemClass;

	UPROPERTY()
	UInventoryItemObject* ItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ArmorSlotTypes ArmorSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ItemTypes ItemType;

	uint32 bIsWeapon : 1;
};
