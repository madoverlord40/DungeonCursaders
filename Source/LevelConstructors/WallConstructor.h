// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelConstructorBase.h"
#include "WallConstructor.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AWallConstructor : public ALevelConstructorBase
{
	GENERATED_BODY()
	

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    AWallConstructor();
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /**
    * Called when an instance of this class is placed (in editor) or spawned.
    * @param	Transform			The transform the actor was constructed at.
    */
    virtual void OnConstruction(const FTransform& Transform) override;

    virtual void PostInitializeComponents() override;

	void GenerateWall();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent* WallColliderBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UPointLightComponent* TorchLightComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UParticleSystemComponent* TorchFireComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* TorchHolderComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* WallTopMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* WallBottomMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* ColumnBottomMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* ColumnTopMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* ColumnLeftTopComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* ColumnLeftBottomComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* ColumnRightTopComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* ColumnRightBottomComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WallTopMeshWidth;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WallTopMeshHieght;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WallBottomMeshHieght;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WallBottomMeshWidth;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WallColliderHeightOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool UsesWallBottoms;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool UsesWallTops;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool UsesLeftColumn;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool UsesRightColumn;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector LeftColumnOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector RightColumnOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ColumnTopHeightOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TorchPositionOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* TorchLightHolderMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UParticleSystem* TorchFireParticle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    uint32 bUsesTorchLight : 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    uint32 bUsesLightFlicker : 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LightFlickerRadiusMin;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LightFlickerRadiusMax;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LightFlickerIntensityMin;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LightFlickerIntensityMax;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FLinearColor LightColorFlickerStart;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FLinearColor LightColorFlickerEnd;

    TArray<UStaticMeshComponent*> WallTops;
    TArray<UStaticMeshComponent*> WallBottoms;

    void ConstructColumns();
    void ConstructWallBottom();
    void ConstructWallTop();

	UPROPERTY()
    bool IsTickingUp = true;

	UPROPERTY()
    float RadiusDelta;

	UPROPERTY()
	float IntensityDelta;

	UPROPERTY()
    float ColorInterop = 0.75f;
	
	UPROPERTY()
	float FlickerTimer;
};
