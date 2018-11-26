// Fill out your copyright notice in the Description page of Project Settings.

#include "CurrencyItemObject.h"




UCurrencyItemObject::UCurrencyItemObject(const FObjectInitializer& Inititializer) : Super(Inititializer)
{
	GoldValue.CopperPieces = 0;
	GoldValue.SilverPieces = 0;
	GoldValue.GoldPieces = 0;

	Type = ItemTypes::ITEM_CURRENCY;
}

const FGoldBarValue* UCurrencyItemObject::GetGoldValue()
{
	if (bUseRandomValues)
	{
		GoldValue.CopperPieces = FMath::RandRange(0, CopperMaxRandom);
		GoldValue.SilverPieces = FMath::RandRange(0, SilverMaxRandom);
		GoldValue.GoldPieces = FMath::RandRange(0, GoldMaxRandom);
	}

	return &GoldValue;
}

void UCurrencyItemObject::PostInitProperties()
{
	Super::PostInitProperties();

	
}
