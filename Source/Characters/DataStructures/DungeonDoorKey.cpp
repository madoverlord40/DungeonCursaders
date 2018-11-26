// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonDoorKey.h"
#include "LevelActors/DungeonDoorActor.h"

UDungeonDoorKey::UDungeonDoorKey(const FObjectInitializer& Inititializer) : Super(Inititializer)
{
	Type = ItemTypes::ITEM_DOOR_KEY;

}

bool UDungeonDoorKey::GetIsUseable()
{
	bIsKeyBroken = (NumberOfUses > 0) ? false : true;

	return !bIsKeyBroken;
}

bool UDungeonDoorKey::UseKey()
{
	bool success = false;

	if (!bIsKeyBroken)
	{
		NumberOfUses--;
		success = true;

		if (NumberOfUses == 0)
		{
			bIsKeyBroken = true;
		}
	}

	return success;
}

void UDungeonDoorKey::GenerateDescriptionString()
{
	DescriptionString = "This is a Dungeon Door Key.\n\n";

	switch (KeyType)
	{
	case DoorKeyTypes::BRONZE_KEY:
		DescriptionString.Append("It is made of a Generic Bronze material and looks like its very old. \n<Bold.Green>Usage:</> Opens any door that requires a <Bold.Red>Bronze Key.</>\n");
		break;
	case DoorKeyTypes::BRASS_KEY:
		DescriptionString.Append("It is made of a Generic Brass material and looks like it's kind of old. \n<Bold.Green>Usage:</> Opens any door that requires a <Bold.Red>Brass Key.</>\n");
		break;
	case DoorKeyTypes::IRON_KEY:
		DescriptionString.Append("It is made of Heavy Iron materials and looks like it could have been used recently. \n<Bold.Green>Usage:</> Opens any door that requires a <Bold.Red>Iron Key.</>\n");
		break;
	case DoorKeyTypes::SILVER_KEY:
		DescriptionString.Append("It is made of a Silver material and looks like its very shiny. \n<Bold.Green>Usage:</> Opens any door that requires a <Bold.Red>Silver Key.</>\n");
		break;
	case DoorKeyTypes::GOLD_KEY:
		DescriptionString.Append("It is made of pure Gold and looks like it has been in someones pocket for quite some time. \n<Bold.Green>Usage:</> Opens any door that requires a <Bold.Red>Gold Key.</>\n");
		break;
	case DoorKeyTypes::SKELETON_KEY:
		DescriptionString = "It is made of a human remains and looks like its ancient and worn. \nUseage: Opens any door that requires a Key.\n";
		break;
	default:
		break;
	}
	DescriptionString.Append(FString("<Bold.Blue>Number of Uses Remaining:</><Bold.Green> "));
	DescriptionString.Append(FString::FromInt((int)NumberOfUses));
	DescriptionString.Append("</>");
}

