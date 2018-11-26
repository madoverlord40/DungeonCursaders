// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "AbilityBase.generated.h"

class UTexture2D;

UENUM(Blueprintable)
enum AbilityTypes {
	NONE,
	GENERIC_ABILITY,
	NORMAL_ATTACK,
	RAGE_ATTACK,
	MAGIC_ATTACK
};
/**
 * 
 */
UCLASS(Blueprintable)
class DUNGEONCRUSADES_API UAbilityBase : public UObject
{
	GENERATED_BODY()
	
public:
	UAbilityBase(const FObjectInitializer& ObjectInitializer);

	TEnumAsByte<AbilityTypes> GetAbilityType() const { return AbilityType; }
	FName& GetAbilityName() { return AbilityName; }
	uint8 GetLevelRequirement() { return LevelRequirement; }

	bool GetHasCooldown() { return HasCooldown; }
	float GetCooldownTime() { return CooldownTime; }
	UTexture2D* GetIconTexture() { return IconTexture; }

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<AbilityTypes> AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 LevelRequirement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasCooldown = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconTexture;

};
