// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/AttackAbility.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
#include "RageAttackAbility.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DUNGEONCRUSADES_API URageAttackAbility : public UAttackAbility
{
	GENERATED_BODY()
	
public:
	URageAttackAbility(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RageCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RageDamageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ParticleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SoundEffect;

};
