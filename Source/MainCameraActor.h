// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "MainCameraActor.generated.h"

class ABattleCharacterBase;
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AMainCameraActor : public ACineCameraActor
{
	GENERATED_BODY()
	
	
public:
	// Ctor
	AMainCameraActor(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	void SetViewCharacter(const ABattleCharacterBase* ViewTarget, bool isPlayer);

	void SetViewRotation(const FRotator& NewRotation);

	bool SetOrbitalRotationValue(const float Value, const FRotator& ControlRotation);

	bool SetPitchRotationValue(const float Value, const FRotator& ControlRotation);

	bool SetOrbitalRotationValue(const float Value);

	bool SetPitchRotationValue(const float Value);


protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	uint32 IsAPlayerTarget : 1;

	UPROPERTY(EditAnywhere)
	float PitchRotationClamp = 45.0f;
	
	UPROPERTY(EditAnywhere)
	float ViewPointDistance = 300.0f;

	UPROPERTY(EditAnywhere)
	float FollowInteropSpeed = 3.0f;

	UPROPERTY(EditAnywhere)
	FVector CameraToTargetOffset;

	TWeakObjectPtr<ABattleCharacterBase> CurrentViewCharacter;

	FRotator CurrentRotation;
	FVector CurrentLocation;
};
