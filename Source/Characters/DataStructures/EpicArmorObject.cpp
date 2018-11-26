// Fill out your copyright notice in the Description page of Project Settings.

#include "EpicArmorObject.h"


UEpicArmorObject::UEpicArmorObject() : Super()
{
	Type = ItemTypes::ITEM_EQUIPABLE_ARMOR;
	QualityType = ArmorQualityTypes::ARMOR_QUALITY_EPIC;
}

UEpicArmorObject::UEpicArmorObject(const UEpicArmorObject& NewArmor)
{
	Type = ItemTypes::ITEM_EQUIPABLE_ARMOR;
	QualityType = ArmorQualityTypes::ARMOR_QUALITY_EPIC;
}

void UEpicArmorObject::CloneThis(const UEpicArmorObject* CopyThis)
{
	UArmorObject::CloneThis(CopyThis);

	ElementalResistances.Empty();

	for (const FElementalResistData& ResistData : CopyThis->ElementalResistances)
	{
		ElementalResistances.Add(ResistData);
	}
	
	DamageResistances.Empty();

	for (const FDamageResistData& ResistData : CopyThis->DamageResistances)
	{
		DamageResistances.Add(ResistData);
	}

	FMemory::Memcpy(InternalAugmentation, CopyThis->InternalAugmentation);
}

UEpicArmorObject& UEpicArmorObject::operator=(const UEpicArmorObject& NewArmor)
{
	CloneThis(&NewArmor);

	return *this;
}