// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatData.h"
#include "StatusEffectData.generated.h"


UENUM(BlueprintType)
enum class StatModifyTypes :uint8
{
	MODIFY_HEALTH,
	MODIFY_STAMINA,
	MODIFY_MANA,
	MODIFY_ATTACK,
	MODIFY_DEFENSE,
	MODIFY_SPEED,
	MODIFY_EVASION,
	MODIFY_MELEE_ACC,
	MODIFY_INTERNALS,
	MODIFY_RESISTANCES,
	MODIFY_NONE
};

UENUM(BlueprintType)
enum class StatusEffectTypes :uint8
{
	STATUS_POISON,
	STATUS_ENFLAMED,
	STATUS_FREEZING,
	STATUS_ELECTOFIED,
	//slower movement and animations
	STATUS_SLOWED,
	//cant move
	STATUS_STUNNED,
	//no healing or regen of any kind
	STATUS_CURSED,
	//holy
	STATUS_PRUIFIED,
	//water
	STATUS_DROWNING,
	//modify Externals directly
	STATUS_MODIFIER,
	STATUS_NONE
};

class UParticleSystem;
class USoundCue;

/**
 * 
 */
UCLASS(Blueprintable)
class DUNGEONCRUSADES_API UStatusEffectData : public UObject
{
	GENERATED_BODY()
	
	
public:

	UStatusEffectData(const FObjectInitializer& Initializer = FObjectInitializer::Get());

	/**
	* Called after the C++ constructor and after the properties have been initialized, including those loaded from config.
	* mainly this is to emulate some behavior of when the constructor was called after the properties were initialized.
	*/
	virtual void PostInitProperties() override;

	UPROPERTY(EditAnywhere)
	StatModifyTypes StatToModify;

	UPROPERTY(EditAnywhere)
	StatusEffectTypes StatusEffect;

	UPROPERTY(EditAnywhere)
	UParticleSystem* EffectParticle;

	UPROPERTY(EditAnywhere)
	USoundCue* SoundEffect;

	UPROPERTY(EditAnywhere)
	FName StatusEffectName;

	UPROPERTY(EditAnywhere)
	float BaseModifier;

	UPROPERTY(EditAnywhere)
	float TimeInterval;
	
	UPROPERTY(EditAnywhere)
	FInternalStats InternalModifier;

	UPROPERTY(EditAnywhere)
	TArray<FElementalResistData> ElementalEffects;
	
	/*if this is a tickable damage effect like poison, set to true and set the tick duration to go with it.*/
	UPROPERTY(EditAnywhere)
	uint32 HasTickIntervals : 1;

	/* how many timer ticks should this status effect last for*/
	UPROPERTY(EditAnywhere)
	uint8 TickDuration;

	UPROPERTY(EditAnywhere)
	float SlowedWalkingSpeed;

	UPROPERTY(EditAnywhere)
	float SlowedAnimationSpeed;


	//something for the icon in widget??


};
