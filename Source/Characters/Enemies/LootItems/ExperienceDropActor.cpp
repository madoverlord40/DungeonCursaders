// Fill out your copyright notice in the Description page of Project Settings.

#include "ExperienceDropActor.h"



AExperienceDropActor::AExperienceDropActor(const FObjectInitializer& Intitializer) : Super(Intitializer)
{
    LootType = LootableItemTypes::ExperienceDrop;

}

float AExperienceDropActor::GetExpReward()
{
    float exp = FMath::RandRange(ExpRangeMin, ExpRangeMax);

    return exp;
}
