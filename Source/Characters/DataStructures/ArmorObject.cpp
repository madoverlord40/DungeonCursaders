// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmorObject.h"
#include "Runtime/Engine/Classes/Engine/SkeletalMesh.h"



UArmorObject::UArmorObject()
{
	Type = ItemTypes::ITEM_EQUIPABLE_ARMOR;
	QualityType = ArmorQualityTypes::ARMOR_QUALITY_NORMAL;
	
}

UArmorObject::UArmorObject(const UArmorObject& NewArmor)
{
	Type = ItemTypes::ITEM_EQUIPABLE_ARMOR;
	QualityType = ArmorQualityTypes::ARMOR_QUALITY_NORMAL;
	
}

void UArmorObject::CloneThis(const UArmorObject* CopyThis)
{
	UItemObject::CloneThis(CopyThis);

	FMemory::Memcpy(ElementalResistance, CopyThis->ElementalResistance);
	
	FMemory::Memcpy(DamageResistance, CopyThis->DamageResistance);

	DefensePower = CopyThis->DefensePower;

	MagicDefense = CopyThis->MagicDefense;

	SkinedMesh = CopyThis->SkinedMesh;

	ArmorSlot = CopyThis->ArmorSlot;

	QualityType = CopyThis->GetQualityType();

}

UArmorObject& UArmorObject::operator=(const UArmorObject& NewArmor)
{

	return *this;
}