// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonCrusadesInstance.h"
#include "MoviePlayer.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UDungeonCrusadesInstance::Init()
{
	Super::Init();
// 	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UDungeonCrusadesInstance::BeginLoadingScreen);
// 	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UDungeonCrusadesInstance::EndLoadingScreen);
}

void UDungeonCrusadesInstance::StartGameInstance()
{
	Super::StartGameInstance();

}

void UDungeonCrusadesInstance::LoadComplete(const float LoadTime, const FString & MapName)
{
	Super::LoadComplete(MapLoadTime, MapName);
	CurrentMapName = MapName;
}

void UDungeonCrusadesInstance::OnStart()
{
	Super::OnStart();

}

void UDungeonCrusadesInstance::Shutdown()
{
	Super::Shutdown();

}

void UDungeonCrusadesInstance::BeginLoadingScreen(const FString & InMapName)
{
// 	if (!IsRunningDedicatedServer())
// 	{
// 		//create our own loading screen widget with button to press to start when loading is compelted
// 		FLoadingScreenAttributes LoadingScreen;
// 		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
// 		LoadingScreen.WidgetLoadingScreen = LoadingScreen.NewTestLoadingScreenWidget();
// 
// 		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
// 	}
}

void UDungeonCrusadesInstance::EndLoadingScreen(UWorld * InLoadedWorld)
{
}

void UDungeonCrusadesInstance::FinishMapLoad()
{
	if (bHasMapLoaded)
	{
		
	}
}
