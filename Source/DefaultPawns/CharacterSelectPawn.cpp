// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterSelectPawn.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "CharSelectPawnController.h"
#include "SelectionPawns/PlayerSelectionActor.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"

// Sets default values
ACharacterSelectPawn::ACharacterSelectPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
}

// Called when the game starts or when spawned
void ACharacterSelectPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterSelectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LookatTarget.IsValid())
	{
		const FVector& Location = GetActorLocation();
		const FVector& TargetLocation = (LookatTarget->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f)); 
		
		FVector Direction = TargetLocation - Location;
		Direction.Normalize(0.001f);

		const FRotator TargetRotation = Direction.ToOrientationRotator();

		const FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f);

		SetActorRotation(CurrentRotation);

	}
}

// Called to bind functionality to input
void ACharacterSelectPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}


void ACharacterSelectPawn::PossessedBy(AController* controller)
{
	Super::PossessedBy(controller);

	PlayerController = Cast<ACharSelectPawnController>(controller);
}

void ACharacterSelectPawn::SetLookatTarget(const TWeakObjectPtr<APlayerSelectionActor> Target)
{
	LookatTarget = Target;
}

