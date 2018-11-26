// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelActors/DungeonDoorActor.h"
#include "PersistentLevelControler.generated.h"


class UInventoryComponent;
class UBattleAttributeController;
class UDungeonCrusadesInstance;
class UPlayerWidgetHUD;
class APlayerCharacterBase;
class UDungeonCrusadesInstance;
class APlayerControllerBase;
class ADungeonGameState;
class ALevelControler;
class ADungeonGameMode;
class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;
class UAbilityManagerComponent;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API APersistentLevelControler : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	APersistentLevelControler(const FObjectInitializer& ObjectIntializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	//unloads loading screen
	//shows loaded map
	//starts movie
	void PlayIntroMovieSequence();

	bool LoadNextFloorSection();

	bool LoadFirstSection();

	void UnloadCurrentSection();

	UFUNCTION()
	void MapUnloadCompleted();

	UFUNCTION()
	void PostMapLoad();

protected:
	bool LoadFloorMap(const FName& LevelName);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint32 bLevelFinishedLoading : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ULevelSequence* IntroSequenceClip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ULevelSequence* OuttroSequenceClip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> LevelSectionNames;

private:
	UPROPERTY()
	TWeakObjectPtr<APlayerCharacterBase> PlayerCharacter;

	UPROPERTY()
	TWeakObjectPtr<UDungeonCrusadesInstance> GameInstance;

	UPROPERTY()
	TWeakObjectPtr<ADungeonGameState> GameState;
	
	UPROPERTY()
	TWeakObjectPtr<ADungeonGameMode> DungeonGameMode;

	UPROPERTY()
	TWeakObjectPtr<APlayerControllerBase> PlayerController;

	UPROPERTY()
	TWeakObjectPtr<ULevel> CurrentLevel;

	UPROPERTY()
	TWeakObjectPtr<ALevelControler> DungeonControler;

	TWeakObjectPtr<ULevelSequencePlayer> IntroSequencePlayer;
	TWeakObjectPtr<ALevelSequenceActor> SequenceActor;

	uint8 CurrentFloorIndex;
	uint8 bIsPlayingMovie;
	FName SelectedDungeonKeyName;
	FName CurrentMapName;
	uint32 bChangingMaps : 1;
};
