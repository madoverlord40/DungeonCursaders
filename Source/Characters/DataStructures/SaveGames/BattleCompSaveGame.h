// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Characters/DataStructures/LegendaryWeaponObject.h"
#include "BattleCompSaveGame.generated.h"


/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UBattleCompSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TSubclassOf<UCombatData> CombatDataClass;
	
	UPROPERTY()
	TSubclassOf<UWeaponObject> WeaponClassLeft;

	UPROPERTY()
	TSubclassOf<UWeaponObject> WeaponClassRight;

	UPROPERTY()
	FName CharacterName;

	UPROPERTY()
	uint8 CurrentLevel;

	

};
