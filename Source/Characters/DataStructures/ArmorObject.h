// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemObject.h"
#include "UObject/NoExportTypes.h"
#include "CombatData.h"
#include "ArmorObject.generated.h"


UENUM(BlueprintType)
enum class ArmorQualityTypes : uint8
{
	ARMOR_QUALITY_NORMAL,
	ARMOR_QUALITY_EPIC,
	ARMOR_QUALITY_LEGENDARY
};

class USkeletalMesh;
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UArmorObject : public UItemObject
{
	GENERATED_BODY()

public:
	UArmorObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FElementalResistData ElementalResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageResistData DamageResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefensePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicDefense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkinedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ArmorSlotTypes ArmorSlot;

	void CloneThis(const UArmorObject* CopyThis);

	const ArmorQualityTypes GetQualityType() const { return QualityType; }
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ArmorQualityTypes QualityType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EquipableTypes EquipType;
};
