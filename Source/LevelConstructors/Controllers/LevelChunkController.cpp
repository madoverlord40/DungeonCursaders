// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelChunkController.h"
#include "../WallDoorwayConstructor.h"
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Characters/Enemies/EnemyCharacterBase.h"
#include "Characters/Players/PlayerCharacterBase.h"
#include "PersistentGameState.h"

// Sets default values
ALevelChunkController::ALevelChunkController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	BoxTriggerComponent = CreateDefaultSubobject<UBoxComponent>(FName("Area Trigger"));
	BoxTriggerComponent->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ALevelChunkController::BeginPlay()
{
	Super::BeginPlay();

	
	TWeakObjectPtr<APersistentGameState> GameState = Cast<APersistentGameState>(GetWorld()->GetGameState());

	if (GameState.IsValid())
	{
		const ALevelChunkController* ThisChunk = &(*this);
		//GameState->AddLevelChunkToMap(&ThisChunk);
	}
}

// Called every frame
void ALevelChunkController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelChunkController::PostInitProperties()
{
	Super::PostInitProperties();

	
}

void ALevelChunkController::PostActorCreated()
{
	Super::PostActorCreated();
	
}

void ALevelChunkController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BoxTriggerComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	BoxTriggerComponent->SetCollisionProfileName(FName("RoomControllerProfile"));
	BoxTriggerComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//BoxTriggerComponent->bGenerateOverlapEvents = true;
	BoxTriggerComponent->SetBoxExtent(BoxExtents);
	
	BoxTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelChunkController::OnBeginBoxOverlap);
	BoxTriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ALevelChunkController::OnEndBoxOverlap);
}

void ALevelChunkController::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
	
}

void ALevelChunkController::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	SetActorTransform(Transform);	
}

void ALevelChunkController::OnNotifyEnemyDeath(const FName EnemyObjectName)
{
	if (SpawnedEnemies.FindChecked(EnemyObjectName) != nullptr)
	{
		++EnemyKillCounter;

		CheckRequirementsCompleted();

		if (EnemyKillCounter == SpawnedEnemies.Num())
		{
			TArray<AEnemyCharacterBase*> Enemies;
			SpawnedEnemies.GenerateValueArray(Enemies);

			for (int index = 0; index < Enemies.Num(); index++)
			{
				//Enemies[index]->StartTimedDestroy();
			}

			SpawnedEnemies.Empty();
		}
	}
	
}

void ALevelChunkController::OnNotifyPuzzleCompleted(const FName& PuzzleObjectName)
{
	CheckRequirementsCompleted();
}

bool ALevelChunkController::CheckRequirementsCompleted()
{
	bool success = false;

	uint32 TotalRequirements = ClearRequirements.Num();
	uint32 TotalCompleted = 0;

	for (uint32 index = 0; index < TotalRequirements; index++)
	{
		LevelClearRequiementsType Type = ClearRequirements[index];

		switch (Type)
		{
		case LevelClearRequiementsType::REQUIREMENT_NONE:
			break;
		case LevelClearRequiementsType::REQUIREMENT_KILLALL:
		case LevelClearRequiementsType::REQUIREMENT_KILLENEMIES:
			if (EnemyKillCounter == EnemyKillsNeeded)
			{
				++TotalCompleted;
			}
			break;
		case LevelClearRequiementsType::REQUIREMENT_KEYITEM:
			break;
		case LevelClearRequiementsType::REQUIREMENT_PUZZLE:
			break;
		default:
			break;
		}

		
	}

	if (IsValid(DoorwayToOpen) && TotalCompleted == TotalRequirements)
	{
		if (!bHasRoomBeenCleared)
		{
			DoorwayToOpen->ToggleDoorOpenClose();
			SpawnRewards();
			bHasRoomBeenCleared = 1;
			BoxTriggerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		

		success = true;
	}

	return success;
}

void ALevelChunkController::OnBeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	TWeakObjectPtr<APlayerCharacterBase> Player = Cast<APlayerCharacterBase>(OtherActor);

	if (Player.IsValid() && Player->IsValidLowLevel())
	{
		TArray<FName> EnemyNames;
		EnemyClassesToSpawn.GetKeys(EnemyNames);

		int32 class_count = EnemyClassesToSpawn.Num();
		int32 location_count = EnemySpawnTransforms.Num();

		if (SpawnedEnemies.Num() > 0)
		{
			TArray<FName> SpawnedEnemyNames;
			SpawnedEnemies.GetKeys(SpawnedEnemyNames);

			if (location_count == class_count) {

				for (int32 x = 0; x < class_count; ++x)
				{
					const FName name = EnemyNames[x];
					const TSubclassOf<AEnemyCharacterBase> SpawnClass = EnemyClassesToSpawn.FindChecked(name);
					const FTransform location = EnemySpawnTransforms.FindChecked(name);

					bool IsSpawned = false;

					for (int32 y = 0; y < SpawnedEnemyNames.Num(); y++)
					{
						if (name.Compare(SpawnedEnemyNames[y]))
						{
							IsSpawned = true;
							break;
						}

					}

					if (!IsSpawned)
					{
						AEnemyCharacterBase* enemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(SpawnClass, location);
						if (IsValid(enemy)) {
							//enemy->SetLevelChunkOwner(this);
							//enemy->SetSpawnedMapName(name);
							SpawnedEnemies.Add(name, enemy);
						}
					}
				}
			}
		}
		else
		{
			if (location_count == class_count) {
				for (int32 x = 0; x < class_count; ++x)
				{
					const FName name = EnemyNames[x];
					const TSubclassOf<AEnemyCharacterBase> SpawnClass = EnemyClassesToSpawn.FindChecked(name);
					const FTransform location = EnemySpawnTransforms.FindChecked(name);

					AEnemyCharacterBase* enemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(SpawnClass, location);
					if (IsValid(enemy)) {
						//enemy->SetLevelChunkOwner(this);
						//enemy->SetSpawnedMapName(name);
						SpawnedEnemies.Add(name, enemy);
					}
				}
			}
		}
	}
}

void ALevelChunkController::OnEndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TWeakObjectPtr<APlayerCharacterBase> Player = Cast<APlayerCharacterBase>(OtherActor);

	if (Player.IsValid() && Player->IsValidLowLevel())
	{
		if (SpawnedEnemies.Num() > 0)
		{
			TArray<FName> KeyNames;
			SpawnedEnemies.GetKeys(KeyNames);

			for (int32 x = 0; x < KeyNames.Num(); x++)
			{
				const FName Key = KeyNames[x];
				TWeakObjectPtr<AEnemyCharacterBase> Enemy = SpawnedEnemies.FindChecked(Key);
				if (Enemy.IsValid() && Enemy->IsValidLowLevel() && !Enemy->GetIsEnemyAlive())
				{
					Enemy->Destroy();
					SpawnedEnemies.FindAndRemoveChecked(Key);
					EnemyKillCounter--;
				}
			}
		}
	}
}

void ALevelChunkController::ResetRoom()
{
	EnemyKillCounter = 0;
	bHasRoomBeenCleared = 0;
	BoxTriggerComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ALevelChunkController::SpawnRewards()
{

}

