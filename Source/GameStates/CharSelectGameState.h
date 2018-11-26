// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CharSelectGameState.generated.h"

enum CHARSELECT_STATES
{
	Initialize,
	Start_Cinematic,
	End_Cinematic,
	User_Wait,
	Show_Profile,
	Exit_Profile,
	Transition_Game,
	Transition_Root,
	Load_Game,
	Load_Title,
	None	
};

enum CHARSELECT_SUBSTATES
{
	StartState,
	ExecuteState,
	WaitState,
	ExitState
};

class UDungeonCrusadesInstance;
class UCombatData;
class ACharSelectPawnController;
class APlayerCharacterBase;
class APlayerSelectionActor;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ACharSelectGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	
	/** Called by game mode to set the started play bool */
	virtual void HandleBeginPlay() override;

	//~ Begin AActor Interface
	virtual void PostInitializeComponents() override;

public:
	ACharSelectGameState();

	void Tick(float DeltaTime) override;

	void AdvanceState();

	UFUNCTION(BlueprintCallable)
	void SetCurrentSelection(const FString& KeyName);

	UFUNCTION(BlueprintCallable)
	void ChooseCurrentSelection();

	UFUNCTION(BlueprintCallable)
	void ExitToRoot();

	UFUNCTION(BlueprintCallable)
	void ShowCharacterProfile(bool show);

	void GetSelectionKeyNames(TArray<FString>& KeyNames);

	void SetEnablePawnLight(const FString& KeyName, bool enable);

	const UCombatData* GetCombatDataByName(const FString& KeyName);

protected:
	void SpawnSelectionPawns();
	void ProcessCurrentState(float time);

	void ProcessInitState(float time);
	void ProcessCinematicState(float time);
	void ProcessPostCinemaState(float time);
	void ProcessStartGameState(float time);
	void ProcessExitToTitleState(float time);
	void ProcessShowProfileState(float time);
	void ProcessExitProfileState(float time);


protected:
	CHARSELECT_STATES CurrentState;
	CHARSELECT_STATES NextState;
	CHARSELECT_SUBSTATES CurrentSubState;
	FString SelectedCharacterKey;

	UPROPERTY()
	TWeakObjectPtr<ACharSelectPawnController> PlayerController;

	UPROPERTY()
	TWeakObjectPtr<UDungeonCrusadesInstance> GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, TSubclassOf<APlayerCharacterBase>> PlayerClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, TSubclassOf<APlayerSelectionActor>> PlayerSelectionClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, TSubclassOf<UCombatData>> CombatDataClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, FTransform> PlayerSelectionLocations;

	UPROPERTY()
	TMap<FString, APlayerSelectionActor*> SpawnedPlayerSelections;

};
