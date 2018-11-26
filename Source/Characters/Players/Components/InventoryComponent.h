// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/DataStructures/ItemObject.h"
#include "InventoryComponent.generated.h"

class UWeaponObject;
class AUseableItemDrop;

USTRUCT()
struct FInventoryStorageData
{
	GENERATED_USTRUCT_BODY()

	FInventoryStorageData();
	FInventoryStorageData(const FInventoryStorageData& NewData);
	FInventoryStorageData& operator=(const FInventoryStorageData& NewData);
	FInventoryStorageData& operator=(const FInventoryStorageData* NewData);

	void CloneThis(const FInventoryStorageData& Data);

	const UItemObject* GetItemObject() const { return InternalItemObject; }

	const uint8 GetStackCount() const { return StackCounter; }

	const uint8 GetButtonIndex() const { return UIButtonIndex; }

	bool Initialize(TSubclassOf<UItemObject> ItemClass);

	void ModifyStackCount(const uint8 value) { StackCounter += value; }

	void SetButtonIndex(const uint8 value) { UIButtonIndex = value; }

protected:
	UPROPERTY()
	UItemObject* InternalItemObject;

	UPROPERTY()
	uint8 StackCounter;

	UPROPERTY()
	uint8 UIButtonIndex;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCRUSADES_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const UItemObject* GetItemByName(const FName& KeyName) const;

	const TArray<FName> GetItemKeyNames() const;

	bool AddLootableItemToInventory(const FName& ItemName, TSubclassOf<UItemObject> LootedItemClass);

	uint8 GetStackCount(const FName& KeyName, ItemTypes Type);

	const UItemObject* UseItem(const FName& KeyName, ItemTypes Type);

	bool UpdateItemButtonIndex(const FName& KeyName, const uint8 index);


protected:
	UPROPERTY()
	TMap<FName, FInventoryStorageData> InventoryItems;

};
