// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConstructorBase.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

// Sets default values
ALevelConstructorBase::ALevelConstructorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    CreatedRootComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("RootComponent"));
    SetRootComponent(CreatedRootComponent);
    
}

// Called when the game starts or when spawned
void ALevelConstructorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelConstructorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelConstructorBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
	GenerateGrid();
}

void ALevelConstructorBase::OnConstruction(const FTransform& Transform)
{
	if (IsValid(CreatedRootComponent) == false)
	{
		CreatedRootComponent = NewObject<UStaticMeshComponent>(this, FName("RootComponent"));
		SetRootComponent(CreatedRootComponent);
	}

	CreatedRootComponent->SetWorldTransform(Transform);

	GenerateGrid();
}

void ALevelConstructorBase::GenerateGrid()
{
	if (GridMeshes.Num() > 0)
	{
		for (int index = 0; index < GridMeshes.Num(); index++)
		{
			GridMeshes[index]->UnregisterComponent();
			GridMeshes[index]->DestroyComponent();
		}
		GridMeshes.Empty();
	}

	

	if (StaticMeshChunks.Num() > 0)
	{
		uint32 count = 0;
		for (uint8 x = 0; x < GridSizeWidth; ++x)
		{
			for (uint8 y = 0; y < GridSizeHieght; ++y)
			{
				FString ComponentName = FString::Printf(TEXT("GridPartX%iY%i"), x, y);
				UStaticMeshComponent* CreatedComponent = NewObject<UStaticMeshComponent>(this, FName(*ComponentName));
				if (IsValid(CreatedComponent))
				{
					CreatedComponent->RegisterComponent();

					CreatedComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

					FVector location;
					switch (GridType)
					{
					case GridPlaneType::GRIDPLANE_XZ:
						location = FVector(ItemWidth * x, 0.0f, ItemHeight * y);
						break;
					case GridPlaneType::GRIDPLANE_XY:
						location = FVector(ItemWidth * x, ItemHeight * y, 0.0f);
						break;
					case GridPlaneType::DRIDPLANE_YZ:
						location = FVector(0.0f, ItemWidth * x, ItemHeight * y);
						break;
					default:
						break;
					}

					location += AlignmentOffset;
					if (UsesRandomVariations)
					{
						int size = StaticMeshChunks.Num();
						uint32 wieghtedValue = 0;

						for (int index = 0; index < size; index++)
						{
							const FMeshChunkInfo* info = &StaticMeshChunks[index];
							wieghtedValue += info->MeshWeight;
						}
						uint32 random = FMath::RandRange(0, wieghtedValue);
						int32 SelectedRandomIndex = -1;

						for (int index = 0; index < size; index++)
						{
							const FMeshChunkInfo* info = &StaticMeshChunks[index];
							if (random < info->MeshWeight)
							{
								SelectedRandomIndex = index;
								break;
							}
							else
							{
								random -= info->MeshWeight;
							}
						}
						if (SelectedRandomIndex >= 0 && SelectedRandomIndex < StaticMeshChunks.Num())
						{
							CreatedComponent->SetStaticMesh(StaticMeshChunks[SelectedRandomIndex].MeshChunk);
						}
						else
						{
							CreatedComponent->SetStaticMesh(StaticMeshChunks[0].MeshChunk);
						}
					}
					else
					{
						CreatedComponent->SetStaticMesh(StaticMeshChunks[0].MeshChunk);
					}
					CreatedComponent->SetRelativeLocationAndRotation(location, Orientation);
					GridMeshes.Add(CreatedComponent);
					count++;

				}
			}
		}

	}
}
