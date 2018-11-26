// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "../../Enemies/LootItems/UseableItemDrop.h"
#include "Characters/DataStructures/DungeonDoorKey.h"
#include "Characters/DataStructures/ConsumableItemObject.h"
#include "Characters/DataStructures/ArmorObject.h"
#include "Characters/DataStructures/WeaponObject.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

const UItemObject* UInventoryComponent::GetItemByName(const FName & KeyName) const
{
	const FInventoryStorageData* FoundItem = InventoryItems.Find(KeyName);

	if (FoundItem)
	{
		const UItemObject* ItemObject = FoundItem->GetItemObject();
				
		return ItemObject;		
	}

	return nullptr;
}


const TArray<FName> UInventoryComponent::GetItemKeyNames() const
{
	TArray<FName> OutNames;
	InventoryItems.GenerateKeyArray(OutNames);
	return OutNames;
}

bool UInventoryComponent::AddLootableItemToInventory(const FName & ItemName, const TSubclassOf<UItemObject> LootedItemClass)
{
	bool success = false;

	if (IsValid(LootedItemClass) && ItemName.Compare(NAME_None) != 0)
	{
		const UItemObject* ItemObject = LootedItemClass->GetDefaultObject<UItemObject>();
		const ItemTypes Type = ItemObject->GetItemType();

		FInventoryStorageData NewInventoryItem;

		FInventoryStorageData* FoundItem = InventoryItems.Find(ItemName);

		if (FoundItem == nullptr)
		{
			if (NewInventoryItem.Initialize(LootedItemClass))
			{
				InventoryItems.Add(ItemName, NewInventoryItem);
				success = true;
			}

		}
		else
		{
			FoundItem->ModifyStackCount(1);

			success = true;

		}

	}
	return success;
}

uint8 UInventoryComponent::GetStackCount(const FName& KeyName, ItemTypes Type)
{
	FInventoryStorageData* FoundItem = InventoryItems.Find(KeyName);

	if (FoundItem == nullptr)
	{
		const UItemObject* ItemObject = FoundItem->GetItemObject();

		if (IsValid(ItemObject))
		{
			if (Type == ItemObject->GetItemType())
			{
				return FoundItem->GetStackCount();
			}
		}
	}

	return 0;
}

const UItemObject* UInventoryComponent::UseItem(const FName& KeyName, ItemTypes Type)
{
	FInventoryStorageData* FoundInventoryItem = InventoryItems.Find(KeyName);

	if (FoundInventoryItem)
	{
		const UItemObject* UseableItem = FoundInventoryItem->GetItemObject();

		if (IsValid(UseableItem))
		{
			const ItemTypes UsableItemType = UseableItem->GetItemType();

			if (Type == UsableItemType)
			{
				FoundInventoryItem->ModifyStackCount(-1);
			}
		}

		return UseableItem;
	}

	return nullptr;
}

bool UInventoryComponent::UpdateItemButtonIndex(const FName& KeyName, const uint8 index)
{
	FInventoryStorageData* FoundInventoryItem = InventoryItems.Find(KeyName);

	if (FoundInventoryItem)
	{
		FoundInventoryItem->SetButtonIndex(index);
		return true;
	}

	return false;
}

FInventoryStorageData::FInventoryStorageData()
{
	InternalItemObject = nullptr;
	StackCounter = 0;
	UIButtonIndex = 0;
}

FInventoryStorageData::FInventoryStorageData(const FInventoryStorageData & NewData)
{
	InternalItemObject = nullptr;
	StackCounter = 0;
	UIButtonIndex = 0;

	CloneThis(NewData);
	
}

FInventoryStorageData & FInventoryStorageData::operator=(const FInventoryStorageData & NewData)
{
	CloneThis(NewData);

	return *this;
}

FInventoryStorageData & FInventoryStorageData::operator=(const FInventoryStorageData * NewData)
{
	CloneThis(*NewData);
	return *this;
}

void FInventoryStorageData::CloneThis(const FInventoryStorageData & Data)
{
	if (IsValid(Data.GetItemObject()))
	{
		Initialize(Data.GetItemObject()->GetClass());

		StackCounter = Data.GetStackCount();
		UIButtonIndex = Data.GetButtonIndex();
	}
}

bool FInventoryStorageData::Initialize(TSubclassOf<UItemObject> ItemClass)
{
	if (IsValid(ItemClass))
	{
		if (IsValid(InternalItemObject))
		{
			InternalItemObject->ConditionalBeginDestroy();
			InternalItemObject = nullptr;
		}

		InternalItemObject = NewObject<UItemObject>(GetTransientPackage(), ItemClass);

		if (IsValid(InternalItemObject))
		{
			return true;
		}
	}
	
	return false;
}
