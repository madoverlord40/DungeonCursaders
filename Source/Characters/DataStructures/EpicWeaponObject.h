// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DataStructures/WeaponObject.h"
#include "EpicWeaponObject.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UEpicWeaponObject : public UWeaponObject
{
	GENERATED_BODY()
	
	
public:
	UEpicWeaponObject(const FObjectInitializer& Inititializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Critical Hit Config")
	uint32 bUsesCriticalHitModifier : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Critical Hit Config")
	float CriticalHitModifier;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Effects")
	TSubclassOf<UStatusEffectData> StatusEffectToApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Effects")
	UParticleSystem* WeaponStatusEffectParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StatusParticleSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Effects")
	FVector RelativeSocketOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Effects")
	FRotator RelativeSocketRotation;

	/*only apply the status effect if critical, is paired with bMustDeliverBaseDamage
	Meaning they both play a role in deciding if the effect lands*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Effects")
	uint32 bMustBeCriticalToApplyStatus : 1;

	/*only apply the status effect if we deliver at least base damage, meaning non 0*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Effects")
	uint32 bMustDeliverBaseDamageMin : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInternalStats InternalStatAugmentation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ElementalDamageTypes ElementDamageType;


	/* Creates a string that can be used in a UI outlining what stats are on this weapon*/
	virtual void GenerateDescriptionString() override;

	void CloneThis(const UEpicWeaponObject* CopyThis);

private:
	void Initialize(const UEpicWeaponObject& NewWeapon);
};
