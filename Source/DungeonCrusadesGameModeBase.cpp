// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonCrusadesGameModeBase.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "DungeonCrusadesInstance.h"

void ADungeonCrusadesGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

}


void ADungeonCrusadesGameModeBase::InitGameState()
{
    Super::InitGameState();

}

void ADungeonCrusadesGameModeBase::StartPlay()
{
    Super::StartPlay();

}

void ADungeonCrusadesGameModeBase::StartToLeaveMap()
{
    Super::StartToLeaveMap();

}

void ADungeonCrusadesGameModeBase::RestartPlayer(AController * NewPlayer)
{
	
    Super::RestartPlayer(NewPlayer);

}

