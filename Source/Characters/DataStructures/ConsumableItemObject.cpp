// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsumableItemObject.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"



UConsumableItemObject::UConsumableItemObject()
{
	Type = ItemTypes::ITEM_CONSUMABLE;
	GenerateDescriptionString();
}

void UConsumableItemObject::GenerateDescriptionString()
{
	DescriptionString = "This is a <Bold.Green>consumable</> item with the following properties:\n";

	switch (ConsumeType)
	{
	case ConsumableItemTypes::ComsumableType_Healing:
		DescriptionString.Append("\n<Bold.Red>Healing Potion</>\n\nReplenishes Health by a certain amount.\n\n");
		DescriptionString.Append("Base Healing Value: <Bold.Yellow>");
		DescriptionString.Append(FString::SanitizeFloat(BaseValue, 0));
		DescriptionString.Append("</>");
		break;
	case ConsumableItemTypes::ConsumableType_Curing:
		break;
	case ConsumableItemTypes::ConsumableType_Boost_Stats:
		break;
	case ConsumableItemTypes::ConsumableType_Boost_Resistances:
		break;
	default:
		break;
	}

	DescriptionString.Append("\n\n<Bold.Blue>This item is usable.</>");
}
