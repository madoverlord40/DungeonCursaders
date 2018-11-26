// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DataStructures/ItemObject.h"
#include "CurrencyItemObject.generated.h"


USTRUCT(BlueprintType)
struct FGoldBarValue
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GoldPieces;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 SilverPieces;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 CopperPieces;
};
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UCurrencyItemObject : public UItemObject
{
	GENERATED_BODY()
	
public:
	UCurrencyItemObject(const FObjectInitializer& Inititializer = FObjectInitializer::Get());
	
	const FGoldBarValue* GetGoldValue();

	virtual void PostInitProperties() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGoldBarValue GoldValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bUseRandomValues : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GoldMaxRandom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 SilverMaxRandom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 CopperMaxRandom;

};
