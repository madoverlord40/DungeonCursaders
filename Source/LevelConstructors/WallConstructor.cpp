// Fill out your copyright notice in the Description page of Project Settings.

#include "WallConstructor.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/PointLightComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"

const float FlickerSpeedMin = 1000.0f;
const float FlickerSpeedMax = 10000.0f;
float FlickerSpeed;

AWallConstructor::AWallConstructor() : Super()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    WallColliderBox = CreateDefaultSubobject<UBoxComponent>(FName("Wall Collider"));
    WallColliderBox->SetupAttachment(RootComponent);

    TorchHolderComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("TorchHolderMesh"));
    TorchHolderComponent->SetupAttachment(RootComponent);

    TorchLightComponent = CreateDefaultSubobject<UPointLightComponent>(FName("TorchLight"));
    TorchLightComponent->SetupAttachment(TorchHolderComponent, FName("FireLightSocket"));

    TorchFireComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("TorchFire"));
    TorchFireComponent->SetupAttachment(TorchHolderComponent, FName("FireParticleSocket"));
    
}

void AWallConstructor::BeginPlay()
{
    Super::BeginPlay();
	RadiusDelta = LightFlickerRadiusMin;
	IntensityDelta = LightFlickerIntensityMin;
	FlickerSpeed = FMath::FRandRange(FlickerSpeedMin, FlickerSpeedMax);
}

void AWallConstructor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bUsesTorchLight && bUsesLightFlicker)
    {
        if (IsTickingUp)
        {
			RadiusDelta = FMath::FInterpConstantTo(RadiusDelta, LightFlickerRadiusMax, DeltaTime, FlickerSpeed);
			IntensityDelta = FMath::FInterpConstantTo(IntensityDelta, LightFlickerIntensityMax, DeltaTime, FlickerSpeed);
			ColorInterop = FMath::FInterpConstantTo(ColorInterop, 1.0f, DeltaTime, 1.0f);

			if (FMath::IsNearlyEqual(RadiusDelta,LightFlickerRadiusMax) && FMath::IsNearlyEqual(IntensityDelta, LightFlickerIntensityMax))
            {
				FlickerSpeed = FMath::FRandRange(FlickerSpeedMin, FlickerSpeedMax);
                IsTickingUp = false;
            }


        }
        else
		{
			RadiusDelta = FMath::FInterpConstantTo(RadiusDelta, LightFlickerRadiusMin, -DeltaTime, -FlickerSpeed);
			IntensityDelta = FMath::FInterpConstantTo(IntensityDelta, LightFlickerIntensityMin, -DeltaTime, -FlickerSpeed);
			
			ColorInterop = FMath::FInterpConstantTo(ColorInterop, 0.0f, -DeltaTime, -1.0f);
            if (FMath::IsNearlyEqual(RadiusDelta, LightFlickerRadiusMin) && FMath::IsNearlyEqual(IntensityDelta, LightFlickerIntensityMin))
            {
				FlickerSpeed = FMath::FRandRange(FlickerSpeedMin, FlickerSpeedMax);
                IsTickingUp = true;
            }
            
        }
		 
        TorchLightComponent->SetAttenuationRadius(RadiusDelta);
        TorchLightComponent->SetIntensity(IntensityDelta);
		
        FLinearColor Color = FLinearColor::LerpUsingHSV(LightColorFlickerStart, LightColorFlickerEnd, ColorInterop);
        TorchLightComponent->SetLightColor(Color);
        
    }
}

void AWallConstructor::OnConstruction(const FTransform & Transform)
{
    RootComponent->SetWorldTransform(Transform);
	if (UsesWallBottoms)
	{
		ConstructWallBottom();

		AlignmentOffset.Z = WallBottomMeshHieght;
	}

	Super::OnConstruction(Transform);
	
	GenerateWall();
}

void AWallConstructor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    WallColliderBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    TorchHolderComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    TorchLightComponent->AttachToComponent(TorchHolderComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("FireLightSocket"));
    TorchFireComponent->AttachToComponent(TorchHolderComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("FireParticleSocket"));
    TorchLightComponent->SetLightColor(LightColorFlickerStart);

	if (UsesWallBottoms)
	{
		ConstructWallBottom();

		AlignmentOffset.Z = WallBottomMeshHieght;
	}

	GenerateWall();
}

void AWallConstructor::GenerateWall()
{
	
	ConstructWallTop();

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

	WallColliderWidth = (WallColliderWidth + 100.0f) / 2.0f;
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
	WallColliderBox->SetRelativeLocation(FVector((WallColliderWidth + 100.0f) / 2.0f, 0.0f, WallColliderHeight - 100.0f));
	WallColliderBox->SetMobility(EComponentMobility::Static);

	ConstructColumns();

	if (bUsesTorchLight)
	{
		FTransform TorchTransform = WallColliderBox->GetRelativeTransform();
		FVector TorchLocation = TorchTransform.GetLocation();
		TorchLocation.Y = TorchPositionOffset;
		TorchTransform.SetLocation(TorchLocation);
		if (IsValid(TorchHolderComponent))
		{
			TorchHolderComponent->SetMobility(EComponentMobility::Movable);
			TorchHolderComponent->SetRelativeTransform(TorchTransform);
			TorchHolderComponent->SetMobility(EComponentMobility::Static);

			TorchHolderComponent->SetVisibility(bUsesTorchLight);
			if (IsValid(TorchLightComponent))
			{
				TorchLightComponent->SetVisibility(bUsesTorchLight);
				
				if (IsValid(TorchFireComponent))
				{
					TorchFireComponent->SetVisibility(bUsesTorchLight);
					TorchFireComponent->SetMobility(EComponentMobility::Static);
				}
			}
		}
	}
}

void AWallConstructor::ConstructColumns()
{
    if (UsesLeftColumn)
    {
        if (IsValid(ColumnLeftBottomComponent) == false)
        {
			FString ComponentName = FString::Printf(TEXT("LeftColumnBottom"));
            ColumnLeftBottomComponent = NewObject<UStaticMeshComponent>(this, FName(*ComponentName));
            if (IsValid(ColumnLeftBottomComponent))
            {
				ColumnLeftBottomComponent->SetMobility(EComponentMobility::Movable);
                ColumnLeftBottomComponent->RegisterComponent();

                if (ColumnLeftBottomComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform))
                {
                    ColumnLeftBottomComponent->SetStaticMesh(ColumnBottomMesh);
                }
            }
        }
		
        ColumnLeftBottomComponent->SetRelativeLocation(LeftColumnOffset);
		ColumnLeftBottomComponent->SetMobility(EComponentMobility::Static);

        if (IsValid(ColumnLeftTopComponent) == false)
        {
			FString ComponentName = FString::Printf(TEXT("LeftColumnTop"));
            ColumnLeftTopComponent = NewObject<UStaticMeshComponent>(this, FName(*ComponentName));
            if (IsValid(ColumnLeftTopComponent))
            {
				ColumnLeftTopComponent->SetMobility(EComponentMobility::Movable);
                ColumnLeftTopComponent->RegisterComponent();

                if (ColumnLeftTopComponent->AttachToComponent(ColumnLeftBottomComponent, FAttachmentTransformRules::KeepRelativeTransform))
                {
                    ColumnLeftTopComponent->SetStaticMesh(ColumnTopMesh);
                }
            }
        }
        FVector ColumnTop(0.0f, 0.0f, ColumnTopHeightOffset);
		ColumnLeftTopComponent->SetRelativeLocation(ColumnTop);
		ColumnLeftTopComponent->SetMobility(EComponentMobility::Static);
    }
    else 
    {
        if (IsValid(ColumnLeftTopComponent))
        {
            ColumnLeftTopComponent->UnregisterComponent();
            ColumnLeftTopComponent->DestroyComponent();

        }

        if (IsValid(ColumnLeftBottomComponent))
        {
            ColumnLeftBottomComponent->UnregisterComponent();
            ColumnLeftBottomComponent->DestroyComponent();
        }
    }

    if (UsesRightColumn)
    {
        if (IsValid(ColumnRightBottomComponent) == false)
        {
			FString ComponentName = FString::Printf(TEXT("RightColumnBottom"));
            ColumnRightBottomComponent = NewObject<UStaticMeshComponent>(this, FName(*ComponentName));
            if (IsValid(ColumnRightBottomComponent))
            {
				ColumnRightBottomComponent->SetMobility(EComponentMobility::Movable);
                ColumnRightBottomComponent->RegisterComponent();

                if (ColumnRightBottomComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform))
                {
                    ColumnRightBottomComponent->SetStaticMesh(ColumnBottomMesh);
                }
            }
        }
        FVector RightColumnPosition(ItemWidth * GridSizeWidth, 0.0f, 0.0f);
        RightColumnPosition += RightColumnOffset;
		ColumnRightBottomComponent->SetRelativeLocation(RightColumnPosition);
		ColumnRightBottomComponent->SetMobility(EComponentMobility::Static);

        if (IsValid(ColumnRightTopComponent) == false)
        {
            FString ComponentName = FString::Printf(TEXT("RightColumnTop"));
            ColumnRightTopComponent = NewObject<UStaticMeshComponent>(this, FName(*ComponentName));
            if (IsValid(ColumnRightTopComponent))
            {
				ColumnRightTopComponent->SetMobility(EComponentMobility::Movable);
                ColumnRightTopComponent->RegisterComponent();

                if (ColumnRightTopComponent->AttachToComponent(ColumnRightBottomComponent, FAttachmentTransformRules::KeepRelativeTransform))
                {
                    ColumnRightTopComponent->SetStaticMesh(ColumnTopMesh);
                }
            }
        }
        FVector ColumnTop(0.0f, 0.0f, ColumnTopHeightOffset);
		ColumnRightTopComponent->SetRelativeLocation(ColumnTop);
		ColumnRightTopComponent->SetMobility(EComponentMobility::Static);
    }
    else
    {
        if (IsValid(ColumnRightTopComponent))
        {
            ColumnRightTopComponent->UnregisterComponent();
            ColumnRightTopComponent->DestroyComponent();

        }

        if (IsValid(ColumnRightBottomComponent))
        {
            ColumnRightBottomComponent->UnregisterComponent();
            ColumnRightBottomComponent->DestroyComponent();
        }
    }
}

void AWallConstructor::ConstructWallBottom()
{
    if (UsesWallBottoms && IsValid(WallBottomMesh))
    {
        if (WallBottoms.Num() > 0)
        {
            for (int x = 0; x < WallBottoms.Num(); x++)
            {
                TWeakObjectPtr<UStaticMeshComponent> mesh = WallBottoms[x];
                if (mesh.IsValid() && mesh->IsValidLowLevel())
                {
                    mesh->DestroyComponent();
                }
            }
            WallBottoms.Empty();
        }
        for (int index = 0; index < GridSizeWidth; index++)
        {
            FString ComponentName = FString::Printf(TEXT("GridBottomX%i"), index);
            UStaticMeshComponent* CreatedMeshComponent = NewObject<UStaticMeshComponent>(this, FName(*ComponentName));
            if (IsValid(CreatedMeshComponent))
            {
				CreatedMeshComponent->SetMobility(EComponentMobility::Movable);
                CreatedMeshComponent->RegisterComponent();

                if (CreatedMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform))
                {
                    CreatedMeshComponent->SetStaticMesh(WallBottomMesh);

                    FVector location;
                    switch (GridType)
                    {
                    case GridPlaneType::GRIDPLANE_XZ:
                        location = FVector(WallBottomMeshWidth * index, AlignmentOffset.Y, 0.0f);
                        break;
                    case GridPlaneType::GRIDPLANE_XY:
                        location = FVector(WallBottomMeshWidth * index, AlignmentOffset.Y, 0.0f);
                        break;
                    case GridPlaneType::DRIDPLANE_YZ:
                        location = FVector(0.0f, WallBottomMeshWidth * index, AlignmentOffset.Y);
                        break;
                    default:
                        break;
                    }
					CreatedMeshComponent->SetRelativeLocationAndRotation(location, Orientation);
					CreatedMeshComponent->SetMobility(EComponentMobility::Static);
                    WallBottoms.Add(CreatedMeshComponent);
                }
            }
        }
    }
    else
    {
        if (WallBottoms.Num() > 0)
        {
            for (int x = 0; x < WallBottoms.Num(); x++)
            {
                WallBottoms[x]->UnregisterComponent();
                WallBottoms[x]->DestroyComponent();
            }
            WallBottoms.Empty();
        }
    }
}

void AWallConstructor::ConstructWallTop()
{
    if (UsesWallTops && IsValid(WallTopMesh))
    {
        if (WallTops.Num() > 0)
        {
            for (int x = 0; x < WallTops.Num(); x++)
            {
                WallTops[x]->UnregisterComponent();
                WallTops[x]->DestroyComponent();
            }
            WallTops.Empty();
        }
        for (int index = 0; index < GridSizeWidth; index++)
        {
            FString ComponentName = FString::Printf(TEXT("GridTopX%i"), index);
            UStaticMeshComponent* CreatedMeshComponent = NewObject<UStaticMeshComponent>(this, FName(*ComponentName));
            if (IsValid(CreatedMeshComponent))
            {
				CreatedMeshComponent->SetMobility(EComponentMobility::Movable);
                CreatedMeshComponent->RegisterComponent();

                CreatedMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
               
                CreatedMeshComponent->SetStaticMesh(WallTopMesh);

                float positionZ = (GridSizeHieght * ItemHeight) + WallTopMeshHieght;
                FVector location;
                switch (GridType)
                {
                case GridPlaneType::GRIDPLANE_XZ:
                    location = FVector(WallTopMeshWidth * index, AlignmentOffset.Y, positionZ);
                    break;
                case GridPlaneType::GRIDPLANE_XY:
                    location = FVector(WallTopMeshWidth * index, AlignmentOffset.Y, positionZ);
                    break;
                case GridPlaneType::DRIDPLANE_YZ:
                    location = FVector(positionZ, WallTopMeshWidth * index, AlignmentOffset.Y);
                    break;
                default:
                    break;
                }
				CreatedMeshComponent->SetRelativeLocationAndRotation(location, Orientation);
				CreatedMeshComponent->SetMobility(EComponentMobility::Static);
                WallTops.Add(CreatedMeshComponent);
            }
        }
    }
    else
    {
        if (WallTops.Num() > 0)
        {
            for (int x = 0; x < WallTops.Num(); x++)
            {
                WallTops[x]->UnregisterComponent();
                WallTops[x]->DestroyComponent();
            }
            WallTops.Empty();
        }
    }
}
