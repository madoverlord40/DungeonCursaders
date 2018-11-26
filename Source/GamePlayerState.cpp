// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerState.h"
#include "Characters/Players/PlayerCharacterBase.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "DefaultPawns/TitlePawnController.h"
#include "DefaultPawns/TitleScreenCameraPawn.h"
#include "DefaultPawns/CharacterSelectPawn.h"
#include "DefaultPawns/CharSelectPawnController.h"


void AGamePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AGamePlayerState::ClientInitialize(class AController* C)
{
	Super::ClientInitialize(C);
	
}

