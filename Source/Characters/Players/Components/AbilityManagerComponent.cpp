// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityManagerComponent.h"
#include "Characters/Abilities/RageAttackAbility.h"
#include "Characters/DataStructures/CombatData.h"


// Sets default values for this component's properties
UAbilityManagerComponent::UAbilityManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bWantsInitializeComponent = true;
}


// Called when the game starts
void UAbilityManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UAbilityManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
	//TODO: create abilities here from classes

	for (const TPair<FName, TSubclassOf<UAbilityBase>>& Pair : AbilityClasses)
	{
		AddNewAbility(Pair.Key, Pair.Value);
	}
}

// Called every frame
void UAbilityManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	for (TPair<FName, FAbilityInfo>& Pair : EquippedAbilityList)
	{
		if (Pair.Value.bIsEnabled == false && Pair.Value.CurrentTime < Pair.Value.CooldownTime)
		{
			Pair.Value.CurrentTime += DeltaTime;
		}
		else
		{
			Pair.Value.CurrentTime = 0.0f;
			Pair.Value.bIsEnabled = true;
			
		}
	}
}

bool UAbilityManagerComponent::AddNewAbility(const FName& KeyName, TSubclassOf<UAbilityBase> Ability)
{
	UAbilityBase* NewAbility = NewObject<UAbilityBase>(this, Ability);

	if (IsValid(NewAbility))
	{
		GenericAbilities.Add(KeyName, NewAbility);

		UAttackAbility* Attack = Cast<UAttackAbility>(NewAbility);

		URageAttackAbility* Rage = Cast<URageAttackAbility>(NewAbility);

		FAbilityInfo Info;

		Info.AbilityName = NewAbility->GetAbilityName();
		Info.bIsEnabled = true;
		Info.CooldownTime = NewAbility->GetCooldownTime();
		Info.IconTexture = NewAbility->GetIconTexture();
		Info.KeyName = KeyName;

		EquippedAbilityList.Add(KeyName, Info);

		if (Rage)
		{
			RageAbilities.Add(Rage->GetAbilityName(), Rage);
		}
		else if (Attack)
		{
			AttackAbilities.Add(Attack->GetAbilityName(), Attack);
			
		}

		return true;
	}
	return false;
}

bool UAbilityManagerComponent::SetEnableAbility(const FName & AbilityKeyName, bool enable)
{
	if (GenericAbilities.Find(AbilityKeyName))
	{
		UAttackAbility* Attack = *AttackAbilities.Find(AbilityKeyName);
		URageAttackAbility* Rage = *RageAbilities.Find(AbilityKeyName);
		FAbilityInfo* ability = EquippedAbilityList.Find(AbilityKeyName);

		if (Attack && ability)
		{
			ability->bIsEnabled = enable;
		}
		else if (Rage && ability)
		{
			ability->bIsEnabled = enable;
		}

		return true;
	}
	return false;
}

void UAbilityManagerComponent::SetDisableAll()
{
	for (TPair<FName, FAbilityInfo>& Pair : EquippedAbilityList)
	{
		Pair.Value.bIsEnabled = false;
	}
}

void UAbilityManagerComponent::RemoveAllAbilities()
{
	RageAbilities.Empty();
	AttackAbilities.Empty();
	GenericAbilities.Empty();
	EquippedAbilityList.Empty();
}

bool UAbilityManagerComponent::RemoveAbility(const FName & AbilityKeyName)
{
	if (GenericAbilities.Find(AbilityKeyName))
	{
		if (AttackAbilities.Find(AbilityKeyName))
		{
			AttackAbilities.Remove(AbilityKeyName);
		}
		else if(RageAbilities.Find(AbilityKeyName))
		{
			RageAbilities.Remove(AbilityKeyName);
		}

		GenericAbilities.Remove(AbilityKeyName);
		EquippedAbilityList.Remove(AbilityKeyName);
		return true;
	}
	return false;
}

bool UAbilityManagerComponent::ChangeAbility(const FName & AbilityKeyName, const TSubclassOf<UAbilityBase> Ability)
{
	if (RemoveAbility(AbilityKeyName))
	{
		AddNewAbility(AbilityKeyName, Ability);
	}
	return false;
}

bool UAbilityManagerComponent::GetCanUseAbility(const FName & AbilityKeyName, const UCombatData ** UserCombatData)
{
	FAbilityInfo* Ability = EquippedAbilityList.Find(AbilityKeyName);

	if (UserCombatData && Ability && Ability->bIsEnabled )
	{
		const UCombatData* CombatData = *UserCombatData;
		if (IsValid(CombatData) && Ability->bIsEnabled)
		{
			const float stamina = CombatData->GetExternalStats()->StaminaPoints;
			const float mana = CombatData->GetExternalStats()->ManaPoints;
			const float rage = CombatData->GetExternalStats()->FuryPoints;

			if (RageAbilities.Num() > 0 && RageAbilities.Find(Ability->AbilityName))
			{
				const URageAttackAbility* RageAbility = *RageAbilities.Find(Ability->AbilityName);
				if (IsValid(RageAbility))
				{
					if (RageAbility->RageCost <= rage && RageAbility->StaminaCost <= stamina)
					{
						
						return true;
					}
				}
			}
			if (AttackAbilities.Num() > 0 && AttackAbilities.Find(Ability->AbilityName))
			{
				const UAttackAbility* Attack = *AttackAbilities.Find(Ability->AbilityName);

				if (IsValid(Attack))
				{
					if (Attack->StaminaCost <= stamina)
					{
						
						return true;
					}
				}
			}
			
		}
	}
	return false;
}

bool UAbilityManagerComponent::UseAbility(const FName & AbilityKeyName, const UCombatData ** UserCombatData)
{
	if (GetCanUseAbility(AbilityKeyName, UserCombatData))
	{
		FAbilityInfo* Ability = EquippedAbilityList.Find(AbilityKeyName);

		if (UserCombatData && Ability)
		{
			Ability->bIsEnabled = false;
			Ability->CurrentTime = 0.0f;
		}
		return true;
	}
	return false;
}

const UAbilityBase * UAbilityManagerComponent::GetGenericAbility(const FName & AbilityKeyName) const
{
	if (GenericAbilities.Find(AbilityKeyName))
	{
		return *GenericAbilities.Find(AbilityKeyName);
	}

	return nullptr;
}

const UAbilityBase* UAbilityManagerComponent::GetAttackAbility(const FName& AbilityKeyName) const
{
	if (AttackAbilities.Find(AbilityKeyName))
	{
		return *AttackAbilities.Find(AbilityKeyName);
	}

	return nullptr;
}

const UAbilityBase* UAbilityManagerComponent::GetRageAbility(const FName& AbilityKeyName) const
{
	if (RageAbilities.Find(AbilityKeyName))
	{
		return *RageAbilities.Find(AbilityKeyName);
	}
	return nullptr;
}

const TMap<FName, FAbilityInfo>* UAbilityManagerComponent::GetEquippedAbilities()
{
	return &EquippedAbilityList;
}

