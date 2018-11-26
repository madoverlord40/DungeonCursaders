// Fill out your copyright notice in the Description page of Project Settings.

#include "LootSystemComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "../LootItems/UseableItemDrop.h"
#include "../LootItems/ExperienceDropActor.h"
#include "../LootItems/WeaponUpgradeDrop.h"
#include "../LootItems/ModifyStatsDrop.h"

// Sets default values for this component's properties
ULootSystemComponent::ULootSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULootSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULootSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULootSystemComponent::DropItemsForPlayerToLoot()
{
	if (LootableItemPool.Num() > 0)
	{
		const uint16 random = FMath::RandRange(0, MaxLootRandom);

		for (int index = 0; index < LootableItemPool.Num(); index++)
		{
			const FLootableItemData* LootData = &LootableItemPool[index];

			if (LootData->UsesRandom)
			{
				if (random > LootData->RandomMin && random < LootData->RandomMax)
				{
					GetWorld()->SpawnActor<ALootableItemBase>(LootData->ItemToDrop, GetOwner()->GetActorTransform());
				}
			}
			else
			{
				GetWorld()->SpawnActor<ALootableItemBase>(LootData->ItemToDrop, GetOwner()->GetActorTransform());
			}
		}
	}
}

