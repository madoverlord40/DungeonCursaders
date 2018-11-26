// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonGameState.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Sound/AmbientSound.h"
#include "GameModes/DungeonGameMode.h"
#include "DungeonCrusadesInstance.h"
#include "GamePlayerState.h"
#include "LevelControlers/PersistentLevelControler.h"

ADungeonGameState::ADungeonGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADungeonGameState::HandleBeginPlay()
{
	Super::HandleBeginPlay();

}

void ADungeonGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ADungeonGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessFlowStates(DeltaTime);
}

void ADungeonGameState::ProcessFlowStates(float time)
{
	switch (CurrentFlowState)
	{
	case GAME_FLOW_INITIALIZE:
		ProcessInitializeState(time);
		break;
	case LOAD_DUNGEON_MAP:
		ProcessLoadMapState(time);
		break;
	case START_GAME:
		ProcessStartGameState(time);
		break;
	case GAME_IN_PROGRESS:
		ProcessGamePlayState(time);
		break;
	case RETURN_TO_ROOT:
		break;
	case GAME_NONE:
		break;
	default:
		break;
	}
}

void ADungeonGameState::ProcessInitializeState(float time)
{
	DungeonGameMode = Cast<ADungeonGameMode>(GetWorld()->GetAuthGameMode());
	if (DungeonGameMode.IsValid())
	{
		GameInstance = Cast<UDungeonCrusadesInstance>(GetGameInstance());

		if (GameInstance.IsValid())
		{
			PersistentControler = Cast<APersistentLevelControler>(GetWorld()->GetCurrentLevel()->GetLevelScriptActor());
			PlayerController = GetWorld()->SpawnActor<APlayerControllerBase>(PlayerControllerClass);
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

		}
	}

	CurrentFlowState = LOAD_DUNGEON_MAP;
}

void ADungeonGameState::ProcessLoadMapState(float time)
{
	switch (LoadMapState)
	{
	case Load_Map_State:
	{
		PersistentControler->LoadFirstSection();
		LoadMapState = Load_Wait_State;

		break;
	}
	case Load_Wait_State:
		
		break;
	case Load_Exit_State:
		CurrentFlowState = START_GAME;
		StartGameState = Start_Game_State;

		break;
	default:
		break;
	}
}

void ADungeonGameState::ProcessStartGameState(float time)
{
	switch (StartGameState)
	{
	case Start_Game_State:
	{
		if (DungeonGameMode.IsValid() && GameInstance.IsValid())
		{
			if (PlayerController.IsValid())
			{
				TWeakObjectPtr<AActor> StartSpot = DungeonGameMode->ChoosePlayerStart(PlayerController.Get());

				if (StartSpot.IsValid())
				{
					const TSubclassOf<APlayerCharacterBase> PlayerClass = GameInstance->GetSelectedPlayerClass();
					PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacterBase>(PlayerClass);
					if (PlayerCharacter.IsValid() && PersistentControler.IsValid())
					{
						
						PlayerCharacter->SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
						GetWorld()->GetFirstPlayerController()->Possess(PlayerCharacter.Get());
						PersistentControler->PlayIntroMovieSequence();
						StartGameState = Start_Wait_State;
					}
				}
			}

		}
		break;
	}

	case Start_Wait_State:
	{
		
		break;
	}

	case Start_Exit_State:
	{
		IsMapLoaded = false;
		GetWorld()->GetFirstPlayerController()->UnPossess();
		DungeonGameMode->SwapPlayerControllers(GetWorld()->GetFirstPlayerController(), PlayerController.Get());
		PlayerController->Possess(PlayerCharacter.Get());
		PlayerController->AttachToActor(PlayerCharacter.Get(), FAttachmentTransformRules::KeepRelativeTransform);
		PlayerController->EnableInput(PlayerController.Get());

		GamePlayState = Game_Progress_Playing;
		StartGameState = Start_Game_State;
		CurrentFlowState = GAME_IN_PROGRESS;
		break;
	}
	default:
		break;
	}
}

void ADungeonGameState::ProcessGamePlayState(float time)
{
	switch (GamePlayState)
	{

	case Game_Progress_Starting:
	{
		if (PlayerController.IsValid())
		{
			TWeakObjectPtr<AActor> StartSpot = DungeonGameMode->ChoosePlayerStart(PlayerController.Get());

			if (StartSpot.IsValid())
			{
				const TSubclassOf<APlayerCharacterBase> PlayerClass = GameInstance->GetSelectedPlayerClass();
				PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacterBase>(PlayerClass);
				if (PlayerCharacter.IsValid() && PersistentControler.IsValid())
				{
					PlayerCharacter->SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
					PlayerController->Possess(PlayerCharacter.Get());
					PlayerController->AttachToActor(PlayerCharacter.Get(), FAttachmentTransformRules::KeepRelativeTransform);
					PlayerController->EnableInput(PlayerController.Get());
					IsMapLoaded = false;
					GamePlayState = Game_Progress_Playing;
				}
			}
		}

		break;
	}
	case Game_Progress_Playing:
		//do nothing we are playing
		break;

	case Game_Progress_ZoneOut:		
		//PlayerController->UnPossess();
		//PlayerCharacter->Destroy();
		
		PlayerController->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, 3.0f, FLinearColor::Black, false, true);
		
		bIsChangingZones = true;
		GamePlayState = Game_Progress_ZoneWait;
		break;

	case Game_Progress_ZoneWait:
		//when fade done move to zone in

		//zone out is completed, started zoning in, lets wait
		if (IsMapLoaded && bZoneChangeCompleted)
		{
			PlayerController->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 3.0f, FLinearColor::Black);
			TWeakObjectPtr<AActor> StartSpot = DungeonGameMode->ChoosePlayerStart(PlayerController.Get());

			if (StartSpot.IsValid())
			{
				PlayerCharacter->SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
				PlayerController->EnableInput(PlayerController.Get());
			}
			GamePlayState = Game_Progress_Playing;
		}
		//we are zoning out and waiting
		else if (bIsChangingZones && !bZoneChangeCompleted)
		{
			if (PlayerController->PlayerCameraManager->FadeTimeRemaining == 0.0f)
			{
				bZoneChangeCompleted = true;
				GamePlayState = Game_Progress_ZoneIn;
			}
		}
		break;

	case Game_Progress_ZoneIn:
		PersistentControler->LoadNextFloorSection();
		GamePlayState = Game_Progress_ZoneWait;

		//set state to Game_Progress_Starting
		break;
	case Game_Progress_Paused:
		break;
	case Game_Progress_Evaluate:
		break;
	case Game_Progress_Won:
		break;
	case Game_Progress_GameOver:
		break;
	case Game_Progress_Exit:
		break;
	default:
		break;
	}
}


void ADungeonGameState::StartGamePlayState()
{
	if (CurrentFlowState == LOAD_DUNGEON_MAP)
	{
		LoadMapState = Load_Exit_State;
	}
}

void ADungeonGameState::FinishPlayerIntroState()
{
	if (CurrentFlowState == START_GAME)
	{
		StartGameState = Start_Exit_State;
	}
}

void ADungeonGameState::BeginZoneChange()
{
	if (GamePlayState == Game_Progress_Playing)
	{
		bIsChangingZones = true;
		GamePlayState = Game_Progress_ZoneOut;
	}
}

void ADungeonGameState::BeginSectionZoneIn()
{
	if (GamePlayState == Game_Progress_ZoneWait && bIsChangingZones)
	{
		GamePlayState = Game_Progress_ZoneIn;
	}
}
