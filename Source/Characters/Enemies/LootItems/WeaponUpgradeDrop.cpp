// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponUpgradeDrop.h"


AWeaponUpgradeDrop::AWeaponUpgradeDrop(const FObjectInitializer& Intitializer) : Super(Intitializer)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    LootType = LootableItemTypes::WeaponUpgradeDrop;
}