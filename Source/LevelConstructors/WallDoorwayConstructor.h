// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallConstructor.h"
#include "WallDoorwayConstructor.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AWallDoorwayConstructor : public AWallConstructor
{
	GENERATED_BODY()
	
	

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    AWallDoorwayConstructor();
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /**
    * Called when an instance of this class is placed (in editor) or spawned.
    * @param	Transform			The transform the actor was constructed at.
    */
    virtual void OnConstruction(const FTransform& Transform) override;

    virtual void PostInitializeComponents() override;
	
	void ToggleDoorOpenClose() { bToggleDoorOpenClose = !bToggleDoorOpenClose; }

	void GenerateDoorWay();

protected:
 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent* RightWallColliderBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent* DoorColliderBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UPointLightComponent* LeftTorchLightComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UPointLightComponent* RightTorchLightComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UParticleSystemComponent* LeftTorchFireComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UParticleSystemComponent* RightTorchFireComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* LeftTorchHolderComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* RightTorchHolderComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* DoorwayFrameMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* DoorMesh;
    
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* DoorwayFrameMeshComponent;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* DoorMeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FTransform DoorwayFrameTransformOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FTransform DoorTransformOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FTransform TorchLeftComponentOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FTransform TorchRightComponentOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DoorOpenRotationYaw;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DoorClosedRotationYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DoorRotationSpeed = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    uint32 bToggleDoorOpenClose : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    uint32 bIsDoorOpen : 1;

    float ColorInterop;

    float DoorYawRotation;

};
