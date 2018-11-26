// Fill out your copyright notice in the Description page of Project Settings.

#include "RootGameState.h"
#include "DefaultPawns/TitlePawnController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"

ARootGameState::ARootGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = ROOT_FADE_IN;
}

void ARootGameState::HandleBeginPlay()
{
	Super::HandleBeginPlay();

}

void ARootGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ARootGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (CurrentState)
	{
	case ROOT_FADE_IN:
	{
		TWeakObjectPtr<ATitlePawnController> RootController = Cast<ATitlePawnController>(GetWorld()->GetFirstPlayerController());

		if (RootController.IsValid())
		{
			RootController->PerformWidgetFadeIn();
			CurrentState = ROOT_WAIT;
		}
	}
		break;

	case ROOT_WAIT:
		break;

	case ROOT_FADE_OUT:
	{
		TWeakObjectPtr<ATitlePawnController> RootController = Cast<ATitlePawnController>(GetWorld()->GetFirstPlayerController());

		if (RootController.IsValid())
		{
			RootController->PerformWidgetFadeOut();
			CurrentState = ROOT_WAIT;
		}
	}
		break;

	case ROOT_CHAR_SELECT:
	{
		CurrentState = ROOT_NONE;
		UGameplayStatics::OpenLevel(this, FName("CharSelectLevel"));		
	}
		break;

	case ROOT_LOAD_GAME:
		break;
	case ROOT_SETTINGS:
		break;

	case ROOT_SHUTDOWN:
		FGenericPlatformMisc::RequestExit(false);
		CurrentState = ROOT_NONE;
		break;

	default:
		break;
	}
}

void ARootGameState::SetStartGameState()
{
	CurrentState = ROOT_FADE_OUT;
	NextState = ROOT_CHAR_SELECT;
}

void ARootGameState::SetSettingsState()
{
}

void ARootGameState::ShutdownGame()
{
	CurrentState = ROOT_FADE_OUT;
	NextState = ROOT_SHUTDOWN;
}

void ARootGameState::AdvanceState()
{
	CurrentState = NextState;
}
