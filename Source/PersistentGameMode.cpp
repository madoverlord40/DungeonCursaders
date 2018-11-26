// Fill out your copyright notice in the Description page of Project Settings.

#include "PersistentGameMode.h"



APersistentGameMode::APersistentGameMode() : Super()
{
}


void APersistentGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}

void APersistentGameMode::InitGameState()
{
	Super::InitGameState();

}

void APersistentGameMode::StartPlay()
{
	Super::StartPlay();
	
}

void APersistentGameMode::StartToLeaveMap()
{
	Super::StartToLeaveMap();

}

void APersistentGameMode::RestartPlayer(AController * NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}
