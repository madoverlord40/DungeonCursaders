// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "AbilityButton.generated.h"

class UPlayerAbilityBar;

/**
 * 
 */
UCLASS(Blueprintable)
class DUNGEONCRUSADES_API UAbilityButton : public UButton
{
	GENERATED_BODY()
	
public:
	UAbilityButton();

	FName ButtonName;
	FName AbilitySlotName;

	UFUNCTION()
	void OnHoveredEvent();

	UFUNCTION()
	void OnClickedEvent();

	TWeakObjectPtr<UPlayerAbilityBar> ParentUI;
	
	
};
