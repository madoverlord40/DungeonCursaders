// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BattleCharacterBase.h"
#include "../DataStructures/CombatData.h"
#include "UI/ItemInventoryUI.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionStimuliSourceComponent.h"
#include "PlayerCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class APlayerControllerBase;

UCLASS()
class DUNGEONCRUSADES_API APlayerCharacterBase : public ABattleCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    void HandleItemPickup(const TWeakObjectPtr<class ALootableItemBase> LootableItem);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	/** Called when the character's movement enters falling */
	virtual void Falling() override {}

	/** Called when character's jump reaches Apex. Needs CharacterMovement->bNotifyApex = true */
	virtual void NotifyJumpApex() override;

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

    virtual void OnNotifyAttackComplete() override;

  	void MouseY(float value);

	UFUNCTION()
	virtual void OnBeginBodyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    virtual void PerformForwardMovement(const float value);
    virtual void PerformRightMovement(const float value);

    virtual void SetHasTargetLocked(bool locked);
    virtual void SetSpringArmRotationX(const float value);

    virtual bool PerformAreaAttack(const FName& AnimSectionName, bool IsRageAttack) override;

    virtual bool PerformSingleAttack(const FName& AnimSectionName) override;

    virtual bool PerformRageAttack(const FName& AnimSectionName) override;
    
    virtual void StartActionJump() override;

    virtual void StopActionJump() override;

    virtual void ToggleCombatMode() override;

	virtual void OnAnimationNotify(const FName& NotificationName, const NotifyTypes Type, const NotifyTypeStates State) override;

	virtual bool ComputeHitAndDamage(const FCombatDamageData* InDamageData, FCombatDamageResult& OutResult) override;

	virtual void UpdateHealthbar() override;

	void OnNotifyTransitionCompleted();

	void OnNotifyIntroAnimationCompleted() {}

	void AddRewardExp(const uint32 reward);

	virtual void OnNotifyWeaponHitTarget(AActor* HitActor, const FVector& HitLocation, bool IsLeftWeapon) override;

	virtual void EnableWeaponCollider(ECollisionEnabled::Type Enable) override;

	bool UnEquipItem(const FInventoryItemInfo* UnequipInfo);

	bool EquipItem(const FInventoryItemInfo* UnequipInfo);

protected:
	void EquipGear();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UActionVoicesComponent* VoicesComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionStimuliSourceComponent* AIPerceptionSource;


	TWeakObjectPtr<APlayerControllerBase> PlayerController;
	
	uint32 bIsTogglingCombatMode : 1;
    uint8 CurrentAttackComboIndex = 0;
    FName CurrentAttackComboName;
    uint32 DoAttackComboTimer : 1;
    float ComboTimer = 0.0f;

};
