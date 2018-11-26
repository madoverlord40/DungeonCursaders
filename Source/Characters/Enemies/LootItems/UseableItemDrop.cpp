// Fill out your copyright notice in the Description page of Project Settings.

#include "UseableItemDrop.h"
#include "Characters/DataStructures/ItemObject.h"

AUseableItemDrop::AUseableItemDrop(const FObjectInitializer& Initializer) : Super(Initializer)
{
	
}

void AUseableItemDrop::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(ItemObjectToDropClass))
	{
		UStaticMesh* mesh = ItemObjectToDropClass->GetDefaultObject<UItemObject>()->StaticMeshBase;

		ItemMesh->SetStaticMesh(mesh);
		
		ItemMesh->SetRelativeTransform(RelativeOffset);

	}
}

