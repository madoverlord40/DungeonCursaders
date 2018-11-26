// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LegendaryWeaponObject.h"
#include "EquippedWeaponObject.generated.h"


USTRUCT(Blueprintable)
struct FCombatDamageData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float InHitAcc;

	UPROPERTY()
	float InPower;

	UPROPERTY(EditAnywhere)
	float InDamage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UStatusEffectData> StatusEffectClass;

	UPROPERTY(EditAnywhere)
	DamageTypes AttackDamageType;

	UPROPERTY(EditAnywhere)
	ElementalDamageTypes AttackElement;

	UPROPERTY(EditAnywhere)
	FName StatusEffectName;

	UPROPERTY()
	uint32 bEffectOnCritical : 1;

	UPROPERTY()
	uint32 bMustDamageForEffect : 1;

	UPROPERTY(EditAnywhere)
	uint32 bUseCriticalMod : 1;

	UPROPERTY()
	uint32 bHitIsCritical : 1;

	UPROPERTY(EditAnywhere)
	float CriticalModifier;

	UPROPERTY()
	uint32 bUsingComboAttack : 1;

};

class UParticleSystemComponent;
class ABattleCharacterBase;
class UCapsuleComponent;
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UEquippedWeaponObject : public UObject
{
	GENERATED_BODY()
	
	
public:
	UEquippedWeaponObject();
	
	UEquippedWeaponObject& operator=(const UEquippedWeaponObject* NewWeapon);

	TWeakObjectPtr<UWeaponObject> GetWeaponObject() const { return MainWeaponObject; }

	virtual void BeginDestroy() override;

	void CreateThis(ABattleCharacterBase* BattleCharacter, const FName& Name, TSubclassOf<UWeaponObject> WeaponClass);

	void EquipThis(UMeshComponent* WeaponMeshComp, UMeshComponent* AttachTo = nullptr);
	
	void EquipThis(USkeletalMeshComponent* AttachTo, bool CreateWeaponMesh = true);

	void ConfigureThis(UCombatData*& CombatStats, const bool IsLeftHand);

	void UnEquipThis(UCombatData*& CombatStats);

	WeaponQualityTypes GetWeaponQualityType();

	const FString& GetDescriptionString() const { return Description; }
	
	void GetWeaponHitSound(USoundBase*& WeaponSound);

	void GetWeaponHitFX(UParticleSystem*& WeaponHitEffect);

	void GetCriticalHitSound(USoundBase*& CriticalHitSound);

	void GetCriticalHitFX(UParticleSystem*& CriticalHitEffect);

	float ComputeWeaponDamage(float modifiers, bool IsCriticalHit);
	
	EquipableTypes GetEquipType();

	DamageTypes GetDamageType();

	const TSubclassOf<UStatusEffectData> GetStatusEffectClass();

	void ConfigureDamageData(FCombatDamageData& DamageData);

	UFUNCTION()
	void OnBeginWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void SetCollisionEnabled(ECollisionEnabled::Type Enabled);

	void SetEnableHighlight(bool enable);

private:
	void Initialize(const UEquippedWeaponObject& NewWeapon);

protected:

	UPROPERTY()
	ABattleCharacterBase* OwningBattleCharacter;

	UPROPERTY()
	UWeaponObject* MainWeaponObject;

	UPROPERTY()
	UEpicWeaponObject* EpicWeapon;

	UPROPERTY()
	ULegendaryWeaponObject* LegendaryWeapon;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* OwningMeshComp;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* WeaponCollisionComp;

	UPROPERTY()
	UParticleSystemComponent* WeaponParticle;

	UPROPERTY()
	FString Description;

	uint32 bIsLeftHandWeapon : 1;
	uint32 bHasBeenConfigured : 1;
	
};
