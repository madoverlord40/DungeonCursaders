// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterSelectGameMode.h"


void ACharacterSelectGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}

void ACharacterSelectGameMode::InitGameState()
{
	Super::InitGameState();

}

void ACharacterSelectGameMode::StartToLeaveMap()
{
	Super::StartToLeaveMap();

}

void ACharacterSelectGameMode::StartPlay()
{
	Super::StartPlay();
	
}

void ACharacterSelectGameMode::RestartPlayer(AController* NewPlayer)
{
	//dont spawn controller or player
	//Super::RestartPlayer(NewPlayer);

	StartingController = NewPlayer;
}

TWeakObjectPtr<AController> ACharacterSelectGameMode::InitializePlayerAndController()
{
	Super::RestartPlayer(StartingController.Get());
	return StartingController;
}
