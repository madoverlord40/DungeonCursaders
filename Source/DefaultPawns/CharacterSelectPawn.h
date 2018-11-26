// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterSelectPawn.generated.h"

class UCameraComponent;
class ACharSelectPawnController;
class APlayerSelectionActor;

UCLASS()
class DUNGEONCRUSADES_API ACharacterSelectPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACharacterSelectPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PossessedBy(AController* Controller) override;

	void SetLookatTarget(const TWeakObjectPtr<APlayerSelectionActor> Target);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComp;

	TWeakObjectPtr<ACharSelectPawnController> PlayerController;
	TWeakObjectPtr<APlayerSelectionActor> LookatTarget;
};
