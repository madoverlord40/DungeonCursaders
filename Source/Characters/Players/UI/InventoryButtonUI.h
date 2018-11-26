// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "InventoryButtonUI.generated.h"

class UItemInventoryUI;
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UInventoryButtonUI : public UButton
{
	GENERATED_BODY()
	
	
public:
	UInventoryButtonUI();

	FName ButtonName;
	uint8 ArrayItemIndex;

	UFUNCTION()
	void OnHoveredEvent();

	UFUNCTION()
	void OnClickedEvent();
	
	TWeakObjectPtr<UItemInventoryUI> ParentUI;
};
