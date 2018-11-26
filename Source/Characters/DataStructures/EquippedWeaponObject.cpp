// Fill out your copyright notice in the Description page of Project Settings.

#include "EquippedWeaponObject.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Characters/BattleCharacterBase.h"

UEquippedWeaponObject::UEquippedWeaponObject()
{
	MainWeaponObject = nullptr;
	EpicWeapon = nullptr;
	LegendaryWeapon = nullptr;
	WeaponParticle = nullptr;
}

UEquippedWeaponObject::UEquippedWeaponObject(const UEquippedWeaponObject& NewWeapon)
{
	Initialize(NewWeapon);
}

void UEquippedWeaponObject::BeginDestroy()
{
	if (IsValid(MainWeaponObject))
	{
		MainWeaponObject->ConditionalBeginDestroy();
	}

	Super::BeginDestroy();
}

void UEquippedWeaponObject::CreateThis(ABattleCharacterBase* BattleCharacter, const FName& Name, TSubclassOf<UWeaponObject> WeaponClass)
{
	if (IsValid(WeaponClass))
	{
		OwningBattleCharacter = &(*BattleCharacter);

		if (IsValid(MainWeaponObject))
		{
			MainWeaponObject->ConditionalBeginDestroy();
			MainWeaponObject = nullptr;
		}

		if (IsValid(OwningMeshComp))
		{
			OwningMeshComp->UnregisterComponent();
			OwningMeshComp->DestroyComponent();
			OwningMeshComp = nullptr;
		}

		if (IsValid(WeaponCollisionComp))
		{
			WeaponCollisionComp->UnregisterComponent();
			WeaponCollisionComp->DestroyComponent();
			WeaponCollisionComp = nullptr;
		}

		MainWeaponObject = NewObject<UWeaponObject>(this, WeaponClass, Name);

		EpicWeapon = Cast<UEpicWeaponObject>(MainWeaponObject);
		LegendaryWeapon = Cast<ULegendaryWeaponObject>(MainWeaponObject);

		Description = MainWeaponObject->GetDescriptionString();

		WeaponCollisionComp = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass());
		WeaponCollisionComp->RegisterComponent();
		WeaponCollisionComp->SetCapsuleHalfHeight(MainWeaponObject->EquipWeaponData.CapsuleHeight);
		WeaponCollisionComp->SetCapsuleRadius(MainWeaponObject->EquipWeaponData.CapsuleRadius);
		WeaponCollisionComp->SetCollisionProfileName(MainWeaponObject->EquipWeaponData.CollisionProfileName);
		WeaponCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponCollisionComp->SetCanEverAffectNavigation(false);
		WeaponCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UEquippedWeaponObject::OnBeginWeaponOverlap);
	}
}

void UEquippedWeaponObject::EquipThis(UMeshComponent* WeaponMeshComp, UMeshComponent* AttachTo)
{
	if (IsValid(WeaponMeshComp) && IsValid(MainWeaponObject))
	{
		USkeletalMeshComponent* BodyMesh = Cast<USkeletalMeshComponent>(AttachTo);
		UStaticMeshComponent* WeaponMesh = Cast<UStaticMeshComponent>(WeaponMeshComp);

		if (IsValid(WeaponMesh))
		{
			WeaponMesh->SetCanEverAffectNavigation(false);
			WeaponMesh->SetStaticMesh(MainWeaponObject->StaticMeshBase);
		}

		if (IsValid(BodyMesh))
		{
			WeaponMeshComp->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform, MainWeaponObject->EquipWeaponData.SocketName);
			OwningMeshComp = WeaponMesh;
		}

		WeaponCollisionComp->AttachToComponent(WeaponMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
		WeaponCollisionComp->SetRelativeTransform(MainWeaponObject->EquipWeaponData.CapsuleOrientation);

		if (IsValid(EpicWeapon) && IsValid(EpicWeapon->WeaponStatusEffectParticle))
		{
			if (IsValid(WeaponParticle))
			{
				WeaponParticle->UnregisterComponent();
				WeaponParticle->DestroyComponent();
			}
			WeaponParticle = UGameplayStatics::SpawnEmitterAttached(
				EpicWeapon->WeaponStatusEffectParticle,
				BodyMesh,
				EpicWeapon->StatusParticleSocketName,
				EpicWeapon->RelativeSocketOffset,
				EpicWeapon->RelativeSocketRotation,
				EAttachLocation::KeepRelativeOffset, false);

			WeaponParticle->ActivateSystem();
		}
	}
}

void UEquippedWeaponObject::EquipThis(USkeletalMeshComponent* AttachTo, bool CreateWeaponMesh)
{
	
	if (!IsValid(OwningMeshComp) && IsValid(MainWeaponObject) && IsValid(AttachTo))
	{
		if (CreateWeaponMesh)
		{
			OwningMeshComp = NewObject<UStaticMeshComponent>(this);
			OwningMeshComp->RegisterComponent();
			OwningMeshComp->SetStaticMesh(MainWeaponObject->StaticMeshBase);
			OwningMeshComp->AttachToComponent(AttachTo, FAttachmentTransformRules::KeepRelativeTransform, MainWeaponObject->EquipWeaponData.SocketName);
			OwningMeshComp->SetCanEverAffectNavigation(false);

			WeaponCollisionComp->AttachToComponent(OwningMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
		}
		else
		{
			WeaponCollisionComp->AttachToComponent(AttachTo, FAttachmentTransformRules::KeepRelativeTransform, MainWeaponObject->EquipWeaponData.SocketName);
		}

		WeaponCollisionComp->SetRelativeTransform(MainWeaponObject->EquipWeaponData.CapsuleOrientation);
		WeaponCollisionComp->SetHiddenInGame(false);

		if (IsValid(EpicWeapon) && IsValid(EpicWeapon->WeaponStatusEffectParticle))
		{
			if (IsValid(WeaponParticle))
			{
				WeaponParticle->UnregisterComponent();
				WeaponParticle->DestroyComponent();
			}
			WeaponParticle = UGameplayStatics::SpawnEmitterAttached(
				EpicWeapon->WeaponStatusEffectParticle,
				AttachTo,
				EpicWeapon->StatusParticleSocketName,
				EpicWeapon->RelativeSocketOffset,
				EpicWeapon->RelativeSocketRotation,
				EAttachLocation::KeepRelativeOffset, false);

			WeaponParticle->ActivateSystem();
		}
	}
}

void UEquippedWeaponObject::ConfigureThis(UCombatData*& CombatStats, const bool IsLeftHand)
{
	if (IsValid(EpicWeapon))
	{
		CombatStats->AugmentInternals(&EpicWeapon->InternalStatAugmentation);
	}

	if (IsValid(LegendaryWeapon))
	{
		CombatStats->AugmentExternals(&LegendaryWeapon->ExternalStatAugmentation);
	}

	bIsLeftHandWeapon = IsLeftHand;
	bHasBeenConfigured = true;
}

void UEquippedWeaponObject::UnEquipThis(UCombatData*& CombatStats)
{
	if (IsValid(EpicWeapon))
	{
		if (bHasBeenConfigured)
		{
			FInternalStats RemoveStats;

			RemoveStats.Agility = -EpicWeapon->InternalStatAugmentation.Agility;
			RemoveStats.Dexterity = -EpicWeapon->InternalStatAugmentation.Dexterity;
			RemoveStats.Intellect = -EpicWeapon->InternalStatAugmentation.Intellect;
			RemoveStats.Mind = -EpicWeapon->InternalStatAugmentation.Mind;
			RemoveStats.Strength = -EpicWeapon->InternalStatAugmentation.Strength;
			RemoveStats.Vitality = -EpicWeapon->InternalStatAugmentation.Vitality;

			CombatStats->AugmentInternals(&RemoveStats);
		}
	}

	if (IsValid(LegendaryWeapon))
	{
		if (bHasBeenConfigured)
		{
			FInternalStats RemoveStats;

			RemoveStats.Agility = -LegendaryWeapon->InternalStatAugmentation.Agility;
			RemoveStats.Dexterity = -LegendaryWeapon->InternalStatAugmentation.Dexterity;
			RemoveStats.Intellect = -LegendaryWeapon->InternalStatAugmentation.Intellect;
			RemoveStats.Mind = -LegendaryWeapon->InternalStatAugmentation.Mind;
			RemoveStats.Strength = -LegendaryWeapon->InternalStatAugmentation.Strength;
			RemoveStats.Vitality = -LegendaryWeapon->InternalStatAugmentation.Vitality;

			CombatStats->AugmentInternals(&RemoveStats);

			FExternalStats RemoveExternals;
			RemoveExternals.AttackAcc = -LegendaryWeapon->ExternalStatAugmentation.AttackAcc;
			RemoveExternals.AttackPower = -LegendaryWeapon->ExternalStatAugmentation.AttackPower;
			RemoveExternals.DefensePower = -LegendaryWeapon->ExternalStatAugmentation.DefensePower;
			RemoveExternals.Evasion = -LegendaryWeapon->ExternalStatAugmentation.Evasion;
			RemoveExternals.FuryPoints = -LegendaryWeapon->ExternalStatAugmentation.FuryPoints;
			RemoveExternals.FuryPointsMax = -LegendaryWeapon->ExternalStatAugmentation.FuryPointsMax;

			const float health = CombatStats->GetHealthPercent();

			RemoveExternals.HealthPointsMax = -LegendaryWeapon->ExternalStatAugmentation.HealthPointsMax;
			RemoveExternals.HealthPoints = RemoveExternals.HealthPointsMax * health;

			RemoveExternals.MagicDefense = -LegendaryWeapon->ExternalStatAugmentation.MagicDefense;
			RemoveExternals.MagicPower = -LegendaryWeapon->ExternalStatAugmentation.MagicPower;

			const float mana = CombatStats->GetManaPercent();

			RemoveExternals.ManaPointsMax = -LegendaryWeapon->ExternalStatAugmentation.ManaPointsMax;
			RemoveExternals.ManaPoints = RemoveExternals.ManaPointsMax * mana;

			const float stamina = CombatStats->GetStaminaPercent();

			RemoveExternals.StaminaPointsMax = -LegendaryWeapon->ExternalStatAugmentation.StaminaPointsMax;
			RemoveExternals.StaminaPoints = RemoveExternals.StaminaPointsMax * stamina;

			CombatStats->AugmentExternals(&RemoveExternals);
		}
	}

	if (IsValid(WeaponParticle))
	{
		WeaponParticle->DeactivateSystem();
		WeaponParticle->UnregisterComponent();
		WeaponParticle->DestroyComponent();
	}
}

WeaponQualityTypes UEquippedWeaponObject::GetWeaponQualityType()
{
	if (IsValid(this) && IsValid(MainWeaponObject))
	{
		return MainWeaponObject->GetWeaponQualityType();
	}

	return WeaponQualityTypes::WEAPON_TYPE_NONE;
}

void UEquippedWeaponObject::GetWeaponHitSound(USoundBase *& WeaponSound)
{
	if (IsValid(MainWeaponObject))
	{
		WeaponSound = MainWeaponObject->WeaponHitSound;
	}
}

void UEquippedWeaponObject::GetWeaponHitFX(UParticleSystem *& WeaponHitEffect)
{
	if (IsValid(MainWeaponObject))
	{
		WeaponHitEffect = MainWeaponObject->WeaponHitEffect;
	}
}

void UEquippedWeaponObject::GetCriticalHitSound(USoundBase *& CriticalHitSound)
{
	if (IsValid(MainWeaponObject))
	{
		CriticalHitSound = MainWeaponObject->CriticalHitSound;
	}
}

void UEquippedWeaponObject::GetCriticalHitFX(UParticleSystem *& CriticalHitEffect)
{
	if (IsValid(MainWeaponObject))
	{
		CriticalHitEffect = MainWeaponObject->CriticalHitEffect;
	}
}

float UEquippedWeaponObject::ComputeWeaponDamage(float modifiers, bool IsCriticalHit)
{
	if (IsValid(MainWeaponObject))
	{
		float value = FMath::FRandRange(MainWeaponObject->DamageMRangeMin, MainWeaponObject->DamageMRangeMax);
		
		value *= modifiers;

		if (IsValid(EpicWeapon))
		{
			if (EpicWeapon->bUsesCriticalHitModifier && IsCriticalHit)
			{
				value *= EpicWeapon->CriticalHitModifier;
			}
		}
		
		return value;
	}
	return 0.0f;
}

EquipableTypes UEquippedWeaponObject::GetEquipType()
{
	if (IsValid(MainWeaponObject))
	{
		return MainWeaponObject->EquipType;
	}
	return EquipableTypes();
}

DamageTypes UEquippedWeaponObject::GetDamageType()
{
	if (IsValid(MainWeaponObject))
	{
		return MainWeaponObject->DamageType;
	}
	return DamageTypes();
}

const TSubclassOf<UStatusEffectData> UEquippedWeaponObject::GetStatusEffectClass()
{
	if (IsValid(EpicWeapon))
	{
		return EpicWeapon->StatusEffectToApply;
	}

	if (IsValid(LegendaryWeapon))
	{
		//maybe do the status effect random by weight?
		return LegendaryWeapon->StatusEffectToApply;
	}

	return nullptr;
}

void UEquippedWeaponObject::ConfigureDamageData(FCombatDamageData& DamageData)
{
	DamageData.AttackDamageType = DamageTypes::DamageType_Normal;
	DamageData.AttackElement = ElementalDamageTypes::ElementType_NONE;

	if (IsValid(EpicWeapon))
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

			DamageData.AttackDamageType = EpicWeapon->DamageType;
			DamageData.AttackElement = EpicWeapon->ElementDamageType;

		}
	}
	else if (IsValid(LegendaryWeapon))
	{
		DamageData.bUseCriticalMod = LegendaryWeapon->bUsesCriticalHitModifier;
		DamageData.CriticalModifier = LegendaryWeapon->CriticalHitModifier;

		const TPair<FName, TSubclassOf<UStatusEffectData>>* StatusEffectPair = LegendaryWeapon->GetRandomStatusEffect();

		if (StatusEffectPair)
		{
			TWeakObjectPtr<UStatusEffectData> StatusEffect = Cast<UStatusEffectData>(StatusEffectPair->Value->GetDefaultObject());
			if (StatusEffect.IsValid())
			{
				DamageData.bEffectOnCritical = LegendaryWeapon->bMustBeCriticalToApplyStatus;
				DamageData.bMustDamageForEffect = LegendaryWeapon->bMustDeliverBaseDamageMin;
				DamageData.StatusEffectClass = LegendaryWeapon->StatusEffectToApply;

				DamageData.StatusEffectName = StatusEffectPair->Key;
				DamageData.AttackDamageType = EpicWeapon->DamageType;
				DamageData.AttackElement = LegendaryWeapon->ElementDamageType;
			}
		}
	}
}

void UEquippedWeaponObject::OnBeginWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	WeaponCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OwningBattleCharacter->OnNotifyWeaponHitTarget(OtherActor, OverlappedComponent->GetComponentLocation(), bIsLeftHandWeapon);
}

void UEquippedWeaponObject::SetCollisionEnabled(ECollisionEnabled::Type Enabled)
{
	if (IsValid(WeaponCollisionComp))
	{
		WeaponCollisionComp->SetCollisionEnabled(Enabled);
	}
}

void UEquippedWeaponObject::SetEnableHighlight(bool enable)
{
	if (IsValid(MainWeaponObject) && IsValid(OwningMeshComp))
	{
		OwningMeshComp->SetRenderCustomDepth(enable);
	}
}

void UEquippedWeaponObject::Initialize(const UEquippedWeaponObject& NewWeapon)
{
	if (IsValid(NewWeapon.LegendaryWeapon))
	{
		MainWeaponObject = NewObject<ULegendaryWeaponObject>(this, ULegendaryWeaponObject::StaticClass());
		LegendaryWeapon = Cast<ULegendaryWeaponObject>(MainWeaponObject);

		LegendaryWeapon->CloneThis(NewWeapon.LegendaryWeapon);

	}
	else if (IsValid(NewWeapon.EpicWeapon))
	{
		MainWeaponObject = NewObject<UEpicWeaponObject>(this, UEpicWeaponObject::StaticClass());
		EpicWeapon = Cast<UEpicWeaponObject>(MainWeaponObject);
		
		EpicWeapon->CloneThis(NewWeapon.EpicWeapon);

	}
	else
	{
		MainWeaponObject = NewObject<UWeaponObject>(this, UWeaponObject::StaticClass());
		MainWeaponObject->CloneThis(NewWeapon.MainWeaponObject);
	}
	if (IsValid(NewWeapon.WeaponParticle))
	{
		WeaponParticle = &(*NewWeapon.WeaponParticle);
	}
}

UEquippedWeaponObject& UEquippedWeaponObject::operator=(const UEquippedWeaponObject* NewWeapon)
{
	Initialize(*NewWeapon);

	return *this;
}
