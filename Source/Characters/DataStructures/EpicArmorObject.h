// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ArmorObject.h"
#include "EpicArmorObject.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UEpicArmorObject : public UArmorObject
{
	GENERATED_BODY()
	
public:
	UEpicArmorObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FElementalResistData> ElementalResistances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDamageResistData> DamageResistances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInternalStats InternalAugmentation;

	void CloneThis(const UEpicArmorObject* CopyThis);

};
