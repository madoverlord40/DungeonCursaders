// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/LootItems/LootableItemBase.h"
#include "Characters/DataStructures/ItemObject.h"
#include "UseableItemDrop.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AUseableItemDrop : public ALootableItemBase
{
	GENERATED_BODY()
	
public:
	AUseableItemDrop(const FObjectInitializer& Initializer = FObjectInitializer::Get());

	const TSubclassOf<UItemObject>& GetItemClass() const { return ItemObjectToDropClass; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UItemObject> ItemObjectToDropClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform RelativeOffset;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	
	
};
