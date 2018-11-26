// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemObject.h"


UItemObject::UItemObject(const FObjectInitializer& Inititializer) : Super(Inititializer)
{
	Type = ItemTypes::ITEM_NONE;
	GenerateDescriptionString();
}

void UItemObject::CloneThis(const UItemObject* CopyThis)
{
	ItemName = CopyThis->ItemName;

	StaticMeshBase = CopyThis->StaticMeshBase;

	InventoryIconTexture = CopyThis->InventoryIconTexture;

	Type = CopyThis->GetItemType();
	DescriptionString = GetDescriptionString();
}

void UItemObject::GenerateDescriptionString()
{
	DescriptionString = "This item has no value or use...";
}

