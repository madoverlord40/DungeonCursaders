// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityManagerComponent.generated.h"


class UAbilityBase;
class UAttackAbility;
class URageAttackAbility;
class UTexture2D;
class UCombatData;

USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY()
	FName AbilityName;

	UPROPERTY()
	FName KeyName;

	UPROPERTY()
	uint32 bIsEnabled : 1;

	UPROPERTY()
	float CooldownTime;

	UPROPERTY()
	float CurrentTime;

	UPROPERTY()
	UTexture2D* IconTexture;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCRUSADES_API UAbilityManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	/**
	* Initializes the component.  Occurs at level startup. This is before BeginPlay (Actor or Component).
	* All Components in the level will be Initialized on load before any Actor/Component gets BeginPlay
	* Requires component to be registered, and bWantsInitializeComponent to be true.
	*/
	virtual void InitializeComponent() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool AddNewAbility(const FName& KeyName, const TSubclassOf<UAbilityBase> Ability);
	bool SetEnableAbility(const FName& AbilityKeyName, bool enable);
	void SetDisableAll();
	void RemoveAllAbilities();
	bool RemoveAbility(const FName& AbilityKeyName);
	bool ChangeAbility(const FName& AbilityKeyName, const TSubclassOf<UAbilityBase> Ability);
	bool GetCanUseAbility(const FName& AbilityKeyName, const UCombatData** UserCombatData);
	bool UseAbility(const FName& AbilityKeyName, const UCombatData** UserCombatData);
	const UAbilityBase* GetGenericAbility(const FName& AbilityKeyName) const;
	const UAbilityBase* GetAttackAbility(const FName& AbilityKeyName) const;
	const UAbilityBase* GetRageAbility(const FName& AbilityKeyName) const;
	const TMap<FName, FAbilityInfo>* GetEquippedAbilities();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, TSubclassOf<UAbilityBase>> AbilityClasses;

	UPROPERTY()
	TMap<FName, UAbilityBase*> GenericAbilities;

	UPROPERTY()
	TMap<FName, UAttackAbility*> AttackAbilities;

	UPROPERTY()
	TMap<FName, URageAttackAbility*> RageAbilities;

private:
	UPROPERTY()
	TMap<FName, FAbilityInfo> EquippedAbilityList;

};
