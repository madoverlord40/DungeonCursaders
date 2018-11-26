// Fill out your copyright notice in the Description page of Project Settings.

#include "EquippedArmorObject.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

UEquippedArmorObject::UEquippedArmorObject() : Super()
{
	MainArmorObject = nullptr;
	EpicArmor = nullptr;	
	LegendaryArmor = nullptr;
	bHasBeenConfigured = false; 
	bWasMeshCreatedLocal = false;
}

UEquippedArmorObject::UEquippedArmorObject(const UEquippedArmorObject& NewArmor) : Super()
{
	MainArmorObject = nullptr;
	EpicArmor = nullptr;
	LegendaryArmor = nullptr;
	bHasBeenConfigured = false;
	bWasMeshCreatedLocal = false;

	Initialize(NewArmor);
}

UEquippedArmorObject& UEquippedArmorObject::operator=(const UEquippedArmorObject& NewArmor)
{
	if (bHasBeenConfigured)
	{
		MainArmorObject->ConditionalBeginDestroy();
	}

	Initialize(NewArmor);

	return *this;
}

void UEquippedArmorObject::BeginDestroy()
{
	if (IsValid(MainArmorObject))
	{
		MainArmorObject->ConditionalBeginDestroy();
		bHasBeenConfigured = false;
	}

	Super::BeginDestroy();
}

void UEquippedArmorObject::CreateThis(const FName & Name, const TSubclassOf<UArmorObject>& ArmorClass)
{
	if (IsValid(MainArmorObject))
	{
		MainArmorObject->ConditionalBeginDestroy();
	}

	MainArmorObject = NewObject<UArmorObject>(this, ArmorClass, Name);

	EpicArmor = Cast<UEpicArmorObject>(MainArmorObject);
	LegendaryArmor = Cast<ULegendaryArmorObject>(MainArmorObject);

	Description = MainArmorObject->GetDescriptionString();
}

void UEquippedArmorObject::EquipThis(UStaticMeshComponent* ArmorMeshComp, const FName& SocketName, USkeletalMeshComponent* AttachTo)
{
	if (IsValid(MainArmorObject) && IsValid(AttachTo) && IsValid(ArmorMeshComp))
	{
		MasterPoseComponent = &(*AttachTo);
		StaticMeshComponent = &(*ArmorMeshComp);

		StaticMeshComponent->SetStaticMesh(MainArmorObject->StaticMeshBase);
		StaticMeshComponent->AttachToComponent(MasterPoseComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}
}

void UEquippedArmorObject::EquipThis(USkeletalMeshComponent*& MasterPoseComp, USkeletalMeshComponent*& SkinnedSlotMesh)
{
	if (IsValid(MainArmorObject) && IsValid(MasterPoseComp) && IsValid(SkinnedSlotMesh))
	{
		SkinnedSlotMesh->SetSkeletalMesh(MainArmorObject->SkinedMesh);
		SkinnedSlotMesh->SetMasterPoseComponent(MasterPoseComp);
		SkinnedSlotMesh->SetCanEverAffectNavigation(false);

		MasterPoseComponent = &(*MasterPoseComp);
		SkinnedMeshComponent = &(*SkinnedSlotMesh);
	}

}

void UEquippedArmorObject::EquipThis(USkeletalMeshComponent*& MasterPoseComp)
{
	if (IsValid(MainArmorObject) && IsValid(MasterPoseComp))
	{
		MasterPoseComponent = &(*MasterPoseComp);

		SkinnedMeshComponent = NewObject<USkeletalMeshComponent>(this);
		SkinnedMeshComponent->RegisterComponent();
		SkinnedMeshComponent->AttachToComponent(MasterPoseComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		SkinnedMeshComponent->SetMasterPoseComponent(MasterPoseComponent);
		SkinnedMeshComponent->SetSkeletalMesh(MainArmorObject->SkinedMesh);

		bWasMeshCreatedLocal = true;
	}
}

void UEquippedArmorObject::ConfigureThis(UCombatData *& CombatStats)
{
	DamageReflection.bAttackerTakesDamage = false;
	DamageReflection.ReflectDamagePercent = 0.0f;
	DamageReflection.ReflectDamageType = DamageTypes::DamageType_NONE;
	DamageReflection.ReflectElementalType = ElementalDamageTypes::ElementType_NONE;

	if (IsValid(EpicArmor))
	{
		CombatStats->AugmentInternals(&EpicArmor->InternalAugmentation);
	}

	if (IsValid(LegendaryArmor))
	{
		CombatStats->AugmentExternals(&LegendaryArmor->ExternalAugmentation);

		if (LegendaryArmor->bAttackerTakesDamage)
		{
			DamageReflection.bAttackerTakesDamage = true;
			DamageReflection.ReflectDamagePercent = LegendaryArmor->ReflectDamagePercent;
			DamageReflection.ReflectDamageType = LegendaryArmor->ReflectDamageType;
			DamageReflection.ReflectElementalType = LegendaryArmor->ReflectElementalType;
		}
	}

	FExternalStats AddExternals;
	AddExternals.MagicDefense = MainArmorObject->MagicDefense;
	AddExternals.DefensePower = MainArmorObject->DefensePower;
	CombatStats->AugmentExternals(&AddExternals);

	bHasBeenConfigured = true;
}

void UEquippedArmorObject::UnEquipThis(UCombatData *& CombatStats)
{
	DamageReflection.bAttackerTakesDamage = false;
	DamageReflection.ReflectDamagePercent = 0.0f;
	DamageReflection.ReflectDamageType = DamageTypes::DamageType_NONE;
	DamageReflection.ReflectElementalType = ElementalDamageTypes::ElementType_NONE;

	if (IsValid(StaticMeshComponent))
	{
		StaticMeshComponent->SetStaticMesh(nullptr);
		StaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		StaticMeshComponent = nullptr;
		MasterPoseComponent = nullptr;
	}
	else if (IsValid(SkinnedMeshComponent) && IsValid(MasterPoseComponent))
	{
		if (bWasMeshCreatedLocal)
		{
			SkinnedMeshComponent->UnregisterComponent();
			SkinnedMeshComponent->DestroyComponent();
			SkinnedMeshComponent = nullptr;
			MasterPoseComponent = nullptr;
			bWasMeshCreatedLocal = false;
		}
		else
		{
			SkinnedMeshComponent->SetSkeletalMesh(nullptr);
			SkinnedMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
			SkinnedMeshComponent = nullptr;
			MasterPoseComponent = nullptr;
		}
	}

	{
		FExternalStats RemoveExternals;
		RemoveExternals.MagicDefense = -MainArmorObject->MagicDefense;
		RemoveExternals.DefensePower = -MainArmorObject->DefensePower;
		CombatStats->AugmentExternals(&RemoveExternals);
	}

	if (IsValid(EpicArmor))
	{
		if (bHasBeenConfigured)
		{
			FInternalStats RemoveStats;

			RemoveStats.Agility = -EpicArmor->InternalAugmentation.Agility;
			RemoveStats.Dexterity = -EpicArmor->InternalAugmentation.Dexterity;
			RemoveStats.Intellect = -EpicArmor->InternalAugmentation.Intellect;
			RemoveStats.Mind = -EpicArmor->InternalAugmentation.Mind;
			RemoveStats.Strength = -EpicArmor->InternalAugmentation.Strength;
			RemoveStats.Vitality = -EpicArmor->InternalAugmentation.Vitality;

			CombatStats->AugmentInternals(&RemoveStats);
		}
	}

	if (IsValid(LegendaryArmor))
	{
		if (bHasBeenConfigured)
		{
			FInternalStats RemoveStats;

			RemoveStats.Agility = -LegendaryArmor->InternalAugmentation.Agility;
			RemoveStats.Dexterity = -LegendaryArmor->InternalAugmentation.Dexterity;
			RemoveStats.Intellect = -LegendaryArmor->InternalAugmentation.Intellect;
			RemoveStats.Mind = -LegendaryArmor->InternalAugmentation.Mind;
			RemoveStats.Strength = -LegendaryArmor->InternalAugmentation.Strength;
			RemoveStats.Vitality = -LegendaryArmor->InternalAugmentation.Vitality;

			CombatStats->AugmentInternals(&RemoveStats);

			FExternalStats RemoveExternals;
			RemoveExternals.AttackAcc = -LegendaryArmor->ExternalAugmentation.AttackAcc;
			RemoveExternals.AttackPower = -LegendaryArmor->ExternalAugmentation.AttackPower;
			RemoveExternals.DefensePower = -LegendaryArmor->ExternalAugmentation.DefensePower;
			RemoveExternals.Evasion = -LegendaryArmor->ExternalAugmentation.Evasion;
			RemoveExternals.FuryPoints = -LegendaryArmor->ExternalAugmentation.FuryPoints;
			RemoveExternals.FuryPointsMax = -LegendaryArmor->ExternalAugmentation.FuryPointsMax;

			const float health = CombatStats->GetHealthPercent();

			RemoveExternals.HealthPointsMax = -LegendaryArmor->ExternalAugmentation.HealthPointsMax;
			RemoveExternals.HealthPoints = RemoveExternals.HealthPointsMax * health;

			RemoveExternals.MagicDefense = -LegendaryArmor->ExternalAugmentation.MagicDefense;
			RemoveExternals.MagicPower = -LegendaryArmor->ExternalAugmentation.MagicPower;

			const float mana = CombatStats->GetManaPercent();

			RemoveExternals.ManaPointsMax = -LegendaryArmor->ExternalAugmentation.ManaPointsMax;
			RemoveExternals.ManaPoints = RemoveExternals.ManaPointsMax * mana;

			const float stamina = CombatStats->GetStaminaPercent();

			RemoveExternals.StaminaPointsMax = -LegendaryArmor->ExternalAugmentation.StaminaPointsMax;
			RemoveExternals.StaminaPoints = RemoveExternals.StaminaPointsMax * stamina;

			CombatStats->AugmentExternals(&RemoveExternals);

			DamageReflection.bAttackerTakesDamage = false;
			DamageReflection.ReflectDamagePercent = 0.0f;
			DamageReflection.ReflectDamageType = DamageTypes::DamageType_NONE;
			DamageReflection.ReflectElementalType = ElementalDamageTypes::ElementType_NONE;

			
		}
	}
}


const TArray<FElementalResistData>* UEquippedArmorObject::GetElementalResistances() const
{
	if (bHasBeenConfigured)
	{
		if (IsValid(EpicArmor))
		{
			return &EpicArmor->ElementalResistances;
		}

		if (IsValid(LegendaryArmor))
		{
			return &LegendaryArmor->ElementalResistances;
		}
	}

	return nullptr;
}

const TArray<FDamageResistData>* UEquippedArmorObject::GetDamageResistances() const
{
	if (bHasBeenConfigured)
	{
		if (IsValid(EpicArmor))
		{
			return &EpicArmor->DamageResistances;
		}

		if (IsValid(LegendaryArmor))
		{
			return &LegendaryArmor->DamageResistances;
		}
	}

	return nullptr;
}

float UEquippedArmorObject::GetDefensePower() const
{
	if (bHasBeenConfigured)
	{
		return MainArmorObject->DefensePower;
	}

	return 0.0f;
}

float UEquippedArmorObject::GetMagicDefense() const
{
	if (bHasBeenConfigured)
	{
		return MainArmorObject->MagicDefense;
	}
	return 0.0f;
}

const USkeletalMesh* UEquippedArmorObject::GetSkinnedMesh() const
{
	if (bHasBeenConfigured)
	{
		return MainArmorObject->SkinedMesh;
	}
	return nullptr;
}

ArmorSlotTypes UEquippedArmorObject::GetArmorSlot() const
{
	if (bHasBeenConfigured)
	{
		return MainArmorObject->ArmorSlot;
	}
	return ArmorSlotTypes();
}

const TArray<StatusEffectTypes>* UEquippedArmorObject::GetStatusImmunities() const
{
	if (IsValid(LegendaryArmor))
	{
		return &LegendaryArmor->StatusEffectImmunities;
	}

	return nullptr;
}

void UEquippedArmorObject::SetEnableHighlightMesh(bool enable)
{
	if (IsValid(SkinnedMeshComponent))
	{
		SkinnedMeshComponent->SetRenderCustomDepth(enable);
	}
}

ArmorQualityTypes UEquippedArmorObject::GetArmorQualityType() const 
{
	if (bHasBeenConfigured)
	{
		return MainArmorObject->GetQualityType();
	}
	return ArmorQualityTypes();
}

void UEquippedArmorObject::Initialize(const UEquippedArmorObject& NewArmor)
{
	if (IsValid(NewArmor.LegendaryArmor))
	{
		MainArmorObject = NewObject<ULegendaryArmorObject>(NewArmor.MainArmorObject, ULegendaryArmorObject::StaticClass());
		LegendaryArmor = Cast<ULegendaryArmorObject>(MainArmorObject);

		LegendaryArmor->CloneThis(NewArmor.LegendaryArmor);
	}
	else if (IsValid(NewArmor.EpicArmor))
	{
		MainArmorObject = NewObject<UEpicArmorObject>(NewArmor.MainArmorObject, UEpicArmorObject::StaticClass());
		EpicArmor = Cast<UEpicArmorObject>(MainArmorObject);
		EpicArmor->CloneThis(NewArmor.EpicArmor);
	}
	else
	{
		MainArmorObject = NewObject<UArmorObject>(NewArmor.MainArmorObject, UArmorObject::StaticClass());
		MainArmorObject->CloneThis(NewArmor.MainArmorObject);
	}
}
