// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonGameMode.h"
#include "DungeonCrusadesInstance.h"

void ADungeonGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ADungeonGameMode::InitGameState()
{
	Super::InitGameState();

}

void ADungeonGameMode::StartPlay()
{
	Super::StartPlay();
}

void ADungeonGameMode::StartToLeaveMap()
{
	Super::StartToLeaveMap();

}

void ADungeonGameMode::RestartPlayer(AController * NewPlayer)
{
	
	Super::RestartPlayer(NewPlayer);
}
