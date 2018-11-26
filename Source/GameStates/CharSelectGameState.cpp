// Fill out your copyright notice in the Description page of Project Settings.

#include "CharSelectGameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "../SelectionPawns/PlayerSelectionActor.h"
#include "Characters/Players/PlayerCharacterBase.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "DefaultPawns/CharacterSelectPawn.h"
#include "DefaultPawns/CharSelectPawnController.h"
#include "Characters/DataStructures/CombatData.h"
#include "DungeonCrusadesInstance.h"

ACharSelectGameState::ACharSelectGameState()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACharSelectGameState::HandleBeginPlay()
{
	Super::HandleBeginPlay();

}

void ACharSelectGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TWeakObjectPtr<UGameInstance> instance = GetWorld()->GetGameInstance();

	if (instance.IsValid())
	{
		GameInstance = Cast<UDungeonCrusadesInstance>(instance.Get());
	}
}

void ACharSelectGameState::SetCurrentSelection(const FString& KeyName)
{
	SelectedCharacterKey = KeyName;
	TWeakObjectPtr<APlayerSelectionActor> SelectedActor = *SpawnedPlayerSelections.Find(SelectedCharacterKey);
	if (SelectedActor.IsValid() && PlayerController.IsValid())
	{
		PlayerController->SetCameraLookAt(SelectedActor);
		GameInstance->SetSelectedPlayerClass(*PlayerClasses.Find(SelectedCharacterKey));
	}
}

void ACharSelectGameState::ChooseCurrentSelection()
{
	NextState = Transition_Game;

	AdvanceState();
}

void ACharSelectGameState::ExitToRoot()
{
	NextState = Transition_Root;
	AdvanceState();
}


void ACharSelectGameState::ShowCharacterProfile(bool show)
{
	if (show)
	{
		CurrentState = Show_Profile;
		CurrentSubState = StartState;
	}
	else
	{
		CurrentState = Exit_Profile;
		CurrentSubState = StartState;
	}
}

void ACharSelectGameState::GetSelectionKeyNames(TArray<FString>& KeyNames)
{
	if (PlayerSelectionClasses.Num() > 0)
	{
		PlayerSelectionClasses.GenerateKeyArray(KeyNames);
	}
}

void ACharSelectGameState::SetEnablePawnLight(const FString& KeyName, bool enable)
{
	TWeakObjectPtr<APlayerSelectionActor> SelectedActor = *SpawnedPlayerSelections.Find(KeyName);
	if (SelectedActor.IsValid()) 
	{
		SelectedActor->SetEnableLight(enable);
	}
	
}

const UCombatData* ACharSelectGameState::GetCombatDataByName(const FString& KeyName)
{
	const TSubclassOf<UCombatData> CombatData = *CombatDataClasses.Find(KeyName);

	if (IsValid(CombatData))
	{
		return CombatData->GetDefaultObject<UCombatData>();
	}

	return nullptr;
}

void ACharSelectGameState::SpawnSelectionPawns()
{
	if (SpawnedPlayerSelections.Num() == 0)
	{
		if (PlayerSelectionClasses.Num() == PlayerSelectionLocations.Num())
		{
			for (const TPair<FString, TSubclassOf<APlayerSelectionActor>>& ActorClassPair : PlayerSelectionClasses)
			{
				const FString& Key = ActorClassPair.Key;
				const TSubclassOf<APlayerSelectionActor>& ActorClass = ActorClassPair.Value;

				const FTransform& SpawnTransform = PlayerSelectionLocations.FindChecked(Key);

				APlayerSelectionActor* SelectionActor = GetWorld()->SpawnActor<APlayerSelectionActor>(ActorClass, SpawnTransform);

				if (IsValid(SelectionActor))
				{
					SpawnedPlayerSelections.Add(Key, SelectionActor);
				}
			}
		}
	}
}

void ACharSelectGameState::ProcessCurrentState(float time)
{
	switch (CurrentState)
	{
	case Initialize:
		ProcessInitState(time);
		break;
	case Start_Cinematic:
		ProcessCinematicState(time);
		break;

	case End_Cinematic:
		ProcessPostCinemaState(time);
		break;

	case User_Wait:
		break;

	case Show_Profile:
		ProcessShowProfileState(time);
		break;

	case Exit_Profile:
		ProcessExitProfileState(time);
		break;
	case Transition_Game:
		ProcessStartGameState(time);
		break;
	case Transition_Root:
		ProcessExitToTitleState(time);
		break;
	
	case Load_Title:
		UGameplayStatics::OpenLevel(this, FName("TitleScreenLevel"));
		CurrentState = None;
		break;

	case Load_Game:
		UGameplayStatics::OpenLevel(this, FName("MainGameLevel"));
		CurrentState = None;
		break;

	case None:
		break;
	default:
		break;
	}
}

void ACharSelectGameState::ProcessInitState(float time)
{
	switch (CurrentSubState)
	{
	case StartState:
		NextState = Start_Cinematic;
		CurrentSubState = ExecuteState;	
		break;
	case ExecuteState:
		SpawnSelectionPawns();
		CurrentSubState = ExitState;
		break;
	case WaitState:
		break;
	case ExitState:
		CurrentSubState = StartState;
		AdvanceState();
		break;
	default:
		break;
	}
}

void ACharSelectGameState::ProcessCinematicState(float time)
{
	switch (CurrentSubState)
	{
	case StartState:
	{
		NextState = End_Cinematic;
		CurrentSubState = ExecuteState;

		PlayerController = Cast<ACharSelectPawnController>(GetWorld()->GetFirstPlayerController());

		break;
	}
	case ExecuteState:
		PlayerController->PlayIntoMovie();
		CurrentSubState = WaitState;
		break;
	case WaitState:
		break;
	case ExitState:
		break;
	default:
		break;
	}
}

void ACharSelectGameState::ProcessPostCinemaState(float time)
{
	switch (CurrentSubState)
	{
	case StartState:
		NextState = User_Wait;
		CurrentSubState = ExecuteState;
		break;
	case ExecuteState:
	{
		TArray<FString> Keys;
		SpawnedPlayerSelections.GenerateKeyArray(Keys);
		SelectedCharacterKey = Keys[0];

		TWeakObjectPtr<APlayerSelectionActor> SelectedActor = *SpawnedPlayerSelections.Find(SelectedCharacterKey);
		if (SelectedActor.IsValid())
		{
			SelectedActor = *SpawnedPlayerSelections.Find(SelectedCharacterKey);
			SelectedActor->SetEnableLight(true);
			PlayerController->SetCameraLookAt(SelectedActor);
			PlayerController->EnableInput(PlayerController.Get());
		}
		CurrentSubState = ExitState;
		break;
	}
	case WaitState:
		break;
	case ExitState:
		AdvanceState();
		break;
	default:
		break;
	}
}

void ACharSelectGameState::ProcessStartGameState(float time)
{
	switch (CurrentSubState)
	{
	case StartState:
		NextState = Load_Game;
		CurrentSubState = ExecuteState;
		break;
	case ExecuteState:
	{
		TWeakObjectPtr<APlayerSelectionActor> SelectedActor = *SpawnedPlayerSelections.Find(SelectedCharacterKey);
		if (SelectedActor.IsValid())
		{
			SelectedActor->SetSelectedForPlay();
		}

		PlayerController->SelectCurrentOption();
		
		CurrentSubState = WaitState;
		break;
	}
	case WaitState:
		break;
	case ExitState:
		break;
	default:
		break;
	}
}

void ACharSelectGameState::ProcessExitToTitleState(float time)
{
	switch (CurrentSubState)
	{
	case StartState:
		NextState = Load_Title;
		CurrentSubState = ExecuteState;

		break;
	case ExecuteState:
		PlayerController->ExitToRootScreen();
		CurrentSubState = WaitState;
		break;

	case WaitState:

		break;
	case ExitState:
		break;
	default:
		break;
	}
}

void ACharSelectGameState::ProcessShowProfileState(float time)
{
	switch (CurrentSubState)
	{
	case StartState:
		NextState = Exit_Profile;
		CurrentSubState = ExecuteState;
		break;
	case ExecuteState:
		PlayerController->ViewCharacterProfile();
		CurrentSubState = WaitState;
		break;
	case WaitState:
		break;
	case ExitState:
		break;
	default:
		break;
	}
}

void ACharSelectGameState::ProcessExitProfileState(float time)
{
	switch (CurrentSubState)
	{
	case StartState:
		NextState = User_Wait;
		CurrentSubState = ExecuteState;
		break;
	case ExecuteState:
		PlayerController->ExitCharacterProfile();
		CurrentSubState = WaitState;
		break;
	case WaitState:
		break;
	case ExitState:
		break;
	default:
		break;
	}
}

void ACharSelectGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProcessCurrentState(DeltaTime);
}

void ACharSelectGameState::AdvanceState()
{
	CurrentSubState = StartState;
	CurrentState = NextState;
}
