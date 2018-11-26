// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "CharSelectPawnController.generated.h"


UENUM()
enum class CHARSLECTSTATES : uint8
{
	INIT,
	TRANSITION_IN,
	TRANSITION_OUT,
	MAIN,
	SELECTION_CHANGING,
	SELECTION_CHOSEN,
	SELECTION_VIEWING,
	SELECTION_VIEWING_EXIT,
	EXIT
};

UENUM()
enum class CHARSLECTSUBSTATES : uint8
{
	STATE_START,
	STATE_WAIT_PROGRESS,
	STATE_FINISH,
	STATE_EXIT
};

class APlayerCharacterBase;
class APlayerSelectionActor;
class ACharacterSelectPawn;
class ACharSelectPawnController;
class ACameraRig_Rail;
class UCharacterProfileWidget;
class ACharSelectGameState;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ACharSelectPawnController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	ACharSelectPawnController();
	/**
	* Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation().
	* PlayerTick is only called if the PlayerController has a PlayerInput object. Therefore, it will only be called for locally controlled PlayerControllers.
	*/
	virtual void PlayerTick(float DeltaTime) override;

	virtual void Possess(APawn* aPawn) override;
	virtual void UnPossess() override;
	virtual void PostInitializeComponents() override;
	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

	void MovePawnAcrossRail(const float RailPosition = 1.0f);
	
	UFUNCTION(BlueprintCallable)
	void SelectCurrentOption();

	UFUNCTION()
	void MoveRightOnRail(const float amount);

	void SetSelectionChangeComplete();

	UFUNCTION(BlueprintCallable)
	void ExitToRootScreen();

	UFUNCTION(BlueprintCallable)
	void ViewCharacterProfile();

	UFUNCTION(BlueprintCallable)
	void ExitCharacterProfile();

	void PlayIntoMovie();

	void SetCameraLookAt(TWeakObjectPtr<APlayerSelectionActor> LookAt);

protected:

	void ChangeSelectionLeft();
	void ChangeSelectionRight();
	void ConfigureRailMovement();
	/////////////////////////////////////////////////////////////////

	UPROPERTY()
	ULevelSequencePlayer* SequencePlayer;

	UPROPERTY(EditAnywhere, Category = "Sequence Settings")
	TMap<FString, ULevelSequence*> TransitionOutSequences;

	UPROPERTY(EditAnywhere, Category = "Sequence Settings")
	ULevelSequence* TransitionInSequence;

	UPROPERTY(EditAnywhere, Category = "Sequence Settings")
	FMovieSceneSequencePlaybackSettings PlaybackSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, float> CharacterSelectRailPositions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCharacterProfileWidget> CharacterProfileWidgetClass;

	UPROPERTY()
	FTimerHandle DelayTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RailInterpSpeed;

	TWeakObjectPtr<ACharacterSelectPawn> SelectionPawn;
	TWeakObjectPtr<ACameraRig_Rail> MainRailRig;
	TWeakObjectPtr<ACharSelectGameState> GameState;
	TWeakObjectPtr<UCharacterProfileWidget> ProfileWidget;

	float MoveToPosition;
	uint32 PerformRailMovement : 1;
	uint8 CharacterSelectIndex;
	float RailPosition;
	uint32 bSelectionChanged : 1;
	uint32 bSelectionChosen : 1;
	uint32 bIsPlayingSequence : 1;
	uint32 bIsCameraFading : 1;
	uint32 bIsProfileZooming : 1;
	uint32 bInProfileView : 1;
};
