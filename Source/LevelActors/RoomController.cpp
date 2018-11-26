// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomController.h"
#include "TorchLightActor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Characters/Players/PlayerCharacterBase.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "Characters/Enemies/EnemyCharacterBase.h"
#include "Characters/Enemies/Controllers/EnemyAIControllerBase.h"
#include "../Characters/Enemies/LootItems/ExperienceDropActor.h"
#include "../Characters/Enemies/LootItems/ModifyStatsDrop.h"
#include "../Characters/Enemies/LootItems/WeaponUpgradeDrop.h"
#include "DungeonTreasureChest.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ARoomController::ARoomController() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RoomTriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("Room Trigger"));
	RoomTriggerBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARoomController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoomController::VerifySpawnedEnemies()
{
	if (SpawnedEnemies.Num() > 0)
	{
		//despawn enemies that are dead and/or not aggro player
		//TODO:loop through keys instead of pairs because some are sometimes invalid
		TArray<FName> KeyNames;
		SpawnedEnemies.GetKeys(KeyNames);

		for (int index = 0; index < KeyNames.Num(); index++)
		{
			const FName& Key = KeyNames[index];

			AEnemyCharacterBase** FoundEntry = SpawnedEnemies.Find(Key);
			if (FoundEntry != nullptr)
			{
				AEnemyCharacterBase* enemy = *FoundEntry;

				if (IsValid(enemy) && enemy->IsValidLowLevel())
				{
					TWeakObjectPtr<AEnemyAIControllerBase> EnemyAI = Cast<AEnemyAIControllerBase>(enemy->GetController());

					if (EnemyAI.IsValid() && EnemyAI->IsValidLowLevel())
					{
						if (!enemy->GetIsEnemyAlive() || (enemy->GetIsEnemyAlive() && !EnemyAI->GetAIConfig()->HasTargetSighted))
						{
							EnemyAI->UnPossess();
							EnemyAI->Destroy();
							enemy->Destroy();
							SpawnedEnemies.Remove(Key);
						}
					}
				}
				
			}
		}
	}
}

// Called every frame
void ARoomController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomController::PostActorCreated()
{
	Super::PostActorCreated();
	
	
}

void ARoomController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RoomTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ARoomController::OnRoomTriggerOverlap);
	RoomTriggerBox->OnComponentEndOverlap.AddDynamic(this, &ARoomController::OnRoomTriggerExitOverlap);

	RoomTriggerBox->SetGenerateOverlapEvents(true);
	RoomTriggerBox->SetCollisionProfileName(RoomTriggerProfileName);
	RoomTriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ARoomController::OnRoomTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsValid(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
		PlayerCharacter = Cast<APlayerCharacterBase>(OtherActor);

		if (PlayerCharacter.IsValid())
		{
			VerifySpawnedEnemies();

			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(this, APlayerControllerBase::StaticClass(), FoundActors);

			TWeakObjectPtr<APlayerControllerBase> PlayerController = Cast<APlayerControllerBase>(FoundActors[0]);

			if (EnemySpawnClasses.Num() > 0 && EnemySpawnTransforms.Num() == EnemySpawnClasses.Num())
			{
				//spawn enemies
				for (const TPair<FName, TSubclassOf<AEnemyCharacterBase>>& Pair : EnemySpawnClasses)
				{
					const FName KeyName = Pair.Key;
					const TSubclassOf<AEnemyCharacterBase> EnemyClass = Pair.Value;

					FTransform SpawnTransform = *EnemySpawnTransforms.Find(KeyName);
					const FVector RoomLocation = GetActorLocation();
					const FVector Location = SpawnTransform.GetLocation();
					const FVector SpawnLocation = RoomLocation + Location;

					if (SpawnedEnemies.Find(KeyName) == nullptr)
					{
						TWeakObjectPtr<AEnemyCharacterBase> enemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(EnemyClass, FTransform::Identity);

						if (enemy.IsValid())
						{
							enemy->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
							enemy->SetActorRelativeTransform(SpawnTransform);
							enemy->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
							TWeakObjectPtr<AEnemyAIControllerBase> EnemyAI = Cast<AEnemyAIControllerBase>(enemy->GetController());
							EnemyAI->UpdateOriginAI();
							enemy->SetRoomController(this, KeyName);
							
							if (PlayerController.IsValid())
							{
								const uint8 level = PlayerController->GetCharacterCombatData()->GetCharacterLevel();
								EnemyAI->SetCurrentLevel(level);
							}
							SpawnedEnemies.Add(KeyName, enemy.Get());
						}
					}
				}
			}
			for (int i = 0; i < TorchLights.Num(); i++)
			{
				TWeakObjectPtr<ATorchLightActor> TorchActor = TorchLights[i];

				if (TorchActor.IsValid())
				{
					TorchLights[i]->SetEnableLight(true, true);
				}
			}
			if (DungeonChestClasses.Num() == DungeonChestTransforms.Num())
			{
				for (TPair<FName, TSubclassOf<ADungeonTreasureChest>>& Pairs : DungeonChestClasses)
				{
					if (IsValid(Pairs.Value))
					{
						if (DungeonChestTransforms.Find(Pairs.Key))
						{
							FTransform SpawnTransform = *DungeonChestTransforms.Find(Pairs.Key);
							ADungeonTreasureChest* Chest = GetWorld()->SpawnActor<ADungeonTreasureChest>(Pairs.Value, SpawnTransform);
							
							SpawnedChests.Add(Pairs.Key, Chest);
						}
					}
					
				}
			}
		}
	}
}

void ARoomController::OnRoomTriggerExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		PlayerCharacter = Cast<APlayerCharacterBase>(OtherActor);

		if (PlayerCharacter.IsValid() && SpawnedEnemies.Num() > 0)
		{			
			VerifySpawnedEnemies();
		}
		if (SpawnedEnemies.Num() == 0)
		{
			for (int i = 0; i < TorchLights.Num(); i++)
			{
				ATorchLightActor* Torch = TorchLights[i];

				if (IsValid(Torch))
				{
					Torch->SetEnableLight(false, false);
				}
			}
		}
	}
}

void ARoomController::RegisterTorch(ATorchLightActor* TorchActor)
{
	if (IsValid(TorchActor) && TorchActor->IsValidLowLevel())
	{
		TorchLights.Add(TorchActor);
	}
}

void ARoomController::NotifyEnemyDeath(const FName& EnemyKeyName)
{
	const TWeakObjectPtr<AEnemyCharacterBase> Enemy = *SpawnedEnemies.Find(EnemyKeyName);

	if (Enemy.IsValid() && Enemy->IsValidLowLevel())
	{
		EnemyDeathCounter++;

		// spawn treasure??

		const uint8 level = Enemy->GetCharacterCombatData()->GetCharacterLevel();

		const uint32 reward = ((BaseExpReward * level) * 0.5f);

		if (PlayerCharacter.IsValid())
		{
			PlayerCharacter->AddRewardExp(reward);
		}

	}
}

