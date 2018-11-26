// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Characters/DataStructures/ItemObject.h"
#include "ItemInventoryUI.generated.h"

class UScrollBox;
class UButton;
class UTextBlock;
class UBorder;
class UPlayerWidgetHUD;
class UUsableItemButton;

USTRUCT()
struct FInventoryItemInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FName ItemName;

	UPROPERTY()
	FName InventoryKeyName;

	UPROPERTY()
	FString ItemDescription;

	UPROPERTY()
	uint8 StackCount;

	UPROPERTY()
	uint32 bIsUseable : 1;

	UPROPERTY()
	TWeakObjectPtr<UTexture2D> ItemIconTexture;

	UPROPERTY()
	ItemTypes Type;

	UPROPERTY()
	ArmorSlotTypes ArmorSlotType;

	UPROPERTY()
	TSubclassOf<UItemObject> ItemClass;

	UPROPERTY()
	uint8 ButtonIndex;

	UPROPERTY()
	uint32 bIsLeftHandWeapon : 1;
};

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UItemInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UItemInventoryUI(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

	void DisplayInventoryItems(TArray<FInventoryItemInfo>& ItemsToShow);

	void OnButtonClicked(uint8 ItemIndex);

	UPlayerWidgetHUD* GetParentUI() const { return ParentWindow; }

	void SetParentUI(UPlayerWidgetHUD* Parent) { ParentWindow = &(*Parent); }

	void RemoveItemFromUI(const uint8 index);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UUsableItemButton*> InventoryButtons;

	TArray<FInventoryItemInfo> InventoryItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerWidgetHUD* ParentWindow;
};
