// Fill out your copyright notice in the Description page of Project Settings.

#include "PersistentGameState.h"
#include "GamePlayerState.h"
#include "PersistentGameMode.h"
#include "Characters/Players/PlayerCharacterBase.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "DefaultPawns/TitlePawnController.h"
#include "DefaultPawns/TitleScreenCameraPawn.h"
#include "DefaultPawns/CharacterSelectPawn.h"
#include "DefaultPawns/CharSelectPawnController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Sound/AmbientSound.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"


APersistentGameState::APersistentGameState() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APersistentGameState::AddPlayerState(APlayerState * PlayerState)
{
	Super::AddPlayerState(PlayerState);

	
}

void APersistentGameState::RemovePlayerState(APlayerState * PlayerState)
{
	Super::RemovePlayerState(PlayerState);

}

void APersistentGameState::HandleBeginPlay()
{
	Super::HandleBeginPlay();

}

void APersistentGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}


void APersistentGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ExecuteGameState(DeltaTime);
}

void APersistentGameState::SetCurrentState(GAMEFLOWSTATES state)
{
	if (state == GAME_PLAY)
	{
		NextGameState = GAME_TRANSITION_IN;
	}
	else
	{
		NextGameState = state;
	}
	CurrentState = STATE_CHANGESTATE;
}

void APersistentGameState::AdvanceNextState()
{
	CurrentState = NextState;
}

void APersistentGameState::SetSelectedCharacterIndex(const FString& KeyName)
{
	PersistentGameMode = Cast<APersistentGameMode>(GetWorld()->GetAuthGameMode());
	if (PersistentGameMode.IsValid())
	{
		PersistentGameMode->DefaultPawnClass = *PlayerClasses.Find(KeyName);
	}
}

void APersistentGameState::PostGameMapLoad()
{
	TWeakObjectPtr<ULevelStreaming> level = UGameplayStatics::GetStreamingLevel(GetWorld(), FName("DungeonFloorOne"));
	if (level.IsValid())
	{
// 		ADungeonFloorController* FloorController = Cast<ADungeonFloorController>(level->GetLevelScriptActor());
// 		if (FloorController)
// 		{
// 			FloorController->GenerateFloorRooms();
 			IsGameLoaded = true;
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
// 		}
	}
}

void APersistentGameState::ExecuteGameState(float DeltaTime)
{
	switch (CurrentGameState)
	{
	case GAME_INITIALIZE:
		ExecuteInitializeState(DeltaTime);
		break;
	case GAME_TITLE_SCREEN:
		ExecuteTitleScreenState(DeltaTime);
		break;
	case GAME_CHARACTER_SELECT:
		ExecuteCharSelectState(DeltaTime);
		break;
	case GAME_TRANSITION_IN:
		ExecuteTransitionInState(DeltaTime);
		break;
	case GAME_TRANSITION_OUT:
		break;
	case GAME_PLAY:
		ExecutePlayState(DeltaTime);
		break;
	case GAME_RETURN_MENU:
		break;
	case GAME_SHUTDOWN:
		FGenericPlatformMisc::RequestExit(false);
		CurrentGameState = GAME_EXIT;
		break;

	case GAME_EXIT:

		break;

	default:
		break;
	}
}

void APersistentGameState::ExecuteInitializeState(float DeltaTime)
{
	switch (CurrentState)
	{
	case STATE_START:
	{
		PersistentGameMode = Cast<APersistentGameMode>(GetWorld()->GetAuthGameMode());
		DefaultController = GetWorld()->GetFirstPlayerController();
		NextGameState = GAME_TITLE_SCREEN;
		CurrentState = STATE_CHANGESTATE;
		break;
	}
	case STATE_INPROGRESS:
		break;
	case STATE_CHANGESTATE:
		CurrentState = STATE_ENDSTATE;

		break;
	case STATE_ENDSTATE:
		CurrentState = STATE_START;
		CurrentGameState = NextGameState;
		break;
	default:
		break;
	}
}

void APersistentGameState::ExecuteTitleScreenState(float DeltaTime)
{
	switch (CurrentState)
	{
	case STATE_START:
		
		CurrentState = STATE_LOAD;
		break;
	case STATE_INPROGRESS:
		break;
	case STATE_CHANGESTATE:
	{
		CurrentState = STATE_UNLOAD;
		break;
	}

	case STATE_LOAD:
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = FName("AdvanceNextState");
		LatentAction.Linkage = 0;
		LatentAction.UUID = 1;
		NextState = STATE_POSTLOAD;
		CurrentState = STATE_WAIT;
		UGameplayStatics::LoadStreamLevel(this, FName("TitleScreenLevel"), true, false, LatentAction);
		break;
	}

	case STATE_POSTLOAD:
	{
		TitleScreenController = GetWorld()->SpawnActor<ATitlePawnController>(PersistentGameMode->TitleScreenControllerClass);
		TitleScreenPawn = GetWorld()->SpawnActor<ATitleScreenCameraPawn>(PersistentGameMode->TitleScreenPawnClass);
		DefaultController = GetWorld()->GetFirstPlayerController();
		PersistentGameMode->SwapPlayerControllers(DefaultController.Get(), TitleScreenController.Get());
		TitleScreenController->Possess(TitleScreenPawn.Get());
		TitleScreenController->PerformWidgetFadeIn();
		
		CurrentState = STATE_INPROGRESS;
		break;
	}

	case STATE_UNLOAD:
	{
		
		TitleScreenPawn->Destroy();
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = FName("AdvanceNextState");
		LatentAction.Linkage = 0;
		LatentAction.UUID = 1;
		NextState = STATE_ENDSTATE;
		CurrentState = STATE_WAIT;
		UGameplayStatics::UnloadStreamLevel(this, FName("TitleScreenLevel"), LatentAction, false);
		break;
	}
	case STATE_ENDSTATE:
		CurrentGameState = NextGameState;
		CurrentState = STATE_START;
		break;

	case STATE_WAIT:

		break;
	default:
		break;
	}
}

void APersistentGameState::ExecuteCharSelectState(float DeltaTime)
{
	switch (CurrentState)
	{
	case STATE_START:
	{
				
		CurrentState = STATE_LOAD;

		break;
	}
	case STATE_INPROGRESS:
		break;
	case STATE_CHANGESTATE:
		CurrentState = STATE_UNLOAD;
		break;

	case STATE_LOAD:
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = FName("AdvanceNextState");
		LatentAction.Linkage = 0;
		LatentAction.UUID = 1;
		NextState = STATE_POSTLOAD;
		CurrentState = STATE_WAIT;
		UGameplayStatics::LoadStreamLevel(this, FName("CharSelectLevel"), true, false, LatentAction);
		break;
	}

	case STATE_POSTLOAD:
	{
		PersistentGameMode = Cast<APersistentGameMode>(GetWorld()->GetAuthGameMode());
		if (PersistentGameMode.IsValid())
		{
			CharSelectController = GetWorld()->SpawnActor<ACharSelectPawnController>(PersistentGameMode->CharSelectControllerClass);
			PersistentGameMode->SwapPlayerControllers(TitleScreenController.Get(), CharSelectController.Get());
			TWeakObjectPtr<AActor> StartSpot = PersistentGameMode->FindPlayerStart(CharSelectController.Get());

			if (StartSpot.IsValid())
			{
				CharSelectPawn = GetWorld()->SpawnActor<ACharacterSelectPawn>(PersistentGameMode->CharSelectPawnClass);
				CharSelectController->Possess(CharSelectPawn.Get());

				CharSelectPawn->SetActorTransform(StartSpot->GetActorTransform());

				//CharSelectController->ActivateStateMachine();

				CurrentState = STATE_INPROGRESS;
			}
		}
		break;
	}

	case STATE_UNLOAD:
	{
		//CharSelectController->DeactivateStateMachine();
		DefaultController = CharSelectController;
		CharSelectPawn->Destroy();

		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = FName("AdvanceNextState");
		LatentAction.Linkage = 0;
		LatentAction.UUID = 1;
		NextState = STATE_ENDSTATE;
		CurrentState = STATE_WAIT;
		UGameplayStatics::UnloadStreamLevel(this, FName("CharSelectLevel"), LatentAction, false);
		break;
	}
	case STATE_ENDSTATE:
		CurrentState = STATE_START;
		CurrentGameState = NextGameState;
		break;
	case STATE_WAIT:

		break;
	default:
		break;
	}
}

void APersistentGameState::ExecutePlayState(float DeltaTime)
{
	switch (CurrentState)
	{
	case STATE_START:
	{
		TWeakObjectPtr<ULevelStreaming> level = UGameplayStatics::GetStreamingLevel(GetWorld(), FName("DungeonFloorOne"));
		if (level.IsValid())
		{
			level->SetShouldBeVisible(true);
			CurrentState = STATE_LOAD;
		}
		break;
	}
	case STATE_LOAD:
	{
		PersistentGameMode = Cast<APersistentGameMode>(GetWorld()->GetAuthGameMode());
		if (PersistentGameMode.IsValid())
		{
			PlayerController = GetWorld()->SpawnActor<APlayerControllerBase>(PersistentGameMode->PlayerCharacterControllerClass);
			PersistentGameMode->SwapPlayerControllers(GetWorld()->GetFirstPlayerController(), PlayerController.Get());
			TWeakObjectPtr<AActor> StartSpot = PersistentGameMode->ChoosePlayerStart(PlayerController.Get());

			if (StartSpot.IsValid())
			{
				PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacterBase>(PersistentGameMode->DefaultPawnClass);
				PlayerCharacter->SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());

				PlayerController->Possess(PlayerCharacter.Get());
				PlayerController->AttachToActor(PlayerCharacter.Get(), FAttachmentTransformRules::KeepRelativeTransform);
				
				//PlayerController->StartPlayerSpawnIntro();
				CurrentState = STATE_WAIT;
				NextState = STATE_POSTLOAD;
			}
		}
		break;
	}
	case STATE_POSTLOAD:
		//PlayerController->ActivateController();
		CurrentState = STATE_INPROGRESS;
		break;
	
	case STATE_UNLOAD:

		break;

	case STATE_INPROGRESS:
		break;
	case STATE_CHANGESTATE:
		break;
	case STATE_ENDSTATE:
		break;
	case STATE_WAIT:

		break;
	default:
		break;
	}
}

void APersistentGameState::ExecuteTransitionInState(float DeltaTime)
{
	switch (CurrentState)
	{
	case STATE_START:
		CurrentState = STATE_LOAD;

		break;

	case STATE_LOAD:
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = FName("AdvanceNextState");
		LatentAction.Linkage = 0;
		LatentAction.UUID = 1;
		NextState = STATE_POSTLOAD;
		CurrentState = STATE_WAIT;
		UGameplayStatics::LoadStreamLevel(this, FName("DungeonTransitionLevel"), true, false, LatentAction);

		break;
	}
	case STATE_POSTLOAD:
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = FName("PostGameMapLoad");
		LatentAction.Linkage = 0;
		LatentAction.UUID = 1;
		NextState = STATE_UNLOAD;
		CurrentState = STATE_WAIT;
		UGameplayStatics::LoadStreamLevel(this, FName("DungeonFloorOne"), false, false, LatentAction);
		break;
	}
	case STATE_UNLOAD:
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = FName("AdvanceNextState");
		LatentAction.Linkage = 0;
		LatentAction.UUID = 1;
		NextState = STATE_ENDSTATE;
		CurrentState = STATE_WAIT;
		UGameplayStatics::UnloadStreamLevel(this, FName("DungeonTransitionLevel"), LatentAction, false);
		break;
	}
	case STATE_ENDSTATE:
		CurrentGameState = GAME_PLAY;
		CurrentState = STATE_START;
		break;
	case STATE_WAIT:
		break;
	default:
		break;
	}
}
