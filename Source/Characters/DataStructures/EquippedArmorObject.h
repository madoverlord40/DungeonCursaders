// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LegendaryArmorObject.h"
#include "EquippedArmorObject.generated.h"


USTRUCT()
struct FDamageReflectData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	uint32 bAttackerTakesDamage : 1;

	UPROPERTY()
	float ReflectDamagePercent;

	UPROPERTY()
	DamageTypes ReflectDamageType;

	UPROPERTY()
	ElementalDamageTypes ReflectElementalType;
};
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UEquippedArmorObject : public UObject
{
	GENERATED_BODY()
	
public:
	UEquippedArmorObject();
	

	const UArmorObject* const& GetArmorObject() const { return MainArmorObject; }

	virtual void BeginDestroy() override;

	void CreateThis(const FName& Name, const TSubclassOf<UArmorObject>& ArmorClass);

	void EquipThis(UStaticMeshComponent* ArmorMeshComp, const FName& SocketName, USkeletalMeshComponent* AttachTo);
	void EquipThis(USkeletalMeshComponent*& MasterPoseComp, USkeletalMeshComponent*& SkinnedSlotMesh);
	void EquipThis(USkeletalMeshComponent*& MasterPoseComp);

	void ConfigureThis(UCombatData*& CombatStats);

	void UnEquipThis(UCombatData*& CombatStats);

	ArmorQualityTypes GetArmorQualityType() const;

	const FString& GetDescriptionString() const { return Description; }
		
	const TArray<FElementalResistData>* GetElementalResistances() const;
	
	const TArray<FDamageResistData>* GetDamageResistances() const;
	
	float GetDefensePower() const;
	
	float GetMagicDefense() const;
	
	const USkeletalMesh* GetSkinnedMesh() const;
	
	ArmorSlotTypes GetArmorSlot() const;

	const FDamageReflectData* GetDamageReflection() { return &DamageReflection; }

	const TArray<StatusEffectTypes>* GetStatusImmunities() const;

	void SetEnableHighlightMesh(bool enable);


private:
	void Initialize(const UEquippedArmorObject& NewWeapon);

protected:
	UPROPERTY()
	UArmorObject* MainArmorObject;

	UPROPERTY()
	UEpicArmorObject* EpicArmor;

	UPROPERTY()
	ULegendaryArmorObject* LegendaryArmor;

	UPROPERTY()
	FString Description;

	UPROPERTY()
	uint32 bHasBeenConfigured : 1;

	UPROPERTY()
	USkeletalMeshComponent* MasterPoseComponent;

	UPROPERTY()
	USkeletalMeshComponent* SkinnedMeshComponent;

	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY()
	FDamageReflectData DamageReflection;

	UPROPERTY()
	uint32 bWasMeshCreatedLocal : 1;
};
