// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/AbilityBase.h"
#include "AttackAbility.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DUNGEONCRUSADES_API UAttackAbility : public UAbilityBase
{
	GENERATED_BODY()
	
public:
	UAttackAbility(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RageBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackACCBonus = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPowerBonus = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AnimationSectionName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bIsAreaAttack : 1;

	/*during ability bar click, use these to flag  combo and name
	  when ability is clicked, check to see if these are already set in controller before executing
	  and pull combo bonus. Call down to controller that we are executing a combo and with which ability
	  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bHasComboAttack : 1;

	/*what ability are we able to combo with*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ComboWithAbilityName = NAME_None;

	/*if combo is detected, use this multiplier on execution of ability*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ComboBonusMultiplier = 1.0f;
	
};
