// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "../../DataStructures/CombatData.h"
#include "../../DataStructures/StatusEffectData.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Characters/Players/Components/ActionVoicesComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "../PlayerCharacterBase.h"
#include "../../Enemies/EnemyCharacterBase.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (!bIsRegeneratingStamina)
	{
		if (CharacterCombatData->GetStaminaPercent() < 0.25f)
		{
			bIsRegeneratingStamina = true;
		}
	}
	else if (bIsRegeneratingStamina)
	{
		//default value, will get from traits later...
		CharacterCombatData->ModifyStaminaPoints(1.5f);

		if (CharacterCombatData->GetStaminaPercent() == 1.0f)
		{
			bIsRegeneratingStamina = false;
		}
	}
}


bool UCombatComponent::InitializeCombatComponent(const FName& Name, const uint8 Level, UEquipmentComponent*& EquipComp, UActionVoicesComponent*& Voices, ABattleCharacterBase* character)
{
	if (IsValid(Voices))
	{
		VoicesComp = &(*Voices);
	}

	if (IsValid(EquipComp))
	{
		EquipmentComp = &(*EquipComp);
	}

	if (IsValid(character))
	{
		BattleCharacter = &(*character);
	}

	if (IsValid(CharacterCombatData))
	{
		CharacterCombatData->ConditionalBeginDestroy();
		CharacterCombatData = nullptr;
	} 
	
	if (IsValid(CombatDataClass) && !IsValid(CharacterCombatData))
	{
		CharacterCombatData = NewObject<UCombatData>(this, CombatDataClass, FName("Combat Data"));
		CharacterCombatData->InitializeData(CharacterName, CurrentLevel);
	}

	EquippedWeaponLeft = (UEquippedWeaponObject*)EquipComp->GetEquippedLeftWeapon();
	EquippedWeaponRight = (UEquippedWeaponObject*)EquipComp->GetEquippedRightWeapon();

	bIsInitialized = (IsValid(character) && IsValid(CharacterCombatData) && IsValid(EquipmentComp) && IsValid(VoicesComp));

	return bIsInitialized;
}

void UCombatComponent::ComputeCombatResult(const FCombatDamageData* InDamageData, FCombatDamageResult& OutDamageResult)
{
	OutDamageResult.HitSuccess = false;
	OutDamageResult.HitIsCritical = false;
	OutDamageResult.IsStillAlive = true;
	OutDamageResult.DamageReceived = 0.0f;

	bool UseCriticalHitMod = InDamageData->bUseCriticalMod;
	float CriticalHitMod = InDamageData->CriticalModifier;

	if (IsValid(CharacterCombatData)) {

		const FExternalStats* DefenderStats = CharacterCombatData->GetExternalStats();
		float attacker_random = FMath::RandRange(InDamageData->InHitAcc / 2.0f, InDamageData->InHitAcc);
		float defender_evasion = FMath::RandRange(0.0f, DefenderStats->Evasion);

		float random_delta = attacker_random - defender_evasion;

		float damage_modifier = InDamageData->InPower / DefenderStats->DefensePower;
		float damage = FMath::FloorToFloat(InDamageData->InDamage * damage_modifier);

		//miss
		if (random_delta < 1.0f) {
			OutDamageResult.ResultText = FText::FromString(FString("MISSED!!"));
		}
		//hit
		else if (random_delta >= 1.0f && random_delta < 100.0f && damage >= 1.0f) {
			OutDamageResult.HitSuccess = true;
			OutDamageResult.DamageReceived = damage;
			if (InDamageData->bUsingComboAttack)
			{
				FString text = "Combo! ";
				text.Append(FString::SanitizeFloat(damage, 0));
				OutDamageResult.ResultText = FText::FromString(text);
			}
			else
			{
				OutDamageResult.ResultText = FText::FromString(FString::SanitizeFloat(damage, 0));
			}

		}
		//hit critical
		else if (random_delta >= 100.0f && damage >= 1.0f) {
			OutDamageResult.HitIsCritical = true;
			OutDamageResult.HitSuccess = true;
			float critical = 1.5f;

			if (UseCriticalHitMod)
			{
				critical += CriticalHitMod;
			}
			damage = FMath::FloorToFloat(damage *= critical);
			OutDamageResult.DamageReceived = damage;

			if (InDamageData->bUsingComboAttack)
			{
				FString DamageText = "Combo! Critical Hit!  ";
				DamageText.Append(FString::SanitizeFloat(damage, 0));
				OutDamageResult.ResultText = FText::FromString(DamageText);
			}
			else
			{
				FString DamageText = "Critical Hit! ";
				DamageText.Append(FString::SanitizeFloat(damage, 0));
				OutDamageResult.ResultText = FText::FromString(DamageText);
			}
		}
		else {
			OutDamageResult.ResultText = FText::FromString(FString("MISSED!!"));
		}

		if (OutDamageResult.HitSuccess) {
			OutDamageResult.DamageReceived = damage;
			if (!CharacterCombatData->ModifyHitPoints(-damage)) {
				OutDamageResult.IsStillAlive = false;
			}

		}

		bool applyEffect = (OutDamageResult.HitSuccess &&
			(OutDamageResult.HitIsCritical == InDamageData->bEffectOnCritical) &&
			(damage > 0.0f && InDamageData->bMustDamageForEffect) &&
			IsValid(InDamageData->StatusEffectClass)) ? true : false;

		if (applyEffect)
		{
			//AddStatusEffect(InDamageData->StatusEffectClass, InDamageData->StatusEffectName);
		}


	}
}

void UCombatComponent::ComputeRightWeaponDamage(ABattleCharacterBase* Target, const UAbilityBase* AttackAbility, const FVector & HitLocation, FCombatDamageResult * OutDamageResult, bool UseComboDamage)
{
	//ComputeWeaponDamage(Target, EquippedWeaponRight->GetWeaponObject().Get(), AttackAbility, HitLocation, OutDamageResult, UseComboDamage);
}

void UCombatComponent::ComputeLeftWeaponDamage(ABattleCharacterBase* Target, const UAbilityBase* AttackAbility, const FVector & HitLocation, FCombatDamageResult * OutDamageResult, bool UseComboDamage)
{
	//ComputeWeaponDamage(Target, EquippedWeaponLeft->GetWeaponObject().Get(), AttackAbility, HitLocation, OutDamageResult, UseComboDamage);
}

void UCombatComponent::ComputeWeaponDamage(ABattleCharacterBase* Target,
	UWeaponObject* AttackWeapon,
	const UAbilityBase* AttackAbility,
	const FVector& HitLocation,
	FCombatDamageResult* OutDamageResult, bool UseComboDamage)
{
	TWeakObjectPtr<APlayerCharacterBase> PlayerTarget = Cast<APlayerCharacterBase>(Target);
	TWeakObjectPtr<AEnemyCharacterBase> EnemyTarget = Cast<AEnemyCharacterBase>(Target);

	float WeaponDamage = 0.0f;

	if (IsValid(CharacterCombatData)) {
		const FExternalStats* PlayerStats = CharacterCombatData->GetExternalStats();

		float damage = 0.0f;

		if (IsValid(AttackAbility) && AttackAbility->IsValidLowLevel())
		{
			FCombatDamageData DamageData;
			DamageData.bUsingComboAttack = UseComboDamage;

			const UAttackAbility* attack = Cast<UAttackAbility>(AttackAbility);
			if (IsValid(attack) && attack->IsValidLowLevel()) {

				ComputeWeaponModifiers(DamageData, AttackWeapon);

				if (AttackAbility->GetAbilityType() == AbilityTypes::NORMAL_ATTACK)
				{
					ComputeNormalAttackDamage(AttackWeapon, AttackAbility, OutDamageResult, DamageData, Target, HitLocation);
				}
				else if (AttackAbility->GetAbilityType() == AbilityTypes::RAGE_ATTACK)
				{
					ComputeRageAttackDamage(AttackWeapon, AttackAbility, OutDamageResult, DamageData, Target, HitLocation);
				}
			}
		}
	}
}

bool UCombatComponent::AddRewardExp(const float exp)
{
	if (IsValid(CharacterCombatData))
	{
		CharacterCombatData->ModifyExp(exp);
		return CharacterCombatData->CheckForLevelUp();
	}

	return false;
}

void UCombatComponent::ComputeNormalAttackDamage(const UWeaponObject* AttackWeapon, const UAbilityBase* AttackAbility, FCombatDamageResult* OutDamageResult, FCombatDamageData& DamageData, TWeakObjectPtr<ABattleCharacterBase> Target, const FVector& HitLocation)
{
	const UAttackAbility* attack = Cast<UAttackAbility>(AttackAbility);
	if (IsValid(attack) && attack->IsValidLowLevel())
	{
		float WeaponDamage = FMath::FRandRange(AttackWeapon->DamageMRangeMin, AttackWeapon->DamageMRangeMax);

		if (AttackAbility->GetAbilityType() == AbilityTypes::NORMAL_ATTACK)
		{
			if (DamageData.bUsingComboAttack)
			{
				DamageData.InDamage = WeaponDamage * (attack->DamageModifier + attack->ComboBonusMultiplier);
			}
			else
			{
				DamageData.InDamage = WeaponDamage * attack->DamageModifier;
			}

			CharacterCombatData->ModifyStaminaPoints(-attack->StaminaCost);
			
			const FExternalStats* PlayerStats = CharacterCombatData->GetExternalStats();

			DamageData.InHitAcc = PlayerStats->AttackAcc;
			DamageData.InPower = PlayerStats->AttackPower;

			if (Target->ComputeHitAndDamage(&DamageData, *OutDamageResult))
			{
				CharacterCombatData->ModifyRagePoints(attack->RageBonus);

				TWeakObjectPtr<UParticleSystem> WeaponParticle = nullptr;
				TWeakObjectPtr<USoundCue> WeaponHitSound = nullptr;

				if (OutDamageResult->HitIsCritical)
				{
					WeaponParticle = AttackWeapon->CriticalHitEffect;
					WeaponHitSound = Cast<USoundCue>(AttackWeapon->CriticalHitSound);
				}
				else
				{
					WeaponParticle = AttackWeapon->WeaponHitEffect;
					WeaponHitSound = Cast<USoundCue>(AttackWeapon->WeaponHitSound);
				}
				if (WeaponParticle.IsValid() && WeaponHitSound.IsValid())
				{
					FTransform transform;
					transform.SetLocation(HitLocation);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponParticle.Get(), transform);
					UGameplayStatics::SpawnSoundAtLocation(this, WeaponHitSound.Get(), HitLocation);
					VoicesComp->PlayDeliverCriticalhitVoice();
				}
			}
		}
	}
}

void UCombatComponent::ComputeRageAttackDamage(const UWeaponObject* AttackWeapon, const UAbilityBase* AttackAbility, FCombatDamageResult* OutDamageResult, FCombatDamageData& DamageData, TWeakObjectPtr<ABattleCharacterBase> Target, const FVector& HitLocation)
{
	const URageAttackAbility* rage = Cast<URageAttackAbility>(AttackAbility);
	if (IsValid(rage) && rage->IsValidLowLevel())
	{
		CharacterCombatData->ModifyStaminaPoints(-rage->StaminaCost);
		CharacterCombatData->ModifyRagePoints(-rage->RageCost);

		float WeaponDamage = FMath::FRandRange(AttackWeapon->DamageMRangeMin, AttackWeapon->DamageMRangeMax);

		const FExternalStats* PlayerStats = CharacterCombatData->GetExternalStats();

		DamageData.InDamage = WeaponDamage * rage->RageDamageModifier;
		float acc = PlayerStats->AttackAcc + rage->AttackACCBonus;
		float power = PlayerStats->AttackPower + rage->AttackPowerBonus;

		DamageData.InHitAcc = acc;
		DamageData.InPower = power;

		CharacterCombatData->ModifyStaminaPoints(rage->StaminaCost);

		if (Target->ComputeHitAndDamage(&DamageData, *OutDamageResult))
		{
			CharacterCombatData->ModifyRagePoints(-rage->RageCost);

			TWeakObjectPtr<UParticleSystem> WeaponParticle = nullptr;
			TWeakObjectPtr<USoundCue> WeaponHitSound = nullptr;

			if (OutDamageResult->HitIsCritical)
			{
				WeaponParticle = AttackWeapon->CriticalHitEffect;
				WeaponHitSound = Cast<USoundCue>(AttackWeapon->CriticalHitSound);
			}
			else
			{
				WeaponParticle = AttackWeapon->WeaponHitEffect;
				WeaponHitSound = Cast<USoundCue>(AttackWeapon->WeaponHitSound);
			}
			if (WeaponParticle.IsValid() && WeaponHitSound.IsValid())
			{
				FTransform transform;
				transform.SetLocation(HitLocation);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponParticle.Get(), transform);
				UGameplayStatics::SpawnSoundAtLocation(this, WeaponHitSound.Get(), HitLocation);
				VoicesComp->PlayDeliverCriticalhitVoice();
			}
		}
	}
}

void UCombatComponent::ComputeWeaponModifiers(FCombatDamageData& DamageData, UWeaponObject* AttackWeapon)
{
	TWeakObjectPtr<UEpicWeaponObject> EpicWeapon = Cast<UEpicWeaponObject>(AttackWeapon);
	TWeakObjectPtr<ULegendaryWeaponObject> Legendary = Cast<ULegendaryWeaponObject>(AttackWeapon);

	if (EpicWeapon.IsValid())
	{
		DamageData.bUseCriticalMod = EpicWeapon->bUsesCriticalHitModifier;
		DamageData.CriticalModifier = EpicWeapon->CriticalHitModifier;

		if (IsValid(EpicWeapon->StatusEffectToApply))
		{
			DamageData.bEffectOnCritical = EpicWeapon->bMustBeCriticalToApplyStatus;
			DamageData.bMustDamageForEffect = EpicWeapon->bMustDeliverBaseDamageMin;
			DamageData.StatusEffectClass = EpicWeapon->StatusEffectToApply;
			TWeakObjectPtr<UStatusEffectData> StatusEffect = Cast<UStatusEffectData>(EpicWeapon->StatusEffectToApply->GetDefaultObject());
			DamageData.StatusEffectName = StatusEffect->StatusEffectName;

		}
	}
	else if (Legendary.IsValid())
	{
		DamageData.bUseCriticalMod = Legendary->bUsesCriticalHitModifier;
		DamageData.CriticalModifier = Legendary->CriticalHitModifier;

		const TPair<FName, TSubclassOf<UStatusEffectData>>* StatusEffectPair = Legendary->GetRandomStatusEffect();

		if (StatusEffectPair)
		{
			TWeakObjectPtr<UStatusEffectData> StatusEffect = Cast<UStatusEffectData>(StatusEffectPair->Value->GetDefaultObject());
			if (StatusEffect.IsValid())
			{
				DamageData.bEffectOnCritical = Legendary->bMustBeCriticalToApplyStatus;
				DamageData.bMustDamageForEffect = Legendary->bMustDeliverBaseDamageMin;
				DamageData.StatusEffectClass = Legendary->StatusEffectToApply;

				DamageData.StatusEffectName = StatusEffectPair->Key;

			}
		}
	}
}
