// Fill out your copyright notice in the Description page of Project Settings.

#include "LegendaryWeaponObject.h"
#include "Runtime/Core/Public/HAL/UnrealMemory.h"

ULegendaryWeaponObject::ULegendaryWeaponObject(const FObjectInitializer& Inititializer) : Super(Inititializer)
{
	Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;
	QualityType = WeaponQualityTypes::WEAPON_TYPE_LEGENDARY;
	GenerateDescriptionString();
}

ULegendaryWeaponObject::ULegendaryWeaponObject(const ULegendaryWeaponObject& NewWeapon)
{
	Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;
	QualityType = WeaponQualityTypes::WEAPON_TYPE_LEGENDARY;

	Initialize(NewWeapon);
}

void ULegendaryWeaponObject::CloneThis(const ULegendaryWeaponObject* CopyThis)
{
	Initialize(*CopyThis);
}

ULegendaryWeaponObject& ULegendaryWeaponObject::operator=(const ULegendaryWeaponObject& NewWeapon)
{
	Initialize(NewWeapon);

	return *this;
}

const TPair<FName, TSubclassOf<UStatusEffectData>>* ULegendaryWeaponObject::GetRandomStatusEffect()
{
	TPair<FName, TSubclassOf<UStatusEffectData>>* ReturnPair = nullptr;

	if (OptionalEffectsToApply.Num() > 0 && RandomWeights.Num() == OptionalEffectsToApply.Num())
	{
		uint32 RandTotals = 0;

		for (const TPair<FName, uint8>& Pair : RandomWeights)
		{
			RandTotals += Pair.Value;
		}

		int32 random = FMath::RandRange(0, RandTotals);
		for (const TPair<FName, uint8>& Pair : RandomWeights)
		{
			if (random > Pair.Value)
			{
				random -= Pair.Value;
			}
			else
			{
				ReturnPair->Value = OptionalEffectsToApply.FindChecked(Pair.Key);
				ReturnPair->Key = Pair.Key;

				break;
			}
		}

	}
	return ReturnPair;
}

void ULegendaryWeaponObject::GenerateDescriptionString()
{

}

void ULegendaryWeaponObject::Initialize(const ULegendaryWeaponObject& NewWeapon)
{
	bUsesCriticalHitModifier = NewWeapon.bUsesCriticalHitModifier;

	CriticalHitModifier = NewWeapon.CriticalHitModifier;

	StatusEffectToApply;

	WeaponStatusEffectParticle = NewWeapon.WeaponStatusEffectParticle;

	StatusParticleSocketName = NewWeapon.StatusParticleSocketName;

	RelativeSocketOffset = NewWeapon.RelativeSocketOffset;

	RelativeSocketRotation = NewWeapon.RelativeSocketRotation;

	bMustBeCriticalToApplyStatus = NewWeapon.bMustBeCriticalToApplyStatus;

	bMustDeliverBaseDamageMin = NewWeapon.bMustDeliverBaseDamageMin;

	FMemory::Memcpy(InternalStatAugmentation, NewWeapon.InternalStatAugmentation);

	ElementDamageType = NewWeapon.ElementDamageType;

	FMemory::Memcpy(ExternalStatAugmentation, NewWeapon.ExternalStatAugmentation);

	const TMap<FName, TSubclassOf<UStatusEffectData>>& StatusEffects = NewWeapon.GetOptionStatusEffects();
	const TMap<FName, uint8>& Weights = NewWeapon.GetStatusEffectWeights();

	OptionalEffectsToApply.Empty(StatusEffects.Num());
	RandomWeights.Empty(Weights.Num());

	for (const TPair < FName, TSubclassOf<UStatusEffectData>>& Pair : StatusEffects)
	{
		OptionalEffectsToApply.Add(Pair.Key, Pair.Value);
	}

	for (const TPair < FName, uint8>& Pair : Weights)
	{
		RandomWeights.Add(Pair.Key, Pair.Value);
	}

	GenerateDescriptionString();
}
