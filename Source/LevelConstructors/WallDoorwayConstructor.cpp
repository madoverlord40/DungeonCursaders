// Fill out your copyright notice in the Description page of Project Settings.

#include "WallDoorwayConstructor.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/PointLightComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AWallDoorwayConstructor::AWallDoorwayConstructor() : Super()
{
    PrimaryActorTick.bCanEverTick = true;

    DoorwayFrameMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Doorway Frame Component"));
    DoorwayFrameMeshComponent->SetupAttachment(RootComponent);

    DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Door Mesh Component"));
    DoorMeshComponent->SetupAttachment(DoorwayFrameMeshComponent);

    LeftTorchHolderComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("LeftTorchHolder"));
    LeftTorchHolderComponent->SetupAttachment(DoorwayFrameMeshComponent);

    RightTorchHolderComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("RightTorchHolder"));
    RightTorchHolderComponent->SetupAttachment(DoorwayFrameMeshComponent);
    
    LeftTorchLightComponent = CreateDefaultSubobject<UPointLightComponent>(FName("LeftTorchLight"));
    LeftTorchLightComponent->SetupAttachment(LeftTorchHolderComponent, FName("FireLightSocket"));

    RightTorchLightComponent = CreateDefaultSubobject<UPointLightComponent>(FName("RightTorchLight"));
    RightTorchLightComponent->SetupAttachment(RightTorchHolderComponent, FName("FireLightSocket"));

    LeftTorchFireComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("LeftTorchFire"));
    LeftTorchFireComponent->SetupAttachment(LeftTorchHolderComponent, FName("FireParticleSocket"));

    RightTorchFireComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("RightTorchFire"));
    RightTorchFireComponent->SetupAttachment(RightTorchHolderComponent, FName("FireParticleSocket"));

    RightWallColliderBox = CreateDefaultSubobject<UBoxComponent>(FName("RightWallBox"));
    RightWallColliderBox->SetupAttachment(RootComponent);

    DoorColliderBox = CreateDefaultSubobject<UBoxComponent>(FName("DoorBox"));
    DoorColliderBox->SetupAttachment(DoorMeshComponent);
}

void AWallDoorwayConstructor::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AWallDoorwayConstructor::Tick(float DeltaTime)
{
    AActor::Tick(DeltaTime);

    if (bUsesTorchLight && bUsesLightFlicker)
    {
        if (IsTickingUp)
        {
            FlickerTimer += DeltaTime;
            ColorInterop += DeltaTime;

            if (FlickerTimer > 3.14159)
            {
                IsTickingUp = false;
            }
            if (ColorInterop > 1.0)
            {
                ColorInterop = 1.0f;
            }
            
            
        }
        else
        {
            FlickerTimer -= DeltaTime;
            ColorInterop -= DeltaTime;
            if (FlickerTimer < 0.0f)
            {
                IsTickingUp = true;
            }
            if (ColorInterop < 0.0f)
            {
                ColorInterop = 0.0f;
            }
        }

        float LeftAttenuation = LightFlickerRadiusMin + FMath::Sin(FlickerTimer) * (LightFlickerRadiusMax - LightFlickerRadiusMin);
        float RightAttenuation = LightFlickerRadiusMin + FMath::Sin(FlickerTimer) * (LightFlickerRadiusMax - LightFlickerRadiusMin);

        float LeftIntensity = LightFlickerIntensityMin + FMath::Sin(FlickerTimer) * (LightFlickerIntensityMax - LightFlickerIntensityMin);
        float RightIntensity = LightFlickerIntensityMin + FMath::Sin(FlickerTimer) * (LightFlickerIntensityMax - LightFlickerIntensityMin);


        LeftTorchLightComponent->SetAttenuationRadius(LeftAttenuation);
        LeftTorchLightComponent->SetIntensity(LeftIntensity);
        
        RightTorchLightComponent->SetAttenuationRadius(RightAttenuation);
        RightTorchLightComponent->SetIntensity(RightIntensity);
        
        FLinearColor LeftColor = FLinearColor::LerpUsingHSV(LightColorFlickerStart, LightColorFlickerEnd, ColorInterop);
        FLinearColor RighColor = FLinearColor::LerpUsingHSV(LightColorFlickerStart, LightColorFlickerEnd, ColorInterop);

        LeftTorchLightComponent->SetLightColor(LeftColor);
        RightTorchLightComponent->SetLightColor(RighColor);
    }

    if (bToggleDoorOpenClose)
    {
        FRotator Rotation = DoorMeshComponent->GetRelativeTransform().GetRotation().Rotator();
        float CurrentYaw = Rotation.Yaw;

        if (bIsDoorOpen)
        {
            DoorYawRotation = FMath::FInterpConstantTo(CurrentYaw, DoorClosedRotationYaw, DeltaTime, 20.0f);
            Rotation.Yaw = DoorYawRotation;
            if (FMath::IsNearlyEqual(DoorYawRotation, DoorClosedRotationYaw, 3.1f))
            {
                bIsDoorOpen = false;
                bToggleDoorOpenClose = false;
				DoorMeshComponent->SetMobility(EComponentMobility::Static);
				DoorColliderBox->SetMobility(EComponentMobility::Static);
            }
            else
            {
				DoorMeshComponent->SetMobility(EComponentMobility::Movable);
				DoorColliderBox->SetMobility(EComponentMobility::Movable);
                DoorMeshComponent->SetRelativeRotation(Rotation);
            }
        }
        else
        {
			DoorYawRotation = CurrentYaw + (DoorRotationSpeed * DeltaTime);
			Rotation.Yaw = DoorYawRotation;

            if (FMath::IsNearlyEqual(DoorYawRotation, DoorOpenRotationYaw, 3.1f))
            {
                bIsDoorOpen = true;
                bToggleDoorOpenClose = false;
				DoorMeshComponent->SetMobility(EComponentMobility::Static);
				DoorColliderBox->SetMobility(EComponentMobility::Static);
            }
            else
            {
				DoorMeshComponent->SetMobility(EComponentMobility::Movable);
				DoorColliderBox->SetMobility(EComponentMobility::Movable);
                DoorMeshComponent->SetRelativeRotation(Rotation);
            }
        }
    }
}

/**
* Called when an instance of this class is placed (in editor) or spawned.
* @param	Transform			The transform the actor was constructed at.
*/
void AWallDoorwayConstructor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

	GenerateDoorWay();
}

void AWallDoorwayConstructor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    LeftTorchHolderComponent->AttachToComponent(DoorwayFrameMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
    LeftTorchLightComponent->AttachToComponent(LeftTorchHolderComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("FireLightSocket"));
    LeftTorchFireComponent->AttachToComponent(LeftTorchHolderComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("FireParticleSocket"));
   
    RightTorchHolderComponent->AttachToComponent(DoorwayFrameMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
    RightTorchLightComponent->AttachToComponent(RightTorchHolderComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("FireLightSocket"));
    RightTorchFireComponent->AttachToComponent(RightTorchHolderComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("FireParticleSocket"));

    LeftTorchLightComponent->SetLightColor(LightColorFlickerStart);
    RightTorchLightComponent->SetLightColor(LightColorFlickerStart);

    RightWallColliderBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    DoorColliderBox->AttachToComponent(DoorMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);

	GenerateDoorWay();
}

void AWallDoorwayConstructor::GenerateDoorWay()
{
	if (IsValid(DoorwayFrameMesh) && IsValid(DoorMesh))
	{
		if (IsValid(DoorwayFrameMeshComponent) && IsValid(DoorMeshComponent))
		{
			DoorwayFrameMeshComponent->SetMobility(EComponentMobility::Movable);
			DoorwayFrameMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			DoorwayFrameMeshComponent->SetStaticMesh(DoorwayFrameMesh);
			DoorwayFrameMeshComponent->SetRelativeTransform(DoorwayFrameTransformOffset);
			DoorwayFrameMeshComponent->SetMobility(EComponentMobility::Static);

			DoorMeshComponent->SetMobility(EComponentMobility::Movable);
			DoorMeshComponent->AttachToComponent(DoorwayFrameMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
			DoorMeshComponent->SetStaticMesh(DoorMesh);
			DoorMeshComponent->SetRelativeTransform(DoorTransformOffset);
			DoorMeshComponent->SetMobility(EComponentMobility::Static);

			for (int index = 0; index < WallBottoms.Num(); index++)
			{
				TWeakObjectPtr<UStaticMeshComponent> MeshComponent = WallBottoms[index];

				if (MeshComponent.IsValid() && MeshComponent->IsValidLowLevel())
				{
					const FString name1 = FString::Printf(TEXT("GridBottomX%i"), 1);
					const FString name2 = FString::Printf(TEXT("GridBottomX%i"), 2);

					const FString ComponentName = MeshComponent->GetName();

					if (ComponentName.Compare(name1) == 0 || ComponentName.Compare(name2) == 0)
					{
						MeshComponent->SetVisibility(false);
						MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

					}
				}
			}
			int32 size = GridMeshes.Num();

			for (int index = 0; index < size; index++)
			{
				TWeakObjectPtr<UStaticMeshComponent> MeshComponent = GridMeshes[index];

				if (MeshComponent.IsValid() && MeshComponent->IsValidLowLevel())
				{
					const FString name1 = FString::Printf(TEXT("GridPartX%iY%i"), 1, 0);
					const FString name2 = FString::Printf(TEXT("GridPartX%iY%i"), 2, 0);

					const FString name3 = FString::Printf(TEXT("GridPartX%iY%i"), 1, 1);
					const FString name4 = FString::Printf(TEXT("GridPartX%iY%i"), 2, 1);

					const FString ComponentName = MeshComponent->GetName();

					if (ComponentName.Compare(name1) == 0 || ComponentName.Compare(name2) == 0 ||
						ComponentName.Compare(name3) == 0 || ComponentName.Compare(name4) == 0)
					{
						MeshComponent->SetVisibility(false);
						MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

					}
				}
			}
		}
	}
	if (bUsesTorchLight)
	{
		if (IsValid(TorchLightComponent) && TorchLightComponent->IsValidLowLevel())
		{
			TorchLightComponent->SetVisibility(false);

		}
		if (IsValid(TorchFireComponent) && TorchFireComponent->IsValidLowLevel())
		{
			TorchFireComponent->SetVisibility(false);

		}
		if (IsValid(TorchHolderComponent) && TorchHolderComponent->IsValidLowLevel())
		{
			TorchHolderComponent->SetVisibility(false);

		}

	}

	LeftTorchHolderComponent->SetMobility(EComponentMobility::Movable);
	LeftTorchHolderComponent->SetRelativeTransform(TorchLeftComponentOffset);
	LeftTorchHolderComponent->SetMobility(EComponentMobility::Static);
	LeftTorchHolderComponent->SetVisibility(bUsesTorchLight);

	LeftTorchLightComponent->SetVisibility(bUsesTorchLight);

	LeftTorchFireComponent->SetVisibility(bUsesTorchLight);
	LeftTorchFireComponent->SetMobility(EComponentMobility::Static);

	RightTorchHolderComponent->SetMobility(EComponentMobility::Movable);
	RightTorchHolderComponent->SetRelativeTransform(TorchRightComponentOffset);
	RightTorchHolderComponent->SetVisibility(bUsesTorchLight);
	RightTorchHolderComponent->SetMobility(EComponentMobility::Static);

	RightTorchFireComponent->SetVisibility(bUsesTorchLight);
	RightTorchFireComponent->SetMobility(EComponentMobility::Static);

	RightTorchLightComponent->SetVisibility(bUsesTorchLight);

	LeftTorchLightComponent->SetLightColor(LightColorFlickerStart);
	RightTorchLightComponent->SetLightColor(LightColorFlickerStart);

	FVector WallColliderExtent;

	float WallColliderWidth = (GridSizeWidth  * ItemWidth);
	float WallColliderHeight = (GridSizeHieght * ItemHeight);

	if (UsesWallBottoms)
	{
		WallColliderHeight += WallBottomMeshHieght;
	}
	if (UsesWallTops)
	{
		WallColliderHeight += WallTopMeshHieght;
	}

	WallColliderWidth = WallColliderWidth / 5.0f;
	WallColliderHeight = WallColliderHeight / 2.0f;

	WallColliderHeight += WallColliderHeightOffset;

	switch (GridType)
	{
	case GridPlaneType::GRIDPLANE_XZ:
		WallColliderExtent = FVector(WallColliderWidth, 32.0f, WallColliderHeight);
		break;
	case GridPlaneType::GRIDPLANE_XY:
		WallColliderExtent = FVector(WallColliderWidth, WallColliderHeight, 32.0f);
		break;
	case GridPlaneType::DRIDPLANE_YZ:
		WallColliderExtent = FVector(32.0f, WallColliderWidth, WallColliderHeight);
		break;
	default:
		break;
	}
	FTransform WallColliderTransform = WallColliderBox->GetComponentTransform();
	WallColliderBox->SetBoxExtent(WallColliderExtent);
	WallColliderBox->SetMobility(EComponentMobility::Movable);
	WallColliderBox->SetRelativeLocation(FVector((WallColliderWidth - 100.0f) / 2.0f, 0.0f, WallColliderHeight - 100.0f));
	WallColliderBox->SetMobility(EComponentMobility::Static);

	RightWallColliderBox->SetBoxExtent(WallColliderExtent);
	RightWallColliderBox->SetRelativeLocation(FVector((WallColliderWidth * 7.1f) / 2.0f, 0.0f, WallColliderHeight - 100.0f));
	RightWallColliderBox->SetMobility(EComponentMobility::Static);

	DoorColliderBox->SetMobility(EComponentMobility::Movable);
	DoorColliderBox->SetBoxExtent(FVector(60, 10, 100));
	DoorColliderBox->SetRelativeLocation(FVector(60, 0, 0));
	DoorColliderBox->SetMobility(EComponentMobility::Static);
}
