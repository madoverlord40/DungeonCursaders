// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DataStructures/CombatData.h"
#include "DataStructures/WeaponObject.h"
#include "Abilities/RageAttackAbility.h"
#include "Players/Components/CombatComponent.h"
#include "Players/Components/AbilityManagerComponent.h"
#include "BattleCharacterBase.generated.h"

enum class NotifyTypes : uint8;
enum class NotifyTypeStates : uint8;


UCLASS()
class DUNGEONCRUSADES_API ABattleCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	ABattleCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

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

	virtual void OnNotifyJumpCompleted();

	virtual void OnNotifyAttackComplete();

    UFUNCTION(BlueprintCallable)
    virtual bool PerformAreaAttack(const FName& AnimSectionName, bool IsRageAttack);

    UFUNCTION(BlueprintCallable)
    virtual bool PerformSingleAttack(const FName& AnimSectionName);

    UFUNCTION(BlueprintCallable)
    virtual bool PerformRageAttack(const FName& AnimSectionName);

    UFUNCTION(BlueprintCallable)
    virtual void StartActionJump();

    UFUNCTION(BlueprintCallable)
    virtual void StopActionJump();

    UFUNCTION(BlueprintCallable)
    virtual void ToggleCombatMode();

	virtual bool ComputeHitAndDamage(const FCombatDamageData* InDamageData, FCombatDamageResult& OutResult);

	virtual void Kill();

	virtual const UCombatData* GetCharacterCombatData() const;
	virtual const UCombatComponent* GetCombatControler() const;

	UFUNCTION(BlueprintCallable)
	virtual void EnableWeaponCollider(ECollisionEnabled::Type Enable);

	virtual void OnAnimationNotify(const FName& NotificationName, const NotifyTypes Type, const NotifyTypeStates State);

	void SpawnFloatingText();

	virtual void UpdateHealthbar() {};

	virtual void OnNotifyWeaponHitTarget(AActor* HitActor, const FVector& HitLocation, bool IsLeftWeapon) {};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCombatComponent* CombatComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEquipmentComponent* EquipmentManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* BodyCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UWeaponObject> LeftWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UWeaponObject> RightWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CharacterName = "Enemy";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 CurrentLevel = 1;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ArmorSlotTypes, TSubclassOf<UArmorObject>> ArmorClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName BodyCollisionProfileName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName BodyCollisionSocketName;

	UPROPERTY()
	TArray<FCombatDamageResult> DamageResultList;
	int32 DamageResultCounter;

    uint32 TookCritialHit : 1;
    uint32 IsAlive : 1;
	uint32 IsUsingAreaAttack : 1;
	uint32 IsInCombat : 1;
	uint32 IsUsingRageAttack : 1;
	uint32 IsAttacking : 1;
	uint32 IsJumping : 1;
};
