// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelController.h"
#include "DungeonDoorActor.h"

ALevelController::ALevelController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

bool ALevelController::UseKeyOnDoor(TWeakObjectPtr<UDungeonDoorKey> DoorKey, TWeakObjectPtr<ADungeonDoorActor> TheDoor)
{
	bool success = false;

	if (DoorKey.IsValid() && TheDoor.IsValid())
	{
		if (DoorKey->UseKey() && DoorKey->GetKeyType() == TheDoor->GetKeyTypeToOpenDoor())
		{
			TheDoor->PerformDoorOpenClose(true);
			success = true;
		}
	}

	return success;
}
