// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/Engine/SkeletalMesh.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "ItemObject.generated.h"

UENUM(Blueprintable)
enum class ItemTypes : uint8 {

	ITEM_NONE,
	ITEM_DOOR_KEY,
	ITEM_EQUIPABLE_WEAPON,
	ITEM_EQUIPABLE_ARMOR,
	ITEM_CONSUMABLE,
	ITEM_CURRENCY,
	ITEM_TOOL
};

UENUM(Blueprintable)
enum class EquipableTypes : uint8 {

	EQUIPABLE_NONE,
	EQUIPABLE_ONE_HAND_WEAPON,
	EQUIPABLE_DUAL_WEAPON,
	EQUIPABLE_TWO_HAND_WEAPON,
	EQUIPABLE_SHIELD
};

UENUM(Blueprintable)
enum class ArmorSlotTypes : uint8 {

	ARMOR_SLOT_NONE,
	ARMOR_SLOT_HEAD,
	ARMOR_SLOT_CHEST,
	ARMOR_SLOT_LEGS,
	ARMOR_SLOT_FEET,
	ARMOR_SLOT_NECKLACE,
	ARMOR_SLOT_BELT,
	ARMOR_SLOT_RING_ONE,
	ARMOR_SLOT_RING_TWO
};
/**
 * 
 */
UCLASS(Blueprintable)
class DUNGEONCRUSADES_API UItemObject : public UObject
{
	GENERATED_BODY()
	
	
public:
	UItemObject(const FObjectInitializer& Inititializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMeshBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIconTexture;

	const ItemTypes GetItemType() const { return Type; }

	const FString& GetDescriptionString() const { return DescriptionString; }

	void CloneThis(const UItemObject* CopyThis);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ItemTypes Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString DescriptionString;

	virtual void GenerateDescriptionString();
};
