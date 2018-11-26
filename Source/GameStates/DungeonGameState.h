// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DungeonGameState.generated.h"

class APlayerControllerBase;
class ADungeonGameMode;
class UDungeonCrusadesInstance;
class APlayerCharacterBase;
class APersistentLevelControler;

enum GAME_FLOW_STATES
{
	GAME_FLOW_INITIALIZE,
	LOAD_DUNGEON_MAP,
	START_GAME,
	GAME_IN_PROGRESS,
	RETURN_TO_ROOT,
	GAME_NONE
	
};

enum INITIALIZE_STATES
{
	Init_Start_State,
	Init_Execute_State,
	Init_Wait_State,
	Init_Exit_State
};

enum LOAD_MAP_STATES
{
	Load_Map_State,
	Load_Wait_State,
	Load_Exit_State
};

enum START_GAME_STATES
{
	Start_Game_State,
	Start_Wait_State,
	Start_Exit_State

};

enum GAME_PROGRESS_STATES
{
	Game_Progress_Starting,
	Game_Progress_Playing,
	Game_Progress_Paused,
	Game_Progress_Evaluate,
	Game_Progress_ZoneOut,
	Game_Progress_ZoneWait,
	Game_Progress_ZoneIn,
	Game_Progress_Won,
	Game_Progress_GameOver,
	Game_Progress_Exit
};
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ADungeonGameState : public AGameStateBase
{
	GENERATED_BODY()

	/** Called by game mode to set the started play bool */
	virtual void HandleBeginPlay() override;

	//~ Begin AActor Interface
	virtual void PostInitializeComponents() override;


public:
	ADungeonGameState();

	void Tick(float DeltaTime) override;

	void FinishPlayerIntroState();

	UFUNCTION(BlueprintCallable)
	void BeginZoneChange();

	void BeginSectionZoneIn();

	void NotifyMapLoaded() { IsMapLoaded = true; }

protected:
	void ProcessFlowStates(float time);
	void ProcessInitializeState(float time);
	void ProcessLoadMapState(float time);
	void ProcessStartGameState(float time);
	void ProcessGamePlayState(float time);

	UFUNCTION(BlueprintCallable)
	void StartGamePlayState();
	
private:
	GAME_FLOW_STATES CurrentFlowState;
	INITIALIZE_STATES InitializeState;
	LOAD_MAP_STATES LoadMapState;
	START_GAME_STATES StartGameState;
	GAME_PROGRESS_STATES GamePlayState;

	UPROPERTY()
	TWeakObjectPtr<ADungeonGameMode> DungeonGameMode;
	
	UPROPERTY()
	TWeakObjectPtr<APlayerCharacterBase> PlayerCharacter;

	UPROPERTY()
	TWeakObjectPtr<UDungeonCrusadesInstance> GameInstance;

	UPROPERTY()
	TWeakObjectPtr<APlayerControllerBase> PlayerController;

	UPROPERTY()
	TWeakObjectPtr<APersistentLevelControler> PersistentControler;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APlayerControllerBase> PlayerControllerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMapLoaded;

	uint32 bIsChangingZones : 1;
	uint32 bZoneChangeCompleted : 1;
};
