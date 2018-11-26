// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FInternalStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Vitality = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Agility = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Dexterity = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mind = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Intellect = 0.0f;
};


USTRUCT(BlueprintType)
struct FExternalStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefensePower = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicPower = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicDefense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Evasion = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackAcc = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthPoints = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthPointsMax = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaPoints = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaPointsMax = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FuryPoints = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FuryPointsMax = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StaminaPoints = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StaminaPointsMax = 0.0f;

};

//used to configure external modifiers
UENUM(BlueprintType)
enum class BattleConfigTypes : uint8 {
	Config_Weak UMETA(DisplayName = "Weak Enemy"),
	Config_Easy UMETA(DisplayName = "Easy Enemy"),
	Config_Normal UMETA(DisplayName = "Normal Enemy"),
	Config_Hard UMETA(DisplayName = "Hard Enemy"),
	Config_Elite UMETA(DisplayName = "Elite Enemy"),
	Config_Boss UMETA(DisplayName = "BOSS"),
	Config_PlayerCharacter UMETA(DisplayName = "Player")
};

//used to determine base internal stats lineup
UENUM(BlueprintType)
enum class ClassConfigTypes :uint8 {
	Config_SmallMelee UMETA(DisplayName = "Rogue"), //rogues
	Config_MediumMelee UMETA(DisplayName = "Warrior"), //warriors
	Config_HeavyMelee UMETA(DisplayName = "Knight"), //tanks
	Config_Magic	  UMETA(DisplayName = "Sorceror"),	   //mage
	Config_MeleeMagic UMETA(DisplayName = "Warlock"), //warlock
	Config_MeleeRanged UMETA(DisplayName = "Ranger"),//Range
	Config_MeleeRangedMagic UMETA(DisplayName = "Mercinary")//JackOfAll
};

UENUM(BlueprintType)
enum class DamageTypes :uint8
{
	DamageType_NONE,
	DamageType_Slashing,
	DamageType_Blunt, 
	DamageType_Choping,
	DamageType_Elemental,
	DamageType_Crushing,
	DamageType_Stun,
	DamageType_Normal
};

UENUM(BlueprintType)
enum class ElementalDamageTypes :uint8
{
	ElementType_NONE,
	ElementType_Fire,
	ElementType_Ice,
	ElementType_Shock,
	ElementType_Wind,
	ElementType_Earth,
	ElementType_Water,
	ElementType_Poison,
	ElementType_Curse,
	ElementType_Holy,
	ElementType_Stun,
	ElementType_Slow
};


USTRUCT(BlueprintType)
struct FStatusEffectResistances
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float PoisonResistanceModifier = 1.0f;

	UPROPERTY(EditAnywhere)
	float StunResistanceModifier = 1.0f;

	UPROPERTY(EditAnywhere)
	float SlowResistanceModifier = 1.0f;

	UPROPERTY(EditAnywhere)
	float CurseResistanceModifier = 1.0f;

};

USTRUCT(BlueprintType)
struct FDamageResistData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	DamageTypes ResistDamageType = DamageTypes::DamageType_NONE;

	UPROPERTY(EditAnywhere)
	float Modifier = 1.0f;
};

USTRUCT(BlueprintType)
struct FElementalResistData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	ElementalDamageTypes ResistDamageType = ElementalDamageTypes::ElementType_NONE;

	UPROPERTY(EditAnywhere)
	float Modifier = 1.0f;
};


UCLASS(Blueprintable)
class DUNGEONCRUSADES_API UCombatData : public UObject
{
	GENERATED_BODY()
	
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInternalStats Internals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FExternalStats Externals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExperiencePoints = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NextLevelUp = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	BattleConfigTypes BattleConfigType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ClassConfigTypes ClassConfigType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStatusEffectResistances StatusResistances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDamageResistData> DamageResistances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FElementalResistData> ElementDamageResistances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 IsTwoHanded : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 IsDualWeilding : 1;

public:
	UFUNCTION(BlueprintCallable)
	void InitializeData(const FName& Name, const uint8 StartingLevel);

	UFUNCTION(BlueprintCallable)
	void ModifyExp(float EXP);

	UFUNCTION(BlueprintCallable)
	bool CheckForLevelUp();

	void AugmentInternals(const FInternalStats* const augmented);

	void AugmentExternals(const FExternalStats* const augmented);

	void AugmentDamageResistance(const FDamageResistData* const augmented, bool remove = false);

	void AugmentElementalResistance(const FElementalResistData* const augmented, bool remove = false);

	void SetInternals(const FInternalStats* const augmented);

	void SetExternals(const FExternalStats* const augmented);

	const FExternalStats* GetExternalStats() const { return &Externals; }

	const FInternalStats* GetInternalStats() const { return &Internals; }

	const FStatusEffectResistances* GetStatusResistances() const { return &StatusResistances; }

	const TArray<FElementalResistData>* GetElemenalResistances() const { return &ElementDamageResistances; }
	
	const TArray<FDamageResistData>* GetDamageResistances() const { return &DamageResistances; }

	void ModifyAttackPower(float value);
	void ModifyDefensePower(float value);
	void ModifyMagicPower(float value);
	void ModifyMagicDefensePower(float value);
	void ModifyAcc(float value);
	void ModifyEvasion(float value);

	UFUNCTION(BlueprintCallable)
	const FName& GetCharacterName() const { return CharacterName; }

	UFUNCTION(BlueprintCallable)
	const uint8 GetCharacterLevel() const { return Level; }

	UFUNCTION(BlueprintCallable)
	const float GetCharacterEXP() const { return ExperiencePoints; }

	UFUNCTION(BlueprintCallable)
	bool ModifyHitPoints(const float value);

    UFUNCTION(BlueprintCallable)
    void ModifyStaminaPoints(const float value);

    UFUNCTION(BlueprintCallable)
    void ModifyManaPoints(const float value);
    
    UFUNCTION(BlueprintCallable)
    void ModifyRagePoints(const float value);
	
    UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable)
	float GetManaPercent() const;

	UFUNCTION(BlueprintCallable)
	float GetStaminaPercent() const;

	UFUNCTION(BlueprintCallable)
	float GetRagePercent() const;

	UFUNCTION(BlueprintCallable)
	float GetExpPercent() const;

	UFUNCTION(BlueprintCallable)
	uint8 GetCurrentLevel() const { return Level; }

	const FName& GetClassName() const { return ClassTypeName; }

	const bool GetIsTwoHanded() const { return IsTwoHanded; }

	const bool GetIsDualWeilding() const { return IsDualWeilding; }

private:
	void ConfigureClass(uint8 level);
	void ConfigureBattleModifiers();

	FName ClassTypeName;
};
