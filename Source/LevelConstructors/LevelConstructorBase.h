// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelConstructorBase.generated.h"

UENUM(BlueprintType)
enum class GridPlaneType : uint8
{
    GRIDPLANE_XZ,
    GRIDPLANE_XY,
    DRIDPLANE_YZ
};

USTRUCT(BlueprintType)
struct FMeshChunkInfo
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere)
    UStaticMesh* MeshChunk;

    UPROPERTY(EditAnywhere)
    uint8 MeshWeight;

};

UCLASS()
class DUNGEONCRUSADES_API ALevelConstructorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelConstructorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void PostInitializeComponents() override;

    void OnConstruction(const FTransform& Transform) override;

	void GenerateGrid();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMeshChunkInfo> StaticMeshChunks;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool UsesRandomVariations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    uint8 GridSizeWidth;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    uint8 GridSizeHieght;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ItemWidth;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ItemHeight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector AlignmentOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRotator Orientation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    GridPlaneType GridType;

    TArray<UStaticMeshComponent*> GridMeshes;

    UPROPERTY()
    UStaticMeshComponent* CreatedRootComponent;
};
