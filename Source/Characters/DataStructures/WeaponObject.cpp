// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponObject.h"


UWeaponObject::UWeaponObject(const FObjectInitializer& Inititializer) : Super(Inititializer)
{
	QualityType = WeaponQualityTypes::WEAPON_TYPE_NORMAL;
}

void UWeaponObject::CloneThis(const UWeaponObject* CopyThis)
{

}

void UWeaponObject::GenerateDescriptionString()
{

}

