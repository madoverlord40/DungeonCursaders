// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RootGameState.generated.h"


enum ROOT_STATES
{
	ROOT_FADE_IN,
	ROOT_WAIT,
	ROOT_FADE_OUT,
	ROOT_CHAR_SELECT,
	ROOT_LOAD_GAME,
	ROOT_SETTINGS,
	ROOT_SHUTDOWN,
	ROOT_NONE
};

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ARootGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	/** Called by game mode to set the started play bool */
	virtual void HandleBeginPlay() override;

	//~ Begin AActor Interface
	virtual void PostInitializeComponents() override;
	

public:
	ARootGameState();

	void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetStartGameState();

	UFUNCTION(BlueprintCallable)
	void SetSettingsState();

	UFUNCTION(BlueprintCallable)
	void ShutdownGame();

	void AdvanceState();

protected:
	ROOT_STATES CurrentState;
	ROOT_STATES NextState;
};
