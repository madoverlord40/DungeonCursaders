// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomController.generated.h"

class ATorchLightActor;
class UBoxComponent;
class AEnemyCharacterBase;
class ALootableItemBase;
class ADungeonTreasureChest;
class ADungeonDoorActor;
class APlayerCharacterBase;

UCLASS()
class DUNGEONCRUSADES_API ARoomController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void VerifySpawnedEnemies();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Called when an actor is done spawning into the world (from UWorld::SpawnActor).
	* For actors with a root component, the location and rotation will have already been set.
	* Takes place after any construction scripts have been called
	*/
	virtual void PostActorCreated() override;

	/** Allow actors to initialize themselves on the C++ side */
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnRoomTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnRoomTriggerExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void RegisterTorch(ATorchLightActor* TorchActor);

	void NotifyEnemyDeath(const FName& EnemyKeyName);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemies)
	TMap<FName, TSubclassOf<AEnemyCharacterBase>> EnemySpawnClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemies)
	TMap<FName, FTransform> EnemySpawnTransforms;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Torchlights)
	TArray<ATorchLightActor*> TorchLights;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TreasureChests)
	TMap<FName, TSubclassOf<ADungeonTreasureChest>> DungeonChestClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TreasureChests)
	TMap<FName, FTransform> DungeonChestTransforms;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* RoomTriggerBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName RoomTriggerProfileName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000"))
	int BaseExpReward;

	UPROPERTY()
	TMap<FName, AEnemyCharacterBase*> SpawnedEnemies;

	UPROPERTY()
	TMap<FName, ADungeonTreasureChest*> SpawnedChests;

	uint8 EnemyDeathCounter;

	UPROPERTY()
	TWeakObjectPtr<APlayerCharacterBase> PlayerCharacter;
};
