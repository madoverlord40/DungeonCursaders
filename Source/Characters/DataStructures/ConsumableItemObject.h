// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DataStructures/ItemObject.h"
#include "ConsumableItemObject.generated.h"


class USoundCue;
class UParticleSystem;

UENUM(BlueprintType)
enum class ConsumableItemTypes : uint8
{
	ComsumableType_Healing,
	ConsumableType_Curing,
	ConsumableType_Boost_Stats,
	ConsumableType_Boost_Resistances
};
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UConsumableItemObject : public UItemObject
{
	GENERATED_BODY()
	
public:

	UConsumableItemObject();

	float GetBaseValue() const { return BaseValue; }
	USoundCue* GetItemSound() const { return UseItemSound; }
	UParticleSystem* GetItemParticle() const { return UseItemParticle; }
	ConsumableItemTypes GetConsumableType() const { return ConsumeType; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ConsumableItemTypes ConsumeType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundCue* UseItemSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* UseItemParticle;

	virtual void GenerateDescriptionString() override;

};
