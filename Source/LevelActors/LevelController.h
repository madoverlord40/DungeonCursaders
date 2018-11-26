// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelController.generated.h"


class UDungeonDoorKey;
class ADungeonDoorActor;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ALevelController : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ALevelController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	bool UseKeyOnDoor(TWeakObjectPtr<UDungeonDoorKey> DoorKey, TWeakObjectPtr<ADungeonDoorActor> TheDoor);

protected:
	
	
};
