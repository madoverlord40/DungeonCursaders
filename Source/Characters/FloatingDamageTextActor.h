// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextRenderActor.h"
#include "FloatingDamageTextActor.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AFloatingDamageTextActor : public ATextRenderActor
{
	GENERATED_BODY()
	
	
protected:

	/** Overridable native event for when play begins for this actor. */
	virtual void BeginPlay() override;

public:

	AFloatingDamageTextActor(const FObjectInitializer& ObjectInitializer);
	/**
	* Called when an actor is done spawning into the world (from UWorld::SpawnActor).
	* For actors with a root component, the location and rotation will have already been set.
	* Takes place after any construction scripts have been called
	*/
	virtual void PostActorCreated() override;

	/** Allow actors to initialize themselves on the C++ side */
	virtual void PostInitializeComponents() override;
	/**
	*	Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
	*	Note that Tick is disabled by default, and you will need to check PrimaryActorTick.bCanEverTick is set to true to enable it.
	*
	*	@param	DeltaSeconds	Game time elapsed during last frame modified by the time dilation
	*/
	virtual void Tick(float DeltaSeconds) override;

	void SetText(const FString& TextString);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShowTextMaxTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FloatingSpeed = 1.0f;


	float Timer = 0.0f;
};
