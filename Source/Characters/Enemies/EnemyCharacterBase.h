// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BattleCharacterBase.h"
#include "../EnemyHealthBarWidget.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Characters/Players/Components/ActionVoicesComponent.h"
#include "Characters/Players/Components/CombatComponent.h"
#include "EnemyCharacterBase.generated.h"

class AEnemyAIControllerBase;
class ARoomController;
class ULootSystemComponent;

UCLASS()
class DUNGEONCRUSADES_API AEnemyCharacterBase : public ABattleCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	/**
	* Called upon landing when falling, to perform actions based on the Hit result. Triggers the OnLanded event.
	* Note that movement mode is still "Falling" during this event. Current Velocity value is the velocity at the time of landing.
	* Consider OnMovementModeChanged() as well, as that can be used once the movement mode changes to the new mode (most likely Walking).
	*
	* @param Hit Result describing the landing that resulted in a valid landing spot.
	* @see OnMovementModeChanged()
	*/
	virtual void Landed(const FHitResult& Hit) override;

	/**
	* Called when pawn's movement is blocked
	* @param Impact describes the blocking hit.
	*/
	virtual void MoveBlockedBy(const FHitResult& Impact) override;

	UFUNCTION(BlueprintCallable)
	virtual void EnableCombatMode(bool enable);

	UFUNCTION(BlueprintCallable)
	virtual void PlayChaseAnimation(bool play);

	UFUNCTION(BlueprintCallable)
	virtual void PlayMovingAnimation(bool play);

	UFUNCTION(BlueprintCallable)
	virtual void OnNotifyAttackComplete() override;

	virtual bool ComputeHitAndDamage(const FCombatDamageData* InDamageData, FCombatDamageResult& OutResult) override;

	//bool ComputeHitAndDamage(const FCombatDamageData* InDamageData, FCombatDamageResult& OutResult);
	UFUNCTION()
	virtual void OnBeginBodyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual bool PerformAreaAttack(const FName& AnimSectionName, bool bIsRageAttack) override;

	virtual bool PerformSingleAttack(const FName& AnimSectionName) override;

	virtual bool PerformRageAttack(const FName& AnimSectionName) override;

	virtual void EnableWeaponCollider(ECollisionEnabled::Type Enable) override;

	virtual void Kill() override;

    void TimedDestroyObject();

	void SetRoomController(const TWeakObjectPtr<ARoomController> owner, const FName& KeyName);

	bool GetIsEnemyAlive();

	void SetEnableTargetLock(bool enable);

	void SetHighlightedAsTarget(bool enable);

	virtual void UpdateHealthbar() override;

	virtual void PostActorCreated() override;

	virtual void OnNotifyWeaponHitTarget(AActor* HitActor, const FVector& HitLocation, bool IsLeftWeapon) override;

	void SetEnemyLevel(const uint8 level);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthBarComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UActionVoicesComponent* VoicesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
	TSubclassOf<UEnemyHealthBarWidget> HealthBarWidgetClass;

    UPROPERTY()
	AEnemyAIControllerBase* AIController;

	bool IsAttacking;
  
	TWeakObjectPtr<ARoomController> OwnerRoomController;
	FName OwnerMapKeyName;
};
