// Fill out your copyright notice in the Description page of Project Settings.

#include "EpicWeaponObject.h"

UEpicWeaponObject::UEpicWeaponObject(const FObjectInitializer& Initializer) : Super(Initializer)
{
	Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;
	QualityType = WeaponQualityTypes::WEAPON_TYPE_EPIC;

	GenerateDescriptionString();
}


UEpicWeaponObject::UEpicWeaponObject(const UEpicWeaponObject& NewWeapon)
{
	Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;
	QualityType = WeaponQualityTypes::WEAPON_TYPE_LEGENDARY;

	CloneThis(&NewWeapon);
}

UEpicWeaponObject& UEpicWeaponObject::operator=(const UEpicWeaponObject& NewWeapon)
{
	CloneThis(&NewWeapon);

	return *this;
}

void UEpicWeaponObject::GenerateDescriptionString()
{

}

void UEpicWeaponObject::CloneThis(const UEpicWeaponObject* CopyThis)
{
	UWeaponObject::CloneThis(CopyThis);
	Initialize(*CopyThis);
}

void UEpicWeaponObject::Initialize(const UEpicWeaponObject& NewWeapon)
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

}

