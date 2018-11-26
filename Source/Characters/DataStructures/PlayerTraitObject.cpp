// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerTraitObject.h"

UPlayerTraitObject::UPlayerTraitObject()
{
	Modifier = 0.0f;
	UpgradeModifierPerPoint = 0.1f;
}
UPlayerTraitObject::UPlayerTraitObject(const UPlayerTraitObject& NewTrait)
{
	CopyThis(&NewTrait);
}

void UPlayerTraitObject::UpgradeTrait(const uint8 Points)
{
	int upgrade = UpgradePoints + Points;
	if (upgrade < UpgradePointsMax && upgrade < 255)
	{
		UpgradePoints += Points;

		switch (TraitType)
		{
		case EPlayerTraitTypes::PlayerTrait_Damage:
			Modifier += (UpgradeModifierPerPoint * Points);
			break;
		case EPlayerTraitTypes::PlayerTrait_Defense:
			Modifier += (UpgradeModifierPerPoint * Points);
			break;
		case EPlayerTraitTypes::PlayerTrait_InternalStat:
			UpgradeInternals(Points);
			break;
		case EPlayerTraitTypes::PlayerTrait_ExternalStat:
			UpgradeExternals(Points);
			break;
		case EPlayerTraitTypes::PlayerTrait_Resistence:
			if (DamageResistance.Modifier > 0.0f)
			{
				DamageResistance.Modifier += UpgradeModifierPerPoint * Points;
			}

			if (ElementResistance.Modifier > 0.0f)
			{
				ElementResistance.Modifier += UpgradeModifierPerPoint * Points;
			}
			break;
		case EPlayerTraitTypes::PlayerTrait_Recovery:
			Modifier += (UpgradeModifierPerPoint * Points);
			break;
		case EPlayerTraitTypes::PlayerTrait_Stealth:
			Modifier += (UpgradeModifierPerPoint * Points);
			break;
		default:
			break;
		}
	}
}

UPlayerTraitObject& UPlayerTraitObject::operator=(const UPlayerTraitObject& NewTrait)
{
	CopyThis(&NewTrait);

	return *this;
}

UPlayerTraitObject& UPlayerTraitObject::operator=(const UPlayerTraitObject* NewTrait)
{
	CopyThis(NewTrait);

	return *this;
}


void UPlayerTraitObject::CopyThis(const UPlayerTraitObject* CopyTrait)
{
	TraitName = CopyTrait->TraitName;

	TraitDescription = CopyTrait->TraitDescription;

	TraitIcon = CopyTrait->TraitIcon;

	TraitType = CopyTrait->TraitType;

	Externals = CopyTrait->Externals;

	Internals = CopyTrait->Internals;

	ElementResistance = CopyTrait->ElementResistance;

	DamageResistance = CopyTrait->DamageResistance;

	Modifier = CopyTrait->Modifier;

	UpgradePoints = CopyTrait->UpgradePoints;
	UpgradePointsMax = CopyTrait->UpgradePointsMax;
	UpgradeModifierPerPoint = CopyTrait->UpgradeModifierPerPoint;

	bHasTraitRequirement = CopyTrait->bHasTraitRequirement;
	TraitRequirementKey = CopyTrait->TraitRequirementKey;
	TraitPointRequirement = CopyTrait->TraitPointRequirement;
}

void UPlayerTraitObject::UpgradeInternals(const uint8 points)
{
	if (Internals.Agility > 0.0f)
	{
		Internals.Agility += UpgradeModifierPerPoint * points;
	}
	if (Internals.Dexterity > 0.0f)
	{
		Internals.Dexterity += UpgradeModifierPerPoint * points;
	}
	if (Internals.Intellect > 0.0f)
	{
		Internals.Intellect += UpgradeModifierPerPoint * points;
	}
	if (Internals.Mind > 0.0f)
	{
		Internals.Mind += UpgradeModifierPerPoint * points;
	}
	if (Internals.Strength > 0.0f)
	{
		Internals.Strength += UpgradeModifierPerPoint * points;
	}
	if (Internals.Vitality > 0.0f)
	{
		Internals.Vitality += UpgradeModifierPerPoint * points;
	}
}

void UPlayerTraitObject::UpgradeExternals(const uint8 points)
{
	if (Externals.AttackAcc > 0.0f)
	{
		Externals.AttackAcc += UpgradeModifierPerPoint * points;
	}
	if (Externals.AttackPower > 0.0f)
	{
		Externals.AttackPower += UpgradeModifierPerPoint * points;
	}
	if (Externals.DefensePower > 0.0f)
	{
		Externals.DefensePower += UpgradeModifierPerPoint * points;
	}
	if (Externals.Evasion > 0.0f)
	{
		Externals.Evasion += UpgradeModifierPerPoint * points;
	}
	if (Externals.HealthPointsMax > 0.0f)
	{
		Externals.HealthPointsMax += UpgradeModifierPerPoint * points;
	}
	if (Externals.MagicDefense > 0.0f)
	{
		Externals.MagicDefense += UpgradeModifierPerPoint * points;
	}
	if (Externals.MagicPower > 0.0f)
	{
		Externals.MagicPower += UpgradeModifierPerPoint * points;
	}
	if (Externals.ManaPointsMax > 0.0f)
	{
		Externals.ManaPointsMax += UpgradeModifierPerPoint * points;
	}
	if (Externals.StaminaPointsMax > 0.0f)
	{
		Externals.StaminaPointsMax += UpgradeModifierPerPoint * points;
	}
}
