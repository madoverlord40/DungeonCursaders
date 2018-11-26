// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerSelectionActor.generated.h"

class USkeletalMeshComponent;
class UPlayerSelectActorAnim;
class UCapsuleComponent;
class USpotLightComponent;

UCLASS()
class DUNGEONCRUSADES_API APlayerSelectionActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerSelectionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	void SetEnableLight(bool enable);
	void SetSelectedForPlay();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* RootCapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpotLightComponent* SpotLight;

	TWeakObjectPtr<UPlayerSelectActorAnim> AnimationComponent;

	
};
