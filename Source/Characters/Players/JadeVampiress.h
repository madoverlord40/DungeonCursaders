// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Players/PlayerCharacterBase.h"
#include "GenericTeamAgentInterface.h"
#include "JadeVampiress.generated.h"

class UParticleSystem;
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AJadeVampiress : public APlayerCharacterBase, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AJadeVampiress();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	/** Called when the character's movement enters falling */
	virtual void Falling() override;

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
	
	virtual bool PerformAreaAttack(const FName& AnimSectionName, bool IsRageAttack) override;

	virtual bool PerformSingleAttack(const FName& AnimSectionName) override;

	virtual bool PerformRageAttack(const FName& AnimSectionName) override;

    virtual void StartActionJump() override;

    virtual void StopActionJump() override;

    virtual void ToggleCombatMode() override;

    virtual void PerformForwardMovement(const float value) override;
    virtual void PerformRightMovement(const float value) override;

    virtual void SetHasTargetLocked(bool locked) override;

	virtual void OnAnimationNotify(const FName& NotificationName, const NotifyTypes Type, const NotifyTypeStates State) override;

	virtual void OnNotifyAttackComplete() override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 MaxNormalAttackCombo = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ComboTimeLimit = 1.5f;

    class UJadeVampiressAnimation* Animation;

	//----------------------------------------------------------------------//
	// IGenericTeamAgentInterface
	//----------------------------------------------------------------------//
private:
	FGenericTeamId GenericTeamID;
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override { return GenericTeamID; }
};
