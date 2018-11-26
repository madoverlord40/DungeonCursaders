// Fill out your copyright notice in the Description page of Project Settings.

#include "LegendaryArmorObject.h"


ULegendaryArmorObject::ULegendaryArmorObject() : Super()
{
	Type = ItemTypes::ITEM_EQUIPABLE_ARMOR;
	QualityType = ArmorQualityTypes::ARMOR_QUALITY_LEGENDARY;
}

ULegendaryArmorObject::ULegendaryArmorObject(const ULegendaryArmorObject& NewArmor)
{
	Type = ItemTypes::ITEM_EQUIPABLE_ARMOR;
	QualityType = ArmorQualityTypes::ARMOR_QUALITY_LEGENDARY;
}

void ULegendaryArmorObject::CloneThis(const ULegendaryArmorObject* CopyThis)
{
	UEpicArmorObject::CloneThis(CopyThis);

	FMemory::Memcpy(ExternalAugmentation, CopyThis->ExternalAugmentation);

	StatusEffectImmunities = CopyThis->StatusEffectImmunities;

	bAttackerTakesDamage = CopyThis->bAttackerTakesDamage;

	ReflectDamagePercent = CopyThis->ReflectDamagePercent;

	ReflectDamageType = CopyThis->ReflectDamageType;

	ReflectElementalType = ReflectElementalType;
}
