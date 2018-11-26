// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionTypes.h"
#include "../Animations/EnemyAnimationBasse.h"
#include "Characters/Players/Components/AbilityManagerComponent.h"
#include "EnemyAIControllerBase.generated.h"

class AEnemyCharacterBase;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;
class APlayerCharacterBase;
class ULootSystemComponent;
class UActionVoicesComponent;


USTRUCT(BlueprintType)
struct FAIConfigurationData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RoamingRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RoamMoveAcceptRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AggroDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float IdleWaitDelay = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveToTargetStopDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRangeMax = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRangeMin = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnInAIDelayTime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector HomePosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool HasTargetSighted = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsChasing = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsEngaging = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsPerceptionActive = false;
};
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()
	

public:
	AEnemyAIControllerBase();
	

	virtual void Kill();
	virtual void Tick(float DeltaTime) override;

	/** Called immediately before gameplay begins. */
	virtual void PreInitializeComponents() override;

	virtual void PostRegisterAllComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void Possess(APawn* InPawn) override;
	virtual void UnPossess() override;

	UFUNCTION(BlueprintCallable)
	virtual bool PerformMoveTo(const FVector& Destination, float MoveRadius);

	UFUNCTION(BlueprintCallable)
	virtual bool PerformChaseTarget(bool &AlreadyAtGoal);

	UFUNCTION(BlueprintCallable)
	virtual bool PerformAttackTarget();

	UFUNCTION(BlueprintCallable)
	bool IsInAttackingRange();

    void AlertDamageEvent(AActor* Instigator, const float damage);

	/** Notifies AIController of changes in given actors' perception */
	UFUNCTION()
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

	UFUNCTION()
	virtual void TargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

	void NotifyAttackComplete();

	/** Called on completing current movement request */
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	const FAIConfigurationData* GetAIConfig() const { return &AIConfiguration; }

	UFUNCTION(BlueprintCallable)
	APlayerCharacterBase* GetSensedTarget();

	UFUNCTION(BlueprintCallable)
	void ActivatePerception();

    void AbortMoveTo();

    void RerouteChasePlayer();

	void LookAtSensedTarget();

	void UpdateOriginAI();

	bool ChooseRandomAbility();

	void SetCurrentLevel(const uint8 level);

	const UAttackAbility* GetSelectedAbility();

protected:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	UAISenseConfig_Sight* SightConfig;

    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
    UAISenseConfig_Damage* DamageConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBehaviorTree* TreeClassToRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FAIConfigurationData AIConfiguration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAbilityManagerComponent* AbilityComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//ULootSystemComponent* LootSystemComponent;


private:
    UPROPERTY()
	AEnemyCharacterBase* EnemyCharacter;

	UPROPERTY()
	TWeakObjectPtr<UEnemyAnimationBasse> EnemyAnimation;

	UPROPERTY()
	TWeakObjectPtr<APlayerCharacterBase> SensedTarget;

	UPROPERTY()
	TWeakObjectPtr<UAttackAbility> SelectedAttackAbility;

	UPROPERTY()
	TWeakObjectPtr<UAttackAbility> SelectedRageAbility;
};
