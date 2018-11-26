// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/LootItems/LootableItemBase.h"
#include "Characters/DataStructures/WeaponObject.h"
#include "WeaponUpgradeDrop.generated.h"


/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AWeaponUpgradeDrop : public ALootableItemBase
{
	GENERATED_BODY()
	
public:
    AWeaponUpgradeDrop(const FObjectInitializer& Intitializer = FObjectInitializer::Get());
	
	const UClass* GetItemClass() const { return WeaponObjectToDropClass; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UWeaponObject> WeaponObjectToDropClass;
};
