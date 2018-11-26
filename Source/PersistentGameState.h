// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PersistentGameState.generated.h"


UENUM()
enum GAMEFLOWSTATES
{
	GAME_INITIALIZE,
	GAME_TITLE_SCREEN,
	GAME_CHARACTER_SELECT,
	GAME_TRANSITION_IN,
	GAME_TRANSITION_OUT,
	GAME_PLAY,
	GAME_RETURN_MENU,
	GAME_SHUTDOWN,
	GAME_EXIT
};

UENUM()
enum GAMEFLOW_SUBSTATES
{
	STATE_START,
	STATE_INPROGRESS,
	STATE_CHANGESTATE,
	STATE_LOAD,
	STATE_POSTLOAD,
	STATE_UNLOAD,
	STATE_ENDSTATE,
	STATE_WAIT

};

class ATitleScreenCameraPawn;
class ATitlePawnController;
class ACharacterSelectPawn;
class ACharSelectPawnController;
class APlayerCharacterBase;
class APlayerControllerBase;
class APersistentGameMode;
class AGamePlayerState;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API APersistentGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	/** Add PlayerState to the PlayerArray */
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	/** Remove PlayerState from the PlayerArray. */
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	/** Called by game mode to set the started play bool */
	virtual void HandleBeginPlay() override;

	//~ Begin AActor Interface
	virtual void PostInitializeComponents() override;

public:
	APersistentGameState();

	void Tick(float DeltaTime) override;

	void SetCurrentState(GAMEFLOWSTATES state);

	UFUNCTION(BlueprintCallable)
	void AdvanceNextState();

	void SetSelectedCharacterIndex(const FString& KeyName);

	UFUNCTION(BlueprintCallable)
	void PostGameMapLoad();
	
protected:
	void ExecuteGameState(float DeltaTime);
	void ExecuteInitializeState(float DeltaTime);
	void ExecuteTitleScreenState(float DeltaTime);
	void ExecuteCharSelectState(float DeltaTime);
	void ExecutePlayState(float DeltaTime);
	void ExecuteTransitionInState(float DeltaTime);

	GAMEFLOWSTATES CurrentGameState;
	GAMEFLOWSTATES NextGameState;
	GAMEFLOW_SUBSTATES CurrentState;
	GAMEFLOW_SUBSTATES NextState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsGameLoaded;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, TSubclassOf<APlayerCharacterBase>> PlayerClasses;

	UPROPERTY()
	TWeakObjectPtr<ATitleScreenCameraPawn> TitleScreenPawn;

	UPROPERTY()
	TWeakObjectPtr<ATitlePawnController> TitleScreenController;

	UPROPERTY()
	TWeakObjectPtr<ACharacterSelectPawn> CharSelectPawn;

	UPROPERTY()
	TWeakObjectPtr<ACharSelectPawnController> CharSelectController;

	UPROPERTY()
	TWeakObjectPtr<APlayerCharacterBase> PlayerCharacter;

	UPROPERTY()
	TWeakObjectPtr<APlayerControllerBase> PlayerController;

	UPROPERTY()
	TWeakObjectPtr<APersistentGameMode> PersistentGameMode;

	UPROPERTY()
	TWeakObjectPtr<AGamePlayerState> GamePlayerState;

	TWeakObjectPtr<APlayerController> DefaultController;
	uint32 bMapLoadCompleted : 1;
	uint8 CharacterSelectIndex;;
};
