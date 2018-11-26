// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatData.h"
#include "PlayerTraitObject.generated.h"


class UTexture2D;

UENUM(BlueprintType)
enum class EPlayerTraitTypes : uint8
{
	PlayerTrait_NONE,
	PlayerTrait_Damage,
	PlayerTrait_Defense,
	PlayerTrait_InternalStat,
	PlayerTrait_ExternalStat,
	PlayerTrait_Resistence,
	PlayerTrait_Recovery,
	PlayerTrait_Stealth
};
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UPlayerTraitObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPlayerTraitObject();

	//UPlayerTraitObject(const UPlayerTraitObject& NewTrait);

	UPlayerTraitObject& operator=(const UPlayerTraitObject* NewTrait);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TraitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TraitDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* TraitIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerTraitTypes TraitType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FExternalStats Externals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInternalStats Internals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FElementalResistData ElementResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageResistData DamageResistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Modifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 UpgradePoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 UpgradePointsMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UpgradeModifierPerPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bHasTraitRequirement : 1;

	/*what trait name should already have at least 1 point in before we can upgrade this*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TraitRequirementKey;

	/*what is the point requirement of the required trait needed to upgrade this*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 TraitPointRequirement;

	void UpgradeTrait(const uint8 Points);

private:
	void CopyThis(const UPlayerTraitObject* CopyTrait);
	void UpgradeInternals(const uint8 points);
	void UpgradeExternals(const uint8 points);


};
