// Fill out your copyright notice in the Description page of Project Settings.

#include "TitleScreenGameMode.h"


void ATitleScreenGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}

void ATitleScreenGameMode::InitGameState()
{
	Super::InitGameState();
}

void ATitleScreenGameMode::StartToLeaveMap()
{
	Super::StartToLeaveMap();
}

void ATitleScreenGameMode::StartPlay()
{
	Super::StartPlay();
}
