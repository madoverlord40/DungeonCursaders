// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonTreasureChest.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"

// Sets default values
ADungeonTreasureChest::ADungeonTreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	ChestCollider = CreateDefaultSubobject<UBoxComponent>(FName("Chest Collider"));
	ChestCollider->SetupAttachment(RootComponent);

	ChestBottomMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("Chest Bottom Comp"));
	ChestBottomMeshComp->SetupAttachment(ChestCollider);

	ChestTopMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("Chest Top Comp"));
	ChestTopMeshComp->SetupAttachment(ChestBottomMeshComp);

	ChestParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(FName("Chest Particles Comp"));
	ChestParticleComp->SetupAttachment(ChestCollider);

	ChestSoundComp = CreateDefaultSubobject<UAudioComponent>(FName("Audio Comp"));
	ChestSoundComp->SetupAttachment(ChestCollider);

	CurrentRotation = 0.0f;
}

// Called when the game starts or when spawned
void ADungeonTreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonTreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPerformChestClose && bIsChestOpen)
	{
		CurrentRotation = FMath::FInterpConstantTo(CurrentRotation, ChestTopClosedRotation, DeltaTime, RotationSpeed);

		if (FMath::IsNearlyEqual(CurrentRotation, ChestTopClosedRotation))
		{
			bIsChestOpen = false;
			bPerformChestClose = false;
		}
		else
		{
			FRotator Rotation = ChestTopMeshComp->GetRelativeTransform().GetRotation().Rotator();

			Rotation.Roll = CurrentRotation;

			ChestTopMeshComp->SetRelativeRotation(Rotation);
		}
	}
	else if (bPerformChestOpen && !bIsChestOpen)
	{
		CurrentRotation = FMath::FInterpConstantTo(CurrentRotation, ChestTopOpenRotation, DeltaTime, RotationSpeed);

		if (FMath::IsNearlyEqual(CurrentRotation, ChestTopOpenRotation))
		{
			bIsChestOpen = true;
			bPerformChestOpen = false;
			ChestParticleComp->DeactivateSystem();
		}
		else
		{
			FRotator Rotation = ChestTopMeshComp->GetRelativeTransform().GetRotation().Rotator();

			Rotation.Roll = CurrentRotation;

			ChestTopMeshComp->SetRelativeRotation(Rotation);
		}
	}
}

void ADungeonTreasureChest::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//ChestCollider->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);
	
	ChestBottomMeshComp->AttachToComponent(ChestCollider, FAttachmentTransformRules::KeepRelativeTransform);

	ChestTopMeshComp->AttachToComponent(ChestBottomMeshComp, FAttachmentTransformRules::KeepRelativeTransform);

	ChestParticleComp->AttachToComponent(ChestCollider, FAttachmentTransformRules::KeepRelativeTransform);

	ChestSoundComp->AttachToComponent(ChestCollider, FAttachmentTransformRules::KeepRelativeTransform);

	ChestParticleComp->ActivateSystem();
}

void ADungeonTreasureChest::PostActorCreated()
{
	Super::PostActorCreated();

	if (IsValid(ChestSoundComp) && IsValid(ChestSpawnSound))
	{
		ChestSoundComp->SetSound(ChestSpawnSound);
		ChestSoundComp->Play();
	}
}

void ADungeonTreasureChest::SetEnableChestOpen(bool open)
{
	bPerformChestOpen = open;
	bPerformChestClose = !bPerformChestOpen;

	if (bIsChestOpen && IsValid(ChestOpenSound))
	{
		ChestSoundComp->SetSound(ChestOpenSound);
		ChestSoundComp->Play();
	}
	else if (!bIsChestOpen && IsValid(ChestCloseSound))
	{
		ChestSoundComp->SetSound(ChestCloseSound);
		ChestSoundComp->Play();
	}
}

void ADungeonTreasureChest::SetEnableChestHighlight(bool enable)
{
	ChestBottomMeshComp->SetRenderCustomDepth(enable);
	ChestTopMeshComp->SetRenderCustomDepth(enable);
}

void ADungeonTreasureChest::GetRandomLootItems(TMap<FName, TSubclassOf<UItemObject>>& InLootables)
{
	if (!bHasLooted)
	{
		while (InLootables.Num() < 2)
		{
			for (uint8 count = 0; count < NumberOfLootRolls; count++)
			{
				const uint32 random = FMath::RandRange(0, RandomWeightMax);
				for (const TPair<FName, FLootItemData> Pair : LootableItemClasses)
				{
					if (random > Pair.Value.WeightMin && random < Pair.Value.WeightMax)
					{
						InLootables.Add(Pair.Key, Pair.Value.LootItemClass);
					}
				}
			}
		}

		bHasLooted = true;
	}
}

