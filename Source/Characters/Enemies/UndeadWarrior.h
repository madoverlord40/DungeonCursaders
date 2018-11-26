// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/EnemyCharacterBase.h"
#include "UndeadWarrior.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AUndeadWarrior : public AEnemyCharacterBase
{
	GENERATED_BODY()
	
public:	
	AUndeadWarrior();



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

	virtual void OnNotifyAttackComplete() override;

protected:

};
