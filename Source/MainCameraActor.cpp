// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCameraActor.h"
#include "Characters/BattleCharacterBase.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"

bool PerformMoveTrace(const FVector& Location, const UWorld* World);

AMainCameraActor::AMainCameraActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
		
}

void AMainCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentViewCharacter.IsValid() && IsAPlayerTarget)
	{
		FRotator Rotation = CurrentViewCharacter->GetControlRotation();
		FVector Location = GetActorLocation();

		const FVector LookAtLocation = CurrentViewCharacter->GetActorLocation();
		FVector OrbitalOffset(-ViewPointDistance, 0, 0);

		FVector AxisRotate = OrbitalOffset.RotateAngleAxis(Rotation.Yaw, FVector::UpVector);
		FVector NewLocation = (LookAtLocation + CameraToTargetOffset) + AxisRotate;

		const float distance = FVector::Dist((LookAtLocation + CameraToTargetOffset), Location);
		
		if (distance <= ViewPointDistance)
		{
			if (PerformMoveTrace(NewLocation, GetWorld()))
			{
				SetActorLocation(NewLocation);
			}
			//failed might be stuck move closer
			else
			{
				OrbitalOffset = FVector(-(ViewPointDistance / 2.0f), 0, 0);
				AxisRotate = OrbitalOffset.RotateAngleAxis(Rotation.Yaw, FVector::UpVector);
				NewLocation = LookAtLocation + AxisRotate + CameraToTargetOffset;

				if (PerformMoveTrace(NewLocation, GetWorld()))
				{
					SetActorLocation(NewLocation);
				}
			}

			FRotator LookRotation(CurrentRotation.Pitch, Rotation.Yaw, 0);

			// Make Camera look at Target
			SetActorRotation(LookRotation);
		}
		//camera might be stuck on wall and character moved so zoom into correct distance
		else
		{
			FVector DirectionToTarget = LookAtLocation - Location;
			DirectionToTarget.Normalize();
		
			FVector DeltaLocation = Location + (DirectionToTarget * ViewPointDistance);

			NewLocation = FMath::VInterpConstantTo(Location, DeltaLocation, DeltaTime, FollowInteropSpeed);

			SetActorLocation(NewLocation);

			FVector LookAtDirection = (LookAtLocation + CameraToTargetOffset) - Location;

			FRotator NewRotation = LookAtDirection.ToOrientationRotator();

			NewRotation.Pitch = CurrentRotation.Pitch;
			// Make Camera look at Target
			SetActorRotation(FQuat(NewRotation));
		}
	}
}

void AMainCameraActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	

}

void AMainCameraActor::SetViewCharacter(const ABattleCharacterBase * ViewTarget, bool isPlayer)
{
	IsAPlayerTarget = isPlayer;

	CurrentViewCharacter = (ABattleCharacterBase *)ViewTarget;

	if (isPlayer)
	{
		CurrentRotation = ViewTarget->GetControlRotation();
	}
	else 
	{
		CurrentRotation = GetActorRotation();
	}

	CurrentLocation = GetActorLocation();

	const FVector LookAtLocation = CurrentViewCharacter->GetActorLocation();
	const FVector OrbitalOffset(-ViewPointDistance, 0, 0);

	const FVector AxisRotate = OrbitalOffset.RotateAngleAxis(CurrentRotation.Yaw, FVector::UpVector);
	const FVector NewLocation = (LookAtLocation + CameraToTargetOffset) + AxisRotate;

	SetActorLocation(NewLocation);

	FVector LookAtDirection = (LookAtLocation + CameraToTargetOffset ) - CurrentLocation;

	FRotator NewRotation = LookAtDirection.ToOrientationRotator();

	// Make Camera look at Target
	SetActorRotation(FQuat(NewRotation));
}

void AMainCameraActor::SetViewRotation(const FRotator & NewRotation)
{
	SetActorRotation(NewRotation);
}

bool AMainCameraActor::SetOrbitalRotationValue(const float Value, const FRotator& ControlRotation)
{
	if (IsAPlayerTarget && CurrentViewCharacter.IsValid() && Value != 0.0f)
	{
		CurrentRotation.Yaw = ControlRotation.Yaw;
		CurrentLocation = GetActorLocation();

		const FVector LookAtLocation = CurrentViewCharacter->GetActorLocation();
		const FVector OrbitalOffset(-ViewPointDistance, 0, 0);

		const FVector AxisRotate = OrbitalOffset.RotateAngleAxis(CurrentRotation.Yaw, FVector::UpVector);
		const FVector NewLocation = (LookAtLocation + CameraToTargetOffset) + AxisRotate;
		
		if (PerformMoveTrace(NewLocation, GetWorld()))
		{
			SetActorLocation(NewLocation);

			FRotator NewRotation(GetActorRotation().Pitch, CurrentRotation.Yaw, 0);

			// Make Camera look at Target
			SetActorRotation(NewRotation);

			return true;
		}
	}

	return false;
}

bool AMainCameraActor::SetPitchRotationValue(const float Value, const FRotator& ControlRotation)
{
	if (IsAPlayerTarget && Value != 0.0f)
	{
		if (IsAPlayerTarget && CurrentViewCharacter.IsValid())
		{
			CurrentRotation.Pitch += Value;
			CurrentRotation.Pitch = FMath::ClampAngle(CurrentRotation.Pitch, -PitchRotationClamp, PitchRotationClamp);

			UE_LOG(LogTemp, Warning, TEXT("DeltaPitch: %f"), CurrentRotation.Pitch);
			FRotator NewRotation(CurrentRotation.Pitch, ControlRotation.Yaw, 0);

			// Make Camera look at Target
			SetActorRotation(NewRotation);
		}
	}

	return true;
}

bool AMainCameraActor::SetOrbitalRotationValue(const float Value)
{
	if (IsAPlayerTarget && CurrentViewCharacter.IsValid() && Value != 0.0f)
	{
		CurrentRotation = GetActorRotation();
		CurrentLocation = GetActorLocation();

		float DeltaYaw = CurrentRotation.Yaw + Value;

		const FVector LookAtLocation = CurrentViewCharacter->GetActorLocation();
		const FVector OrbitalOffset(-ViewPointDistance, 0, 0);

		const FVector AxisRotate = OrbitalOffset.RotateAngleAxis(DeltaYaw, FVector::UpVector);
		const FVector NewLocation = (LookAtLocation + CameraToTargetOffset) + AxisRotate;

		if (PerformMoveTrace(NewLocation, GetWorld()))
		{
			SetActorLocation(NewLocation);

			FRotator NewRotation(CurrentRotation.Pitch, DeltaYaw, 0);

			// Make Camera look at Target
			SetActorRotation(NewRotation);

			return true;
		}
	}

	return false;
}

bool AMainCameraActor::SetPitchRotationValue(const float Value)
{
	if (IsAPlayerTarget && Value != 0.0f)
	{
		if (IsAPlayerTarget && CurrentViewCharacter.IsValid())
		{
			CurrentRotation = GetActorRotation();
			float DeltaPitch = CurrentRotation.Pitch + Value;

			DeltaPitch = FMath::ClampAngle(DeltaPitch, -PitchRotationClamp, PitchRotationClamp);
			FRotator NewRotation(DeltaPitch, CurrentRotation.Yaw, 0);

			// Make Camera look at Target
			SetActorRotation(NewRotation);
		}
	}
	return true;
}

void AMainCameraActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetViewTarget(this);
}

bool PerformMoveTrace(const FVector& Location, const UWorld* World)
{
	bool CanMove = false;

	if (IsValid(World))
	{
		FHitResult Result;
		CanMove = !World->SweepSingleByChannel(Result, Location, Location,
			FQuat(FRotator(90, 90, 90)), ECC_WorldStatic, FCollisionShape::MakeCapsule(25, 25));

	}

	return CanMove;
}