// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LootSystemComponent.generated.h"


class ALootableItemBase;
class AUseableItemDrop;
class AWeaponUpgradeDrop;
class AModifyStatsDrop;
class AExperienceDropActor;


USTRUCT(BlueprintType)
struct FLootableItemData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ALootableItemBase> ItemToDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RandomMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RandomMax;

	/*Do we just drop, or do we use the random loot? False just drop*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 UsesRandom : 1;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCRUSADES_API ULootSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULootSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DropItemsForPlayerToLoot();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FLootableItemData> LootableItemPool;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxLootRandom;

};
