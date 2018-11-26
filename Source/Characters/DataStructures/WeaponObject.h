// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DataStructures/ItemObject.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "StatusEffectData.h"
#include "CombatData.h"
#include "WeaponObject.generated.h"

UENUM(BlueprintType)
enum class WeaponQualityTypes : uint8
{
	WEAPON_TYPE_NONE,
	WEAPON_TYPE_NORMAL,
	WEAPON_TYPE_EPIC,
	WEAPON_TYPE_LEGENDARY
};

USTRUCT(BlueprintType)
struct FWeaponEquipData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FTransform MeshOrientation;

	UPROPERTY(EditAnywhere)
	FTransform CapsuleOrientation;

	UPROPERTY(EditAnywhere)
	FName SocketName;

	UPROPERTY(EditAnywhere)
	float CapsuleHeight;

	UPROPERTY(EditAnywhere)
	float CapsuleRadius;

	UPROPERTY(EditAnywhere)
	FName CollisionProfileName;
};
/**
 * 
 */
UCLASS(Blueprintable)
class DUNGEONCRUSADES_API UWeaponObject : public UItemObject
{
	GENERATED_BODY()
	
	
public:
	UWeaponObject(const FObjectInitializer& Inititializer = FObjectInitializer::Get());

	WeaponQualityTypes GetWeaponQualityType() { return QualityType; }

	void CloneThis(const UWeaponObject* CopyThis);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponEquipData EquipWeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* WeaponHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* WeaponHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Critical Hit Config")
	USoundBase* CriticalHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Critical Hit Config")
	UParticleSystem* CriticalHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMRangeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMRangeMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EquipableTypes EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	DamageTypes DamageType;

protected:
	WeaponQualityTypes QualityType;

	virtual void GenerateDescriptionString() override;
};
