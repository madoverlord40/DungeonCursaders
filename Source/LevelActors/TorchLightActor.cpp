// Fill out your copyright notice in the Description page of Project Settings.

#include "TorchLightActor.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/PointLightComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "RoomController.h"

// Sets default values
ATorchLightActor::ATorchLightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TorchMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Torch Mesh Component"));
	RootComponent = TorchMeshComponent;

	TorchLightComponent = CreateDefaultSubobject<UPointLightComponent>(FName("Torch Light Component"));
	TorchLightComponent->SetupAttachment(TorchMeshComponent, FName("FireLightSocket"));

	FireParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("Torch Fire Component"));
	FireParticleComponent->SetupAttachment(TorchMeshComponent, FName("FireParticleSocket"));
}

// Called when the game starts or when spawned
void ATorchLightActor::BeginPlay()
{
	Super::BeginPlay();
	
}

static float LightColorProgress = 0.0f;
static float LightFlickerDelta = 0.0f;
static float RandomIntensity = 0.0f;
static float RandomRadius = 0.0f;
static bool FlickerUpward = true;
static uint8 RandomCounter = 0;

// Called every frame
void ATorchLightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUseLightFlicker)
	{
		if (RandomCounter == 2)
		{
			RandomIntensity = FMath::RandRange(LightIntensityMin, LightIntensityMax);
			RandomRadius = FMath::RandRange(LightRadiusMin, LightRadiusMax);
			RandomCounter = 0;
		}
		if (FlickerUpward)
		{
			LightFlickerDelta += (0.03f * DeltaTime);
			if (LightFlickerDelta > PI)
			{
				LightFlickerDelta = PI;
				FlickerUpward = false;
				RandomCounter++;
			}
		}
		else
		{
			LightFlickerDelta -= (0.03f * DeltaTime);
			if (LightFlickerDelta < 0.0f)
			{
				FlickerUpward = true;
				LightFlickerDelta = 0.0f;
				RandomCounter++;
			}
		}

		const float Intensity = ((FMath::Sin(LightFlickerDelta) * RandomIntensity) + RandomIntensity / 2.0f);
		const float Radius = ((FMath::Sin(LightFlickerDelta) * RandomRadius) + RandomRadius / 2.0f);

		//UE_LOG(LogTemp, Warning, TEXT("Radius: %f"), Radius);
		TorchLightComponent->SetAttenuationRadius(Radius);
		TorchLightComponent->SetIntensity(Intensity);
	}
}

void ATorchLightActor::PostActorCreated()
{
	Super::PostActorCreated();

	if (IsValid(TorchMesh))
	{
		TorchMeshComponent->SetStaticMesh(TorchMesh);
		TorchLightComponent->AttachToComponent(TorchMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("FireLightSocket"));

		if (IsValid(FireParticleTemplate))
		{
			FireParticleComponent->AttachToComponent(TorchMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("FireParticleSocket"));
			FireParticleComponent->SetTemplate(FireParticleTemplate);
			FireParticleComponent->ActivateSystem();
		}
	}
}

void ATorchLightActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TorchLightComponent->IntensityUnits = ELightUnits::Candelas;

	ARoomController* controller = Cast<ARoomController>(GetParentActor());

	if (IsValid(controller))
	{
		controller->RegisterTorch(this);
		SetEnableLight(bLightEnabled, bUseLightFlicker);
	}

	
}

void ATorchLightActor::SetEnableLight(bool enabled, bool flickering)
{
	bUseLightFlicker = flickering;
	bLightEnabled = enabled;

	TorchLightComponent->SetVisibility(enabled);
	FireParticleComponent->SetVisibility(enabled);
	
	TorchLightComponent->SetAttenuationRadius(LightRadiusMin);
	TorchLightComponent->SetIntensity(LightIntensityMin);
	TorchLightComponent->SetLightColor(FlickerEndColor);
	
}


