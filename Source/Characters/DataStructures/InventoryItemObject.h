// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Characters/Players/UI/ItemInventoryUI.h"
#include "InventoryItemObject.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UInventoryItemObject : public UObject
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY()
	FInventoryItemInfo InventoryInfo;

	UPROPERTY()
	UUserWidget* DragWidget;

	UPROPERTY()
	UUserWidget* DragFromParentWidget;

	UPROPERTY()
	UUserWidget* SelfWidget;

	UPROPERTY()
	uint32 bCanDeleteOnDrop : 1;
};
