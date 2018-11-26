// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingDamageTextActor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"

AFloatingDamageTextActor::AFloatingDamageTextActor(const FObjectInitializer & ObjectInitializer) : Super (ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFloatingDamageTextActor::BeginPlay()
{
	Super::BeginPlay();

}

void AFloatingDamageTextActor::PostActorCreated()
{
	Super::PostActorCreated();
	
	FVector Location = GetActorLocation();
	Location.Z += 100.0f;
	
	SetActorRelativeLocation(Location);

	GetTextRender()->SetTextRenderColor(FColor::White);
	GetTextRender()->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	GetTextRender()->SetWorldSize(24);
}

void AFloatingDamageTextActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AFloatingDamageTextActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Timer += DeltaSeconds;

	if (Timer >= ShowTextMaxTime) {
		Destroy();
	}
	else {
		FVector Location = GetActorLocation();

		Location.Z += FloatingSpeed;

		SetActorLocation(Location);

		FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

		FRotator LookAtRot = FRotationMatrix::MakeFromX(CameraLocation - Location).Rotator();

		LookAtRot.Pitch = 0.0f;
		LookAtRot.Roll = 0.0f;

		SetActorRotation(LookAtRot);
		
	}
}

void AFloatingDamageTextActor::SetText(const FString & TextString)
{
	GetTextRender()->SetText(FText::FromString(TextString));
}
