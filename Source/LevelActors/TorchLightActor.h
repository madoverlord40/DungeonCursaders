// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchLightActor.generated.h"

class UPointLightComponent;
class UParticleSystemComponent;


UCLASS()
class DUNGEONCRUSADES_API ATorchLightActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATorchLightActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	UFUNCTION(BlueprintCallable)
	void SetEnableLight(bool enabled, bool flickering);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPointLightComponent* TorchLightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TorchMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* FireParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* TorchMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* FireParticleTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bUseLightFlicker : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bLightEnabled : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flicker Configuration")
	FLinearColor FlickerStartColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flicker Configuration")
	FLinearColor FlickerEndColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flicker Configuration")
	float LightRadiusMin = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flicker Configuration")
	float LightRadiusMax = 10000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flicker Configuration")
	float LightIntensityMin = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flicker Configuration")
	float LightIntensityMax = 100.0f;



};
