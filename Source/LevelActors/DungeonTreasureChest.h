// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonTreasureChest.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UBoxComponent;
class USoundCue;
class UAudioComponent;
class UItemObject;

USTRUCT(BlueprintType)
struct FLootItemData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UItemObject> LootItemClass;

	UPROPERTY(EditAnywhere)
	uint32 WeightMin;

	UPROPERTY(EditAnywhere)
	uint32 WeightMax;

};

UCLASS()
class DUNGEONCRUSADES_API ADungeonTreasureChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonTreasureChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void PostActorCreated() override;

	void SetEnableChestOpen(bool open);

	void SetEnableChestHighlight(bool enable);

	const TMap<FName, FLootItemData>& GetLootItems() const { return LootableItemClasses; }

	void GetRandomLootItems(TMap<FName, TSubclassOf<UItemObject>>& InLootables);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ChestTopMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ChestBottomMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* ChestSoundComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* ChestCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundCue* ChestSpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundCue* ChestOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundCue* ChestCloseSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ChestParticleComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChestTopOpenRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChestTopClosedRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bIsChestOpen : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bPerformChestOpen : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bPerformChestClose : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 NumberOfLootRolls = 3;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, FLootItemData> LootableItemClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RandomWeightMax = 100;


private:
	float CurrentRotation;
	uint32 bHasLooted : 1;
};
