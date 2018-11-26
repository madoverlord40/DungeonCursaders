// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DataStructures/ItemObject.h"
#include "DungeonDoorKey.generated.h"

UENUM(BlueprintType)
enum class DoorKeyTypes : uint8
{
	BRONZE_KEY,
	BRASS_KEY,
	IRON_KEY,
	SILVER_KEY,
	GOLD_KEY,
	SKELETON_KEY,
	NONE_KEY
};

class ADungeonDoorActor;
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UDungeonDoorKey : public UItemObject
{
	GENERATED_BODY()
	
public:
	UDungeonDoorKey(const FObjectInitializer& Inititializer = FObjectInitializer::Get());

	DoorKeyTypes GetKeyType() const { return KeyType; }
	bool GetIsUseable();

	bool UseKey();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	DoorKeyTypes KeyType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 NumberOfUses = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bIsKeyBroken : 1;

	virtual void GenerateDescriptionString() override;

};
