// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSelectionActor.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "PlayerSelectActorAnim.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SpotLightComponent.h"

// Sets default values
APlayerSelectionActor::APlayerSelectionActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root Scene Comp"));
	RootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("Capsule Comp"));
	RootCapsuleComponent->SetupAttachment(RootComponent);
		
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Skeletal Mesh Comp"));
	MeshComponent->SetupAttachment(RootCapsuleComponent);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(FName("Spotlight Comp"));
	SpotLight->SetupAttachment(RootCapsuleComponent);
}

// Called when the game starts or when spawned
void APlayerSelectionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerSelectionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerSelectionActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(MeshComponent))
	{
		AnimationComponent = Cast<UPlayerSelectActorAnim>(MeshComponent->GetAnimInstance());
		AnimationComponent->SetIdleState(true);
		
	}
}

void APlayerSelectionActor::SetEnableLight(bool enable)
{
	float intensity = enable ? 100000.0f : 0.0f;
	SpotLight->SetIntensity(intensity);

	if (AnimationComponent.IsValid())
	{
		AnimationComponent->SetIsSelecting(enable);
	}
}

void APlayerSelectionActor::SetSelectedForPlay()
{
	if (AnimationComponent.IsValid())
	{
		AnimationComponent->SetSelectedForPlay();
	}
}
