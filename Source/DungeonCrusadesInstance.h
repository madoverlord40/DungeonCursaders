// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Characters/Players/PlayerCharacterBase.h"
#include "DungeonCrusadesInstance.generated.h"

class ULoadingScreenWidget;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UDungeonCrusadesInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init() override;
	virtual void StartGameInstance() override;
	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	/** Called when the game instance is started either normally or through PIE. */
	virtual void OnStart() override;
	/** virtual function to allow custom GameInstances an opportunity to do cleanup when shutting down */
	virtual void Shutdown();

	void SetSelectedPlayerClass(TSubclassOf<APlayerCharacterBase> PlayerClass) { SelectedPlayerClass = PlayerClass; }
	TSubclassOf<APlayerCharacterBase>& GetSelectedPlayerClass() { return SelectedPlayerClass; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APlayerCharacterBase> SelectedPlayerClass;

	FString CurrentMapName;
	FString NextMapToLoad;

	uint32 bHasMapLoaded : 1;

	float MapLoadTime;

public:

	UFUNCTION()
	void BeginLoadingScreen(const FString& InMapName);

	UFUNCTION()
	void EndLoadingScreen(UWorld* InLoadedWorld);

	UFUNCTION(BlueprintCallable)
	bool GetHasMapLoaded() const { return bHasMapLoaded; }

	UFUNCTION(BlueprintCallable)
	void FinishMapLoad();

	UFUNCTION(BlueprintCallable)
	void SetNextMapName(const FString& MapName) { NextMapToLoad = MapName; }
};
