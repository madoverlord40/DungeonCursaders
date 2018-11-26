// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DataStructures/EpicWeaponObject.h"
#include "LegendaryWeaponObject.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ULegendaryWeaponObject : public UEpicWeaponObject
{
	GENERATED_BODY()


public:

	ULegendaryWeaponObject(const FObjectInitializer& Inititializer = FObjectInitializer::Get());

	void CloneThis(const ULegendaryWeaponObject* CopyThis);

	/* Get a random status effect using the weights provided as chances to get one, possibly none*/
	const TPair<FName, TSubclassOf<UStatusEffectData>>* GetRandomStatusEffect();
	
	const TMap<FName, TSubclassOf<UStatusEffectData>>& GetOptionStatusEffects() const { return OptionalEffectsToApply; }

	const TMap<FName, uint8>& GetStatusEffectWeights() const { return RandomWeights; }

	/* these are to be treated like percentages to be added, not full increase/decrease*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FExternalStats ExternalStatAugmentation;

protected:
	/*Can be empty, optional status effects to apply, are random and weighted*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Effect Config")
	TMap<FName, TSubclassOf<UStatusEffectData>> OptionalEffectsToApply;

	/* This map must match the optional effects so we know what the random chance is to apply them*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Effect Config")
	TMap<FName, uint8> RandomWeights;

	/* Creates a string that can be used in a UI outlining what stats are on this weapon*/
	virtual void GenerateDescriptionString() override;

private:
	void Initialize(const ULegendaryWeaponObject& NewWeapon);
	
};
