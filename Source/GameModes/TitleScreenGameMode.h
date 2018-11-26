// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleScreenGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ATitleScreenGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	//~=============================================================================
	// Initializing the game

	/**
	* Initialize the game.
	* The GameMode's InitGame() event is called before any other functions (including PreInitializeComponents() )
	* and is used by the GameMode to initialize parameters and spawn its helper classes.
	* @warning: this is called before actors' PreInitializeComponents.
	*/
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	/**
	* Initialize the GameState actor with default settings
	* called during PreInitializeComponents() of the GameMode after a GameState has been spawned
	* as well as during Reset()
	*/
	virtual void InitGameState();

	/** Start the transition out of the current map. Called at start of seamless travel, or right before map change for hard travel. */
	virtual void StartToLeaveMap();

	/** Transitions to calls BeginPlay on actors. */
	virtual void StartPlay();
	
	
};
