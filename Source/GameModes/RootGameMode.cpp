// Fill out your copyright notice in the Description page of Project Settings.

#include "RootGameMode.h"

ARootGameMode::ARootGameMode()
{

}

void ARootGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}

void ARootGameMode::InitGameState()
{
	Super::InitGameState();

}

void ARootGameMode::StartPlay()
{
	Super::StartPlay();

}

void ARootGameMode::StartToLeaveMap()
{
	Super::StartToLeaveMap();

}

void ARootGameMode::RestartPlayer(AController * NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}
