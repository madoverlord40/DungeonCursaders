// Fill out your copyright notice in the Description page of Project Settings.

#include "PersistentLevelControler.h"
#include "GameStates/DungeonGameState.h"
#include "GameModes/DungeonGameMode.h"
#include "DungeonCrusadesInstance.h"
#include "Characters/Players/PlayerCharacterBase.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "Characters/Players/Components/InventoryComponent.h"
#include "Characters/Players/UI/DoorInteractWindow.h"
#include "GameStates/DungeonGameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"
#include "Runtime/CinematicCamera/Public/CineCameraActor.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"

APersistentLevelControler::APersistentLevelControler(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APersistentLevelControler::BeginPlay()
{
	Super::BeginPlay();
}

void APersistentLevelControler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsPlayingMovie)
	{
		if (IntroSequencePlayer.IsValid() && IntroSequencePlayer->IsPlaying() == false)
		{
			SequenceActor->Destroy();

			if (GameState.IsValid())
			{
				GameState->FinishPlayerIntroState();
				bIsPlayingMovie = false;
			}
		}
	}
}

void APersistentLevelControler::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GameState = Cast<ADungeonGameState>(GetWorld()->GetGameState());
	
}

void APersistentLevelControler::PlayIntroMovieSequence()
{
	if (IsValid(IntroSequenceClip) && !bIsPlayingMovie)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bDisableLookAtInput = true;
		Settings.bDisableMovementInput = true;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.ObjectFlags |= RF_Transient;
		SpawnParams.bAllowDuringConstructionScript = true;
		SequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(SpawnParams);

		SequenceActor->PlaybackSettings = Settings;
		SequenceActor->LevelSequence = IntroSequenceClip;
		SequenceActor->InitializePlayer();

		IntroSequencePlayer = SequenceActor->SequencePlayer;

		TArray<AActor*> actors;

		UGameplayStatics::GetAllActorsOfClass(this, ACineCameraActor::StaticClass(), actors);

		if (actors.Num() > 0)
		{
			TWeakObjectPtr<ACineCameraActor> MovieCamera = Cast<ACineCameraActor>(actors[0]);

			if (MovieCamera.IsValid())
			{
				GetWorld()->GetFirstPlayerController()->SetViewTarget(MovieCamera.Get());
				IntroSequencePlayer->Play();
				bIsPlayingMovie = true;
			}
		}
	}
}

bool APersistentLevelControler::LoadFloorMap(const FName& LevelName)
{
	TWeakObjectPtr<ULevelStreaming> level = UGameplayStatics::GetStreamingLevel(GetWorld(), LevelName);
	
	if (level.IsValid())
	{
		CurrentMapName = LevelName;

		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.Linkage = 0;
		LatentAction.UUID = 1;
		LatentAction.ExecutionFunction = FName("PostMapLoad");

		ULevel* LoadedLevel = GetWorld()->GetCurrentLevel();

		UGameplayStatics::LoadStreamLevel(LoadedLevel, LevelName, true, false, LatentAction);
		return true;
	}

	return false;
}

bool APersistentLevelControler::LoadNextFloorSection()
{
	if (!bChangingMaps)
	{
		CurrentFloorIndex++;

		UnloadCurrentSection();

		return true;
	}

	return false;
}

bool APersistentLevelControler::LoadFirstSection()
{
	if (!bChangingMaps)
	{
		CurrentFloorIndex = 0;
		bChangingMaps = true;

		const FName LevelName = LevelSectionNames[CurrentFloorIndex];

		if (LoadFloorMap(LevelName))
		{
			CurrentMapName = LevelName;
			return true;
		}

	}
	return false;
}

void APersistentLevelControler::UnloadCurrentSection()
{
	if (!bChangingMaps)
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.Linkage = 0;
		LatentAction.UUID = 1;
		LatentAction.ExecutionFunction = FName("MapUnloadCompleted");
		UGameplayStatics::UnloadStreamLevel(this, CurrentMapName, LatentAction, false);

		bChangingMaps = true;
	}
}

void APersistentLevelControler::MapUnloadCompleted()
{
	const FName LevelName = LevelSectionNames[CurrentFloorIndex];

	if (LoadFloorMap(LevelName))
	{
		CurrentMapName = LevelName;
	}
}

void APersistentLevelControler::PostMapLoad()
{
	ULevel* LoadedLevel = GetWorld()->GetCurrentLevel();
	
	CurrentLevel = LoadedLevel;

	if (CurrentLevel.IsValid())
	{
		bLevelFinishedLoading = true;

		if (GameState.IsValid())
		{
			bChangingMaps = false;
			GameState->NotifyMapLoaded();
		}
		
	}
}
