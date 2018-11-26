// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatData.h"

void UCombatData::InitializeData(const FName & Name, const uint8 StartingLevel)
{
	CharacterName = Name;
	Level = StartingLevel;

	ConfigureClass(StartingLevel);

	ConfigureBattleModifiers();

	NextLevelUp = (((200.0f + (100 * StartingLevel)) / 2.0f) * StartingLevel) ;
}

void UCombatData::ModifyExp(float EXP)
{
	ExperiencePoints += EXP;
}

bool UCombatData::CheckForLevelUp()
{
	bool leveled = false;

	if (ExperiencePoints >= NextLevelUp) {
		ExperiencePoints -= NextLevelUp;
		Level++;
		InitializeData(CharacterName, Level);
		leveled = true;
	}
	return leveled;
}

void UCombatData::AugmentInternals(const FInternalStats * const augmented)
{
	Internals.Agility	+= augmented->Agility;
	Internals.Dexterity += augmented->Dexterity;
	Internals.Intellect += augmented->Intellect;
	Internals.Mind		+= augmented->Mind;
	Internals.Strength	+= augmented->Strength;
	Internals.Vitality	+= augmented->Vitality;

	ConfigureBattleModifiers();
}

void UCombatData::AugmentExternals(const FExternalStats * const augmented)
{
	Externals.AttackAcc			+= augmented->AttackAcc;
	Externals.AttackPower		+= augmented->AttackPower;
	Externals.DefensePower		+= augmented->DefensePower;
	Externals.Evasion			+= augmented->Evasion;
	Externals.FuryPoints		+= augmented->FuryPoints;
	Externals.FuryPointsMax		+= augmented->FuryPointsMax;
	Externals.HealthPointsMax	+= augmented->HealthPointsMax;
	Externals.HealthPoints		+= augmented->HealthPoints;
	Externals.MagicDefense		+= augmented->MagicDefense;
	Externals.MagicPower		+= augmented->MagicPower;
	Externals.ManaPoints		+= augmented->ManaPoints;
	Externals.ManaPointsMax		+= augmented->ManaPointsMax;
	Externals.StaminaPoints		+= augmented->StaminaPoints;
	Externals.StaminaPointsMax  += augmented->StaminaPointsMax;

}

void UCombatData::AugmentDamageResistance(const FDamageResistData* const augmented, bool remove)
{
	bool updated = false;

	for (int index = 0; index < DamageResistances.Num(); index++)
	{
		FDamageResistData& DamageResist = DamageResistances[index];

		if (DamageResist.ResistDamageType == augmented->ResistDamageType)
		{
			if (remove)
			{
				DamageResistances.RemoveAt(index);
				updated = true;
				break;
			}
			else
			{
				DamageResist.Modifier = augmented->Modifier;
				updated = true;
				break;
			}
		}
	}

	if (!updated)
	{
		DamageResistances.Add(*augmented);
	}
}

void UCombatData::AugmentElementalResistance(const FElementalResistData* const augmented, bool remove)
{
	bool updated = false;

	for (int index = 0; index < ElementDamageResistances.Num(); index++)
	{
		FElementalResistData& ElementalResist = ElementDamageResistances[index];

		if (ElementalResist.ResistDamageType == augmented->ResistDamageType)
		{
			if (remove)
			{
				ElementDamageResistances.RemoveAt(index);
				updated = true;
				break;
			}
			else
			{
				ElementalResist.Modifier = augmented->Modifier;
				updated = true;
				break;
			}
		}
	}

	if (!updated)
	{
		ElementDamageResistances.Add(*augmented);
	}
}


void UCombatData::SetInternals(const FInternalStats * const augmented)
{
	Internals.Agility = augmented->Agility;
	Internals.Dexterity = augmented->Dexterity;
	Internals.Intellect = augmented->Intellect;
	Internals.Mind = augmented->Mind;
	Internals.Strength = augmented->Strength;
	Internals.Vitality = augmented->Vitality;

	ConfigureBattleModifiers();
}

void UCombatData::SetExternals(const FExternalStats * const augmented)
{
	Externals.AttackAcc = augmented->AttackAcc;
	Externals.AttackPower = augmented->AttackPower;
	Externals.DefensePower = augmented->DefensePower;
	Externals.Evasion = augmented->Evasion;
	Externals.FuryPoints = augmented->FuryPoints;
	Externals.FuryPointsMax = augmented->FuryPointsMax;
	Externals.HealthPointsMax = augmented->HealthPointsMax;
	Externals.HealthPoints = augmented->HealthPoints;
	Externals.MagicDefense = augmented->MagicDefense;
	Externals.MagicPower = augmented->MagicPower;
	Externals.ManaPoints = augmented->ManaPoints;
	Externals.ManaPointsMax = augmented->ManaPointsMax;
	Externals.StaminaPoints = augmented->StaminaPoints;
	Externals.StaminaPointsMax = augmented->StaminaPointsMax;
}


void UCombatData::ModifyAttackPower(float value)
{
	Externals.AttackPower += value;
}

void UCombatData::ModifyDefensePower(float value)
{
	Externals.DefensePower += value;
}

void UCombatData::ModifyMagicPower(float value)
{
	Externals.MagicPower += value;
}

void UCombatData::ModifyMagicDefensePower(float value)
{
	Externals.MagicDefense += value;
}

void UCombatData::ModifyAcc(float value)
{
	Externals.AttackAcc += value;
}

void UCombatData::ModifyEvasion(float value)
{
	Externals.Evasion += value;
}

bool UCombatData::ModifyHitPoints(const float value)
{
	Externals.HealthPoints += value;

	return (Externals.HealthPoints > 0.1f ? true : false);
}

void UCombatData::ModifyStaminaPoints(const float value)
{
    Externals.StaminaPoints += value;
	Externals.StaminaPoints = Externals.StaminaPoints < 0.0f ? 0.0f : Externals.StaminaPoints;
}

void UCombatData::ModifyManaPoints(const float value)
{
    Externals.ManaPoints += value;
	Externals.ManaPoints = Externals.ManaPoints < 0.0f ? 0.0f : Externals.ManaPoints;
}

void UCombatData::ModifyRagePoints(const float value)
{
    Externals.FuryPoints += value;
	Externals.FuryPoints = Externals.FuryPoints < 0.0f ? 0.0f : Externals.FuryPoints;
}

float UCombatData::GetHealthPercent() const
{
	float percent = Externals.HealthPoints / Externals.HealthPointsMax;
	return percent;
}

float UCombatData::GetManaPercent() const
{
	float percent = Externals.ManaPoints / Externals.ManaPointsMax;
	return percent;
}

float UCombatData::GetStaminaPercent() const
{
	float percent = Externals.StaminaPoints / Externals.StaminaPointsMax;
	return percent;
}

float UCombatData::GetRagePercent() const
{
	float percent = Externals.FuryPoints / Externals.FuryPointsMax;
	return percent;
}

float UCombatData::GetExpPercent() const
{
	float percent = ExperiencePoints / NextLevelUp;
	return percent;
}


void UCombatData::ConfigureClass(uint8 level)
{
	float agility = 0.0f;
	float dexterity = 0.0f;
	float intellect = 0.0f;
	float mind = 0.0f;
	float strength = 0.0f;
	float vitality = 0.0f;

	float agility_growth = 0.0f;
	float dexterity_growth = 0.0f;
	float intellect_growth = 0.0f;
	float mind_growth = 0.0f;
	float strength_growth = 0.0f;
	float vitality_growth = 0.0f;

	switch (ClassConfigType) {

	case ClassConfigTypes::Config_HeavyMelee:
		agility = 15.0f;
		agility_growth = 3.0f;

		dexterity = 16.0f;
		dexterity_growth = 3.0f;

		intellect = 12.0f;
		intellect_growth = 2.0f;

		mind = 10.0f;
		mind_growth = 2.5f;

		strength = 18.0f;
		strength_growth = 3.5f;

		vitality = 23.0f;
		vitality_growth = 5.0f;

		ClassTypeName = FName("Knight");
		break;

	case ClassConfigTypes::Config_Magic:
		agility = 12.0f;
		agility_growth = 2.0f;

		dexterity = 14.0f;
		dexterity_growth = 2.0f;

		intellect = 23.0f;
		intellect_growth = 5.0f;

		mind = 21.0f;
		mind_growth = 4.5f;

		strength = 12.0f;
		strength_growth = 2.5f;

		vitality = 14.0f;
		vitality_growth = 2.0f;

		ClassTypeName = FName("Sorceror");
		break;

	case ClassConfigTypes::Config_MediumMelee:
		agility = 16.0f;
		agility_growth = 3.0f;

		dexterity = 15.0f;
		dexterity_growth = 3.0f;

		intellect = 15.0f;
		intellect_growth = 2.0f;

		mind = 12.0f;
		mind_growth = 2.5f;

		strength = 23.0f;
		strength_growth = 5.0f;

		vitality = 18.0f;
		vitality_growth = 4.0f;

		ClassTypeName = FName("Warrior");
		break;

	case ClassConfigTypes::Config_MeleeMagic:
		agility = 14.0f;
		agility_growth = 2.0f;

		dexterity = 13.0f;
		dexterity_growth = 2.5f;

		intellect = 18.0f;
		intellect_growth = 3.0f;

		mind = 16.0f;
		mind_growth = 3.5f;

		strength = 12.0f;
		strength_growth = 3.5f;

		vitality = 15.0f;
		vitality_growth = 3.0f;

		ClassTypeName = FName("Warlock");
		break;

	case ClassConfigTypes::Config_MeleeRanged:
		agility = 17.0f;
		agility_growth = 3.0f;

		dexterity = 21.0f;
		dexterity_growth = 4.0f;

		intellect = 12.0f;
		intellect_growth = 2.0f;

		mind = 13.0f;
		mind_growth = 2.0f;

		strength = 16.0f;
		strength_growth = 3.0f;

		vitality = 17.0f;
		vitality_growth = 3.5f;

		ClassTypeName = FName("Ranger");
		break;

	case ClassConfigTypes::Config_MeleeRangedMagic:
		agility = 16.0f;
		agility_growth = 3.0f;

		dexterity = 16.0f;
		dexterity_growth = 3.0f;

		intellect = 15.0f;
		intellect_growth = 3.0f;

		mind = 16.0f;
		mind_growth = 3.0f;

		strength = 14.0f;
		strength_growth = 3.0f;

		vitality = 15.0f;
		vitality_growth = 3.0f;

		ClassTypeName = FName("Mercenary");
		break;

	case ClassConfigTypes::Config_SmallMelee:
		agility = 23.0f;
		agility_growth = 5.0f;

		dexterity = 21.0f;
		dexterity_growth = 4.0f;

		intellect = 12.0f;
		intellect_growth = 2.0f;

		mind = 14.0f;
		mind_growth = 2.0f;

		strength = 16.0f;
		strength_growth = 3.0f;

		vitality = 15.0f;
		vitality_growth = 2.5f;

		ClassTypeName = FName("Rogue");
		break;

	default:
		break;
	}

	Internals.Agility =   (agility + (agility_growth * level));
	Internals.Dexterity = (dexterity + (dexterity_growth * level));
    Internals.Intellect = (intellect + (intellect_growth * level));
	Internals.Mind =      (mind + (mind_growth * level));
	Internals.Strength =  (strength + (strength_growth * level));
	Internals.Vitality =  (vitality + (vitality_growth * level));
}

void UCombatData::ConfigureBattleModifiers()
{
	Externals.AttackPower = (Internals.Strength * 1.75f);
	Externals.DefensePower = (Internals.Vitality * 5.5f);
	Externals.Evasion = ((Internals.Agility * 2.75f) + (Internals.Dexterity * 2.25f));
	Externals.AttackAcc = ((Internals.Strength * 1.5f) + (Internals.Dexterity * 1.75f));
	Externals.FuryPoints = 0;
	Externals.FuryPointsMax = 1000;
	Externals.HealthPointsMax = (Internals.Vitality * 15.0f);
	Externals.HealthPoints = Externals.HealthPointsMax;
	Externals.MagicDefense = (Internals.Mind * 7.75f);
	Externals.MagicPower = (Internals.Intellect * 5.55f);
	Externals.ManaPoints = ((Internals.Intellect * 4.5f) + (Internals.Mind * 6.75f));
	Externals.ManaPointsMax = Externals.ManaPoints;
    Externals.StaminaPointsMax = ((Internals.Strength * 6.5f) + (Internals.Dexterity * 3.0f) + (Internals.Vitality * 10.0f));
    Externals.StaminaPoints = Externals.StaminaPointsMax;

	switch (BattleConfigType) {
	case BattleConfigTypes::Config_Boss:
		Externals.AttackPower *= 1.35f;
		Externals.DefensePower *= 1.20f;
		Externals.Evasion *= 1.15f;
		Externals.AttackAcc *= 1.25f;
		Externals.HealthPointsMax *= 4.0f;
		Externals.HealthPoints = Externals.HealthPointsMax;
		Externals.MagicDefense *= 1.20f;
		Externals.MagicPower *= 1.25f;
		Externals.ManaPoints *= 1.50f;
		Externals.ManaPointsMax = Externals.ManaPoints;
		break;

	case BattleConfigTypes::Config_Elite:
		Externals.AttackPower *= 1.15f;
		Externals.DefensePower *= 1.10f;
		Externals.Evasion *= 1.15f;
		Externals.AttackAcc *= 1.15f;
		Externals.HealthPointsMax *= 3.0f;
		Externals.HealthPoints = Externals.HealthPointsMax;
		Externals.MagicDefense *= 1.10f;
		Externals.MagicPower *= 1.15f;
		Externals.ManaPoints *= 1.15f;
		Externals.ManaPointsMax = Externals.ManaPoints;
		break;

	case BattleConfigTypes::Config_Hard:
		Externals.AttackPower *= 1.10f;
		Externals.DefensePower *= 1.10f;
		Externals.Evasion *= 1.10f;
		Externals.AttackAcc *= 1.10f;
		Externals.HealthPointsMax *= 2.0f;
		Externals.HealthPoints = Externals.HealthPointsMax;
		Externals.MagicDefense *= 1.10f;
		Externals.MagicPower *= 1.10f;
		Externals.ManaPoints *= 1.10f;
		Externals.ManaPointsMax = Externals.ManaPoints;
		break;

	case BattleConfigTypes::Config_Normal:
		Externals.AttackPower *= 1.0f;
		Externals.DefensePower *= 1.0f;
		Externals.Evasion *= 1.0f;
		Externals.AttackAcc *= 1.0f;
		Externals.HealthPointsMax *= 1.0f;
		Externals.HealthPoints = Externals.HealthPointsMax;
		Externals.MagicDefense *= 1.0f;
		Externals.MagicPower *= 1.0f;
		Externals.ManaPoints *= 1.0f;
		Externals.ManaPointsMax = Externals.ManaPoints;
		break;

	case BattleConfigTypes::Config_Easy:
		Externals.AttackPower *= 0.75f;
		Externals.DefensePower *= 0.75f;
		Externals.Evasion *= 0.75f;
		Externals.AttackAcc *= 0.75f;
		Externals.HealthPointsMax *= 0.75f;
		Externals.HealthPoints = Externals.HealthPointsMax;
		Externals.MagicDefense *= 1.0f;
		Externals.MagicPower *= 1.0f;
		Externals.ManaPoints *= 1.0f;
		Externals.ManaPointsMax = Externals.ManaPoints;
		break;

	case BattleConfigTypes::Config_Weak:
		Externals.AttackPower *= 0.5f;
		Externals.DefensePower *= 0.5f;
		Externals.Evasion *= 0.5f;
		Externals.AttackAcc *= 0.5f;
		Externals.HealthPointsMax *= 0.5f;
		Externals.HealthPoints = Externals.HealthPointsMax;
		Externals.MagicDefense *= 0.5f;
		Externals.MagicPower *= 1.0f;
		Externals.ManaPoints *= 1.0f;
		Externals.ManaPointsMax = Externals.ManaPoints;
		break;

	case BattleConfigTypes::Config_PlayerCharacter:
		Externals.AttackPower *= 1.0f;
		Externals.DefensePower *= 1.0f;
		Externals.Evasion *= 1.0f;
		Externals.AttackAcc *= 1.0f;
		Externals.HealthPointsMax *= 3.0f;
		Externals.HealthPoints = Externals.HealthPointsMax;
		Externals.MagicDefense *= 1.0f;
		Externals.MagicPower *= 1.0f;
		Externals.ManaPoints *= 2.0f;
		Externals.ManaPointsMax = Externals.ManaPoints;
		break;
	}
	
}
