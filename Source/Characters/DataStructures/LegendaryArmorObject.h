// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DataStructures/EpicArmorObject.h"
#include "StatusEffectData.h"
#include "LegendaryArmorObject.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ULegendaryArmorObject : public UEpicArmorObject
{
	GENERATED_BODY()
	
public:

	ULegendaryArmorObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FExternalStats ExternalAugmentation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<StatusEffectTypes> StatusEffectImmunities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Reflection")
	uint32 bAttackerTakesDamage : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Reflection")
	float ReflectDamagePercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Reflection")
	DamageTypes ReflectDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Reflection")
	ElementalDamageTypes ReflectElementalType;

	void CloneThis(const ULegendaryArmorObject* CopyThis);
};
