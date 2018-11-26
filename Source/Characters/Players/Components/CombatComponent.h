// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.h"
#include "Characters/Players/Components/ActionVoicesComponent.h"
#include "CombatComponent.generated.h"

class ABattleCharacterBase;

class UEquipmentComponent;
class UEquippedArmorObject;
class UEquippedWeaponObject;
class UWeaponObject;
class UCombatData;
class UAbilityBase;


USTRUCT(BlueprintType)
struct FCombatDamageResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float DamageReceived;

	UPROPERTY()
	uint32 HitSuccess : 1;

	UPROPERTY()
	uint32 HitIsCritical : 1;

	UPROPERTY()
	uint32 IsStillAlive : 1;

	UPROPERTY()
	uint32 AppliedStatusEffect : 1;

	UPROPERTY()
	FText ResultText;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCRUSADES_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UCombatData* GetCharacterCombatData() const { return CharacterCombatData; }

	void ComputeCombatResult(const FCombatDamageData* InDamageData, FCombatDamageResult& OutDamageResult);
	void ComputeRightWeaponDamage(ABattleCharacterBase* Target, const UAbilityBase* AttackAbility, const FVector& HitLocation, FCombatDamageResult* OutDamageResult, bool UseComboDamage = false);
	void ComputeLeftWeaponDamage(ABattleCharacterBase* Target, const UAbilityBase* AttackAbility, const FVector& HitLocation, FCombatDamageResult* OutDamageResult, bool UseComboDamage = false);

	bool InitializeCombatComponent(const FName& Name, const uint8 Level, UEquipmentComponent*& EquipComp, UActionVoicesComponent*& Voices, ABattleCharacterBase* character);

	void ComputeWeaponDamage(ABattleCharacterBase* Target, UWeaponObject* AttackWeapon, const UAbilityBase* AttackAbility, const FVector& HitLocation, FCombatDamageResult* OutDamageResult, bool UseComboDamage = false);

	bool AddRewardExp(const float exp);

	const UEquippedWeaponObject* GetLeftEquippedWeapon() const { return EquippedWeaponLeft; }
	const UEquippedWeaponObject* GetRightEquippedWeapon() const { return EquippedWeaponRight; }

	const UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComp; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	TSubclassOf<UCombatData> CombatDataClass;

	//if true, saves state to memory and to disk
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	uint32 bUsesSaveState : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	uint8 CurrentLevel;

private:
	
	void ComputeNormalAttackDamage(const UWeaponObject* AttackWeapon, const UAbilityBase* AttackAbility, FCombatDamageResult* OutDamageResult, FCombatDamageData& DamageData, TWeakObjectPtr<ABattleCharacterBase> Target, const FVector& HitLocation);
	void ComputeRageAttackDamage(const UWeaponObject* AttackWeapon, const UAbilityBase* AttackAbility, FCombatDamageResult* OutDamageResult, FCombatDamageData& DamageData, TWeakObjectPtr<ABattleCharacterBase> Target, const FVector& HitLocation);
	void ComputeWeaponModifiers(FCombatDamageData& DamageData, UWeaponObject* AttackWeapon);

	UPROPERTY()
	ABattleCharacterBase* BattleCharacter;

	UPROPERTY()
	UEquippedWeaponObject* EquippedWeaponLeft;

	UPROPERTY()
	UEquippedWeaponObject* EquippedWeaponRight;

	UPROPERTY()
	UCombatData* CharacterCombatData;
	
	UPROPERTY()
	UEquipmentComponent* EquipmentComp;

	UPROPERTY()
	UActionVoicesComponent* VoicesComp;

	uint32 bIsInitialized : 1;
	uint32 bIsRegeneratingStamina : 1;
};
