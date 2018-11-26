// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/LootItems/LootableItemBase.h"
#include "ExperienceDropActor.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AExperienceDropActor : public ALootableItemBase
{
	GENERATED_BODY()
	
public:
    AExperienceDropActor(const FObjectInitializer& Intitializer = FObjectInitializer::Get());

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ExpRangeMin;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ExpRangeMax;

    float GetExpReward();
};
